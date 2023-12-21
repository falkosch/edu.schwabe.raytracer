#include "raytracing/common/Bitmap.h"
#include "../../stdafx.h"

#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace raytracer {
  Bitmap::Bitmap() : resolution(One<ASizeT>()), stride(), data() {
    init();
  }

  Bitmap::Bitmap(const Size2 &resolutionIn) : resolution(resolutionIn), stride(), data() {
    init();
  }

  Bitmap::~Bitmap() {
    if (data) {
      delete[] data;
      data = nullptr;
    }
  }

  Bitmap::VectorType::ValueType &Bitmap::operator[](const ASizeT index) {
    return data[index];
  }

  const Bitmap::VectorType::ValueType &Bitmap::operator[](const ASizeT index) const {
    return data[index];
  }

  const Size2 Bitmap::getResolution() const {
    return resolution;
  }

  const ASizeT Bitmap::getStride() const {
    return stride;
  }

  const BITMAPINFO Bitmap::getBITMAPINFO() const {
    BITMAPINFO sysBitmapInfo{
        {
            // bmiHeader
            DWORD{sizeof(BITMAPINFOHEADER)},                                       // biSize
            static_cast<LONG>(x(this->resolution)),                                // biWidth
            static_cast<LONG>(y(this->resolution)),                                // biHeight
            WORD{1},                                                               // biPlanes
            WORD{(VectorType::SIZE * sizeof(VectorType::ValueType)) << ASizeT{3}}, // biBitCount
            DWORD{BI_RGB},                                                         // biCompression
            static_cast<DWORD>(y(this->resolution) * this->stride),                // biSizeImage
            LONG{0x0ec4},                                                          // biXPelsPerMeter
            LONG{0x0ec4},                                                          // biYPelsPerMeter
            DWORD{0},                                                              // biClrUsed
            DWORD{0}                                                               // biClrImportant
        },
        {
            // bmiColors
            {0, 0, 0, 0} // [0]
        }
    };
    return sysBitmapInfo;
  }

  const BITMAP Bitmap::getBITMAP() const {
    BITMAPINFO bitmapInfo = this->getBITMAPINFO();
    BITMAP sysBitmap{
        LONG{0},                         // bmType
        bitmapInfo.bmiHeader.biWidth,    // bmWidth
        bitmapInfo.bmiHeader.biHeight,   // bmHeight
        static_cast<LONG>(this->stride), // bmWidthBytes
        bitmapInfo.bmiHeader.biPlanes,   // bmPlanes
        bitmapInfo.bmiHeader.biBitCount, // bmBitsPixel
        static_cast<LPVOID>(this->data)  // bmBits
    };
    return sysBitmap;
  }

  void Bitmap::init() {
    this->stride = bitPad(x(resolution) * VectorType::SIZE, VectorType::SIZE);
    this->data = new VectorType::ValueType[this->stride * y(resolution)];
  }

  Bitmap *const Bitmap::loadPPM(const std::string &filename) {
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "opening file " << filename << " failed" << std::endl;
      return nullptr;
    }

    // grab first two chars of the file and make sure that it has the
    // correct magic cookie for a raw PPM file.
    std::string magic;
    getline(file, magic);
    if (magic.substr(Zero<ASizeT>(), Two<ASizeT>()) != "P6") {
      std::cerr << "File " << filename << " is not a raw PPM file" << std::endl;
      return nullptr;
    }

    // grab the three elements in the header (width, height, maxval).
    std::string dimensions;
    do {
      getline(file, dimensions);
    } while (dimensions[Zero<ASizeT>()] == '#');

    Size2 resolution;
    std::stringstream(dimensions) >> resolution[VectorIndices::X] >> resolution[VectorIndices::Y];

    Bitmap *loaded = new Bitmap(resolution);

    std::string max;
    getline(file, max);
    ASizeT rawMaxValue;
    std::stringstream(max) >> rawMaxValue;

    // grab all the image data in one fell swoop.
    const ASizeT rawStride = VectorType::SIZE * x(resolution);
    std::vector<char, AlignedAllocator<char>> raw =
        std::vector<char, AlignedAllocator<char>>(rawStride * y(resolution));
    file.read(&raw[Zero<ASizeT>()], std::streamsize(raw.size()));
    file.close();

    typedef VectorType::ValueType BitmapValueType;
    typedef std::numeric_limits<BitmapValueType> BitmapValueLimits;

    constexpr BitmapValueType scaleMin = BitmapValueLimits::lowest();
    constexpr BitmapValueType scaleMax = BitmapValueLimits::max();
    const Int4 scale = Int4((scaleMax - scaleMin) / convert<Int4::ValueType>(static_cast<UInt_64>(rawMaxValue)));
    const Int4 scaleOffset = Int4(-convert<Int4::ValueType>(scaleMin));
    const int heighti = convert<int>(y(resolution));

