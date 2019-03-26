#include "raytracing/common/Bitmap.h"
#include "../../stdafx.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace raytracer
{

	Bitmap::Bitmap()
		: resolution(One<ASizeT>()), stride(), data()
	{
		init();
	}

	Bitmap::Bitmap(const Size2 & resolutionIn)
		: resolution(resolutionIn), stride(), data()
	{
		init();
	}

	Bitmap::~Bitmap()
	{
		if (data) {
			delete[] data;
			data = nullptr;
		}
	}

	Bitmap::VectorType::ValueType & Bitmap::operator[](const ASizeT index)
	{
		return data[index];
	}

	const Bitmap::VectorType::ValueType & Bitmap::operator[](const ASizeT index) const
	{
		return data[index];
	}

	const Size2 Bitmap::getResolution() const
	{
		return resolution;
	}

	const ASizeT Bitmap::getStride() const
	{
		return stride;
	}

	const BITMAPINFO Bitmap::getBITMAPINFO() const
	{
		BITMAPINFO sysBitmapInfo;
		sysBitmapInfo.bmiHeader.biSize = static_cast<DWORD>(sizeof(BITMAPINFOHEADER));
		sysBitmapInfo.bmiHeader.biWidth = static_cast<LONG>(x(resolution));
		sysBitmapInfo.bmiHeader.biHeight = static_cast<LONG>(y(resolution));
		sysBitmapInfo.bmiHeader.biPlanes = One<WORD>();
		sysBitmapInfo.bmiHeader.biBitCount = static_cast<WORD>((VectorType::SIZE * sizeof(VectorType::ValueType)) << static_cast<ASizeT>(3));
		sysBitmapInfo.bmiHeader.biCompression = static_cast<DWORD>(BI_RGB);
		sysBitmapInfo.bmiHeader.biSizeImage = static_cast<DWORD>(y(resolution) * stride);
		sysBitmapInfo.bmiHeader.biXPelsPerMeter = static_cast<LONG>(0x0ec4);
		sysBitmapInfo.bmiHeader.biYPelsPerMeter = sysBitmapInfo.bmiHeader.biXPelsPerMeter;
		sysBitmapInfo.bmiHeader.biClrUsed = Zero<DWORD>();
		sysBitmapInfo.bmiHeader.biClrImportant = Zero<DWORD>();
		return sysBitmapInfo;
	}

	const BITMAP Bitmap::getBITMAP() const
	{
		BITMAPINFO bitmapInfo = getBITMAPINFO();
		BITMAP sysBitmap;
		sysBitmap.bmType = Zero<LONG>();
		sysBitmap.bmWidth = bitmapInfo.bmiHeader.biWidth;
		sysBitmap.bmHeight = bitmapInfo.bmiHeader.biHeight;
		sysBitmap.bmWidthBytes = static_cast<LONG>(stride);
		sysBitmap.bmPlanes = bitmapInfo.bmiHeader.biPlanes;
		sysBitmap.bmBitsPixel = bitmapInfo.bmiHeader.biBitCount;
		sysBitmap.bmBits = static_cast<LPVOID>(data);
		return sysBitmap;
	}

	void Bitmap::init()
	{
		const ASizeT tstride = bitpad(x(resolution) * VectorType::SIZE, VectorType::SIZE);
		this->stride = tstride;
		this->data = new VectorType::ValueType[tstride * y(resolution)];
	}

	Bitmap * const Bitmap::loadPPM(const std::string & filename)
	{
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
		std::vector<char, AlignedAllocator<char>> raw = std::vector<char, AlignedAllocator<char>>(rawStride * y(resolution));
		file.read(&raw[Zero<ASizeT>()], std::streamsize(raw.size()));
		file.close();

		typedef VectorType::ValueType BitmapValueType;
		typedef std::numeric_limits<BitmapValueType> BitmapValueLimits;

		const BitmapValueType MIN = BitmapValueLimits::lowest();
		const BitmapValueType MAX = BitmapValueLimits::max();
		const Int4 VSCALE = Int4(
			(MAX - MIN) / convert<Int4::ValueType>(static_cast<UInt_64>(rawMaxValue))
		);
		const Int4 VOFFSET = Int4(-convert<Int4::ValueType>(MIN));
		const int heighti = convert<int>(y(resolution));

#pragma omp parallel for
		for (int i = Zero<int>(); i < heighti; ++i) {
			ASizeT scanline = i * loaded->stride;
			ASizeT scanlineRaw = rawStride * (y(resolution) - i - One<ASizeT>());

			for (ASizeT j = VectorIndices::X; j < rawStride; j += VectorType::SIZE) {
				const char * const rawData = &raw[scanlineRaw + j];
				const Int4 texel = (Int4(
					static_cast<BitmapValueType>(*(rawData)),
					static_cast<BitmapValueType>(*(rawData + 1)),
					static_cast<BitmapValueType>(*(rawData + 2))
				) + VOFFSET) * VSCALE;

				BitmapValueType * dataLoaded = &(loaded->data[scanline + j]);
				*(dataLoaded++) = static_cast<BitmapValueType>(x(texel));
				*(dataLoaded++) = static_cast<BitmapValueType>(y(texel));
				*(dataLoaded) = static_cast<BitmapValueType>(z(texel));
			}
		}

		std::cout << "Image " << filename << "; " << x(resolution) << "x" << y(resolution) << " loaded." << std::endl;
		return loaded;
	}

	void Bitmap::saveAsBMP(const std::string & filename) const
	{
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

		file.write(reinterpret_cast<char*>(&bmfh), sizeof(BITMAPFILEHEADER));
		file.write(reinterpret_cast<char*>(&bitmapInfo.bmiHeader), bitmapInfo.bmiHeader.biSize);
		file.write(reinterpret_cast<char*>(data), bitmapInfo.bmiHeader.biSizeImage);

		file.flush();
		file.close();
	}

}
