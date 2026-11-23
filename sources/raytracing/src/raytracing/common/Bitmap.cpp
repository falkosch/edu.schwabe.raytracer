#include "raytracing/common/Bitmap.h"
#include "raytracing/common/load_png.h"
#include "raytracing/common/save_png.h"
#include "../../stdafx.h"

#include <array>
#include <cassert>
#include <cstring>
#include <fstream>
#include <logging.h>
#include <sstream>
#include <string>
#include <vector>

static const auto Log = logging::scope("Bitmap");

namespace raytracer
{
    Bitmap::Bitmap() : resolution(One<ASizeT>()), stride(), data()
    {
        init();
    }

    Bitmap::Bitmap(const Size2& resolution) : resolution(resolution), stride(), data()
    {
        init();
    }

    Bitmap::~Bitmap() = default;

    Bitmap::VectorType::ValueType& Bitmap::operator[](const ASizeT index)
    {
        return data[index];
    }

    const Bitmap::VectorType::ValueType& Bitmap::operator[](const ASizeT index) const
    {
        return data[index];
    }

    Size2 Bitmap::getResolution() const
    {
        return resolution;
    }

    ASizeT Bitmap::getStride() const
    {
        return stride;
    }

    BITMAPINFO Bitmap::getBITMAPINFO() const
    {
        const BITMAPINFO sysBitmapInfo{
            {
                // bmiHeader
                DWORD{sizeof(BITMAPINFOHEADER)}, // biSize
                static_cast<LONG>(x(this->resolution)), // biWidth
                static_cast<LONG>(y(this->resolution)), // biHeight
                WORD{1}, // biPlanes
                WORD{(VectorType::SIZE * sizeof(VectorType::ValueType)) << ASizeT{3}}, // biBitCount
                DWORD{BI_RGB}, // biCompression
                static_cast<DWORD>(y(this->resolution) * this->stride), // biSizeImage
                LONG{0x0ec4}, // biXPelsPerMeter
                LONG{0x0ec4}, // biYPelsPerMeter
                DWORD{0}, // biClrUsed
                DWORD{0} // biClrImportant
            },
            {
                // bmiColors
                {0, 0, 0, 0} // [0]
            }
        };
        return sysBitmapInfo;
    }

    BITMAP Bitmap::getBITMAP() const
    {
        const auto [bmiHeader, bmiColors] = this->getBITMAPINFO();
        const BITMAP sysBitmap{
            LONG{0}, // bmType
            bmiHeader.biWidth, // bmWidth
            bmiHeader.biHeight, // bmHeight
            static_cast<LONG>(this->stride), // bmWidthBytes
            bmiHeader.biPlanes, // bmPlanes
            bmiHeader.biBitCount, // bmBitsPixel
            static_cast<LPVOID>(this->data.get()) // bmBits
        };
        return sysBitmap;
    }

    void Bitmap::init()
    {
        this->stride = bitPad(x(resolution) * VectorType::SIZE, VectorType::SIZE);
        this->data.reset(new VectorType::ValueType[this->stride * y(resolution)]);
    }

    std::unique_ptr<Bitmap> Bitmap::loadPPM(const std::string& filename)
    {
        std::ifstream file(filename.c_str(), std::ios::binary);
        if (!file.is_open())
        {
            Log.error([fn = filename] { return "opening file " + fn + " failed"; });
            return nullptr;
        }

        // grab first two chars of the file and make sure that it has the
        // correct magic cookie for a raw PPM file.
        std::string magic;
        getline(file, magic);
        if (magic.substr(Zero<ASizeT>(), Two<ASizeT>()) != "P6")
        {
            Log.error([fn = filename] { return "File " + fn + " is not a raw PPM file"; });
            return nullptr;
        }

        // grab the three elements in the header (width, height, maxval).
        std::string dimensions;
        do
        {
            getline(file, dimensions);
        }
        while (dimensions[Zero<ASizeT>()] == '#');

        Size2 resolution;
        std::stringstream(dimensions) >> resolution[VectorIndices::X] >> resolution[VectorIndices::Y];

        auto loaded = std::make_unique<Bitmap>(resolution);

        std::string max;
        getline(file, max);
        ASizeT rawMaxValue;
        std::stringstream(max) >> rawMaxValue;

        // grab all the image data in one fell swoop.
        const auto rawStride = VectorType::SIZE * x(resolution);
        auto raw = std::vector<char, AlignedAllocator<char>>(rawStride * y(resolution));
        file.read(&raw[Zero<ASizeT>()], static_cast<std::streamsize>(raw.size()));
        file.close();

        using BitmapValueType = VectorType::ValueType;
        using BitmapValueLimits = std::numeric_limits<BitmapValueType>;
        constexpr BitmapValueType scaleMin = BitmapValueLimits::lowest();
        constexpr BitmapValueType scaleMax = BitmapValueLimits::max();
        const auto scale = Int4((scaleMax - scaleMin) / convert<Int4::ValueType>(static_cast<UInt_64>(rawMaxValue)));
        const auto scaleOffset = Int4(-convert<Int4::ValueType>(scaleMin));
        const auto heighti = convert<int>(y(resolution));

#pragma omp parallel for
        for (auto i = Zero<int>(); i < heighti; ++i)
        {
            const auto scanline = i * loaded->stride;
            const auto scanlineRaw = rawStride * (y(resolution) - i - One<ASizeT>());

            for (auto j = VectorIndices::X; j < rawStride; j += VectorType::SIZE)
            {
                const auto rawData = &raw[scanlineRaw + j];
                const auto texel = (Int4(
                            static_cast<BitmapValueType>(*rawData), static_cast<BitmapValueType>(*(rawData + 1)),
                            static_cast<BitmapValueType>(*(rawData + 2))
                        )
                        + scaleOffset)
                    * scale;

                // swap r,g,b to b,g,r
                BitmapValueType* dataLoaded = &(loaded->data[scanline + j]);
                *(dataLoaded++) = static_cast<BitmapValueType>(z(texel));
                *(dataLoaded++) = static_cast<BitmapValueType>(y(texel));
                *(dataLoaded) = static_cast<BitmapValueType>(x(texel));
            }
        }

        Log.info([fn = filename, rx = x(resolution), ry = y(resolution)]
        {
            return "Image " + fn + "; " + std::to_string(rx) + "x" + std::to_string(ry) + " loaded.";
        });
        return loaded;
    }

    std::unique_ptr<Bitmap> Bitmap::loadBMP(const std::string& filename)
    {
        std::ifstream file(filename.c_str(), std::ios::binary);
        if (!file.is_open())
        {
            Log.error([fn = filename] { return "opening file " + fn + " failed"; });
            return nullptr;
        }

        BITMAPFILEHEADER fileHeader{};
        BITMAPINFOHEADER infoHeader{};
        file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
        file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));
        if (!file || fileHeader.bfType != static_cast<WORD>(0x4d42) || infoHeader.biSize < sizeof(BITMAPINFOHEADER))
        {
            Log.error([fn = filename] { return "File " + fn + " is not a BMP file"; });
            return nullptr;
        }
        const auto bitCount = static_cast<ASizeT>(VectorType::SIZE * sizeof(VectorType::ValueType)) << ASizeT{3};
        if (infoHeader.biPlanes != WORD{1} || infoHeader.biBitCount != bitCount
            || infoHeader.biCompression != BI_RGB || infoHeader.biWidth <= LONG{0}
            || infoHeader.biHeight == LONG{0})
        {
            Log.error([fn = filename] { return "File " + fn + " is not an uncompressed 24-bit BMP file"; });
            return nullptr;
        }

        // a negative height stores the top scanline first
        const auto topDown = infoHeader.biHeight < LONG{0};
        const auto height = static_cast<ASizeT>(topDown ? -infoHeader.biHeight : infoHeader.biHeight);
        auto loaded = std::make_unique<Bitmap>(Size2(static_cast<ASizeT>(infoHeader.biWidth), height));

        // the file pads its scanlines to 4 bytes as the bitmap does
        file.seekg(static_cast<std::streamoff>(fileHeader.bfOffBits));
        for (auto row = Zero<ASizeT>(); row < height; ++row)
        {
            const auto scanline = topDown ? height - row - One<ASizeT>() : row;
            file.read(
                reinterpret_cast<char*>(&loaded->data[scanline * loaded->stride]),
                static_cast<std::streamsize>(loaded->stride)
            );
        }
        if (!file)
        {
            Log.error([fn = filename] { return "File " + fn + " is truncated"; });
            return nullptr;
        }

        Log.info([fn = filename, rx = x(loaded->resolution), ry = height]
        {
            return "Image " + fn + "; " + std::to_string(rx) + "x" + std::to_string(ry) + " loaded.";
        });
        return loaded;
    }

    std::unique_ptr<Bitmap> Bitmap::loadPNG(const std::string& filename)
    {
        auto width = Zero<int>();
        auto height = Zero<int>();
        std::vector<unsigned char> bgrData;
        if (!raytracer::loadPNG(filename, width, height, bgrData))
        {
            return nullptr;
        }

        auto loaded = std::make_unique<Bitmap>(Size2(static_cast<ASizeT>(width), static_cast<ASizeT>(height)));
        assert(bgrData.size() == loaded->stride * y(loaded->resolution));
        std::memcpy(loaded->data.get(), bgrData.data(), bgrData.size());

        Log.info([fn = filename, width, height]
        {
            return "Image " + fn + "; " + std::to_string(width) + "x" + std::to_string(height) + " loaded.";
        });
        return loaded;
    }

    template <typename T, ASizeT ByteSize>
    void writeObject(std::ostream& file, const T& object)
    {
        constexpr auto CharCount = ByteSize / sizeof(std::ofstream::char_type);
        std::array<std::ofstream::char_type, CharCount> buffer{};
        std::memcpy(buffer.data(), &object, ByteSize);
        file.write(buffer.data(), CharCount);
    }

    template <typename T>
    void writeVariadicObject(std::ostream& file, const T& object, const ASizeT byteSize)
    {
        const auto charCount = byteSize / sizeof(std::ofstream::char_type);
        std::vector<std::ofstream::char_type> buffer(charCount);
        std::memcpy(buffer.data(), &object, byteSize);
        file.write(buffer.data(), static_cast<streamsize>(charCount));
    }

    template <typename T>
    void writeVariadicData(std::ostream& file, const T* const data, const ASizeT byteSize)
    {
        const auto charCount = byteSize / sizeof(std::ofstream::char_type);
        std::vector<std::ofstream::char_type> buffer(charCount);
        std::memcpy(buffer.data(), data, byteSize);
        file.write(buffer.data(), static_cast<streamsize>(charCount));
    }

    bool Bitmap::saveAsPNG(const std::string& filename) const
    {
        return savePNG(
            filename, static_cast<int>(x(resolution)), static_cast<int>(y(resolution)),
            static_cast<int>(stride), data.get()
        );
    }

    bool Bitmap::saveAsBMP(const std::string& filename) const
    {
        std::ofstream file(filename.c_str(), std::ios::binary | std::ios::trunc);
        if (!file.is_open())
        {
            Log.error([fn = filename] { return "opening file " + fn + " failed"; });
            return false;
        }

        const auto [bmiHeader, bmiColors] = getBITMAPINFO();
        BITMAPFILEHEADER bmfh;
        bmfh.bfType = static_cast<WORD>(0x4d42); // == "BM"
        bmfh.bfReserved1 = bmfh.bfReserved2 = Zero<WORD>();
        bmfh.bfOffBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER)) + bmiHeader.biSize;
        bmfh.bfSize = bmfh.bfOffBits + bmiHeader.biSizeImage;

        writeObject<BITMAPFILEHEADER, sizeof(BITMAPFILEHEADER)>(file, bmfh);
        writeVariadicObject<BITMAPINFOHEADER>(file, bmiHeader, bmiHeader.biSize);
        writeVariadicData<UInt_8>(file, this->data.get(), bmiHeader.biSizeImage);

        file.flush();
        file.close();
        return file.good();
    }
}