#pragma omp parallel for
    for (int i = Zero<int>(); i < heighti; ++i) {
      ASizeT scanline = i * loaded->stride;
      ASizeT scanlineRaw = rawStride * (y(resolution) - i - One<ASizeT>());

      for (ASizeT j = VectorIndices::X; j < rawStride; j += VectorType::SIZE) {
        const char *const rawData = &raw[scanlineRaw + j];
        const Int4 texel = (Int4(
                                static_cast<BitmapValueType>(*(rawData)), static_cast<BitmapValueType>(*(rawData + 1)),
                                static_cast<BitmapValueType>(*(rawData + 2))
                            )
                            + scaleOffset)
                           * scale;

        BitmapValueType *dataLoaded = &(loaded->data[scanline + j]);
        *(dataLoaded++) = static_cast<BitmapValueType>(x(texel));
        *(dataLoaded++) = static_cast<BitmapValueType>(y(texel));
        *(dataLoaded) = static_cast<BitmapValueType>(z(texel));
      }
    }

    std::cout << "Image " << filename << "; " << x(resolution) << "x" << y(resolution) << " loaded." << std::endl;
    return loaded;
  }

  template <typename T, ASizeT ByteSize>
  void writeObject(std::ostream &file, const T &object) {
    constexpr auto CharCount = ByteSize / sizeof(std::ofstream::char_type);
    std::array<std::ofstream::char_type, CharCount> buffer{};
    std::memcpy(buffer.data(), &object, ByteSize);
    file.write(buffer.data(), CharCount);
  }

  template <typename T>
  void writeVariadicObject(std::ostream &file, const T &object, const ASizeT byteSize) {
    const auto charCount = byteSize / sizeof(std::ofstream::char_type);
    std::vector<std::ofstream::char_type> buffer(charCount);
    std::memcpy(buffer.data(), &object, byteSize);
    file.write(buffer.data(), charCount);
  }

  template <typename T>
  void writeVariadicData(std::ostream &file, const T *const data, const ASizeT byteSize) {
    const auto charCount = byteSize / sizeof(std::ofstream::char_type);
    std::vector<std::ofstream::char_type> buffer(charCount);
    std::memcpy(buffer.data(), data, byteSize);
    file.write(buffer.data(), charCount);
  }

  void Bitmap::saveAsBMP(const std::string &filename) const {
    std::ofstream file(filename.c_str(), std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
      std::cerr << "opening file " << filename << " failed" << std::endl;
      return;
    }

    BITMAPINFO bitmapInfo = getBITMAPINFO();
    BITMAPFILEHEADER bmfh;
    bmfh.bfType = static_cast<WORD>(0x4d42); // == "BM"
    bmfh.bfReserved1 = bmfh.bfReserved2 = Zero<WORD>();
    bmfh.bfOffBits = static_cast<DWORD>(sizeof(BITMAPFILEHEADER)) + bitmapInfo.bmiHeader.biSize;
    bmfh.bfSize = bmfh.bfOffBits + bitmapInfo.bmiHeader.biSizeImage;

    writeObject<BITMAPFILEHEADER, sizeof(BITMAPFILEHEADER)>(file, bmfh);
    writeVariadicObject<BITMAPINFOHEADER>(file, bitmapInfo.bmiHeader, bitmapInfo.bmiHeader.biSize);
    writeVariadicData<UInt_8>(file, this->data, bitmapInfo.bmiHeader.biSizeImage);

    file.flush();
    file.close();
  }
}