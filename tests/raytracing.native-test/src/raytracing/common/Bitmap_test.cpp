#include "raytracing_tests.h"

#include "TempFile.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace raytracer::test {
  namespace {
    // Bitmap stores each pixel as b,g,r, the bottom scanline first (a Windows DIB). The round-trips
    // exercised here:
    //  - construct a binary PPM in a temp file → loadPPM → assert pixels
    //  - construct a Bitmap in memory → saveAsBMP/saveAsPNG → loadBMP/loadPNG → assert pixels

    void writePPM(const std::filesystem::path& path, ASizeT width, ASizeT height,
                  const std::vector<unsigned char>& rgbRows) {
      std::ofstream out(path, std::ios::binary | std::ios::trunc);
      out << "P6\n" << width << " " << height << "\n255\n";
      out.write(reinterpret_cast<const char*>(rgbRows.data()),
                static_cast<std::streamsize>(rgbRows.size()));
    }

    bool fileStartsWith(const std::filesystem::path& path, const std::string& magic) {
      std::ifstream in(path, std::ios::binary);
      if (!in.is_open()) {
        return false;
      }
      std::vector<char> buffer(magic.size());
      in.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
      return in.gcount() == static_cast<std::streamsize>(magic.size())
             && std::memcmp(buffer.data(), magic.data(), magic.size()) == 0;
    }

    std::uintmax_t fileSize(const std::filesystem::path& path) {
      std::error_code ec;
      const auto sz = std::filesystem::file_size(path, ec);
      return ec ? 0u : sz;
    }

    // A bitmap with a different value in every pixel byte; 5 pixels wide, so each scanline has a padding byte
    std::unique_ptr<Bitmap> patternBitmap() {
      auto bitmap = std::make_unique<Bitmap>(Size2{5, 3});
      for (ASizeT row = 0; row < 3; ++row) {
        for (ASizeT byte = 0; byte < bitmap->getStride(); ++byte) {
          (*bitmap)[row * bitmap->getStride() + byte] = static_cast<Bitmap::VectorType::ValueType>(16 * row + byte);
        }
      }
      return bitmap;
    }

    // The pixel bytes must match, the padding bytes may differ
    void assertSamePixels(const Bitmap& expected, const Bitmap& actual) {
      Assert::AreEqual(x(expected.getResolution()), x(actual.getResolution()), L"width", LINE_INFO());
      Assert::AreEqual(y(expected.getResolution()), y(actual.getResolution()), L"height", LINE_INFO());
      Assert::AreEqual(expected.getStride(), actual.getStride(), L"stride", LINE_INFO());
      const auto rowBytes = Bitmap::VectorType::SIZE * x(expected.getResolution());
      for (ASizeT row = 0; row < y(expected.getResolution()); ++row) {
        for (ASizeT byte = 0; byte < rowBytes; ++byte) {
          const auto index = row * expected.getStride() + byte;
          Assert::AreEqual(static_cast<int>(expected[index]), static_cast<int>(actual[index]), L"pixel byte",
                           LINE_INFO());
        }
      }
    }

    // An uncompressed BMP with the given height (negative: top-down) and bit count, 1 pixel wide
    void writeBMP(const std::filesystem::path& path, const LONG height, const WORD bitCount,
                  const std::vector<unsigned char>& rows) {
      BITMAPINFOHEADER info{};
      info.biSize = sizeof(BITMAPINFOHEADER);
      info.biWidth = 1;
      info.biHeight = height;
      info.biPlanes = 1;
      info.biBitCount = bitCount;
      info.biCompression = BI_RGB;
      info.biSizeImage = static_cast<DWORD>(rows.size());
      BITMAPFILEHEADER header{};
      header.bfType = 0x4d42;
      header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
      header.bfSize = header.bfOffBits + info.biSizeImage;
      std::ofstream out(path, std::ios::binary | std::ios::trunc);
      out.write(reinterpret_cast<const char*>(&header), sizeof(header));
      out.write(reinterpret_cast<const char*>(&info), sizeof(info));
      out.write(reinterpret_cast<const char*>(rows.data()), static_cast<std::streamsize>(rows.size()));
    }
  }

  TEST_CLASS(BitmapTest) {
  public:
    TEST_METHOD(defaultConstructsOneByOne) {
      const Bitmap bitmap{};
      const auto resolution = bitmap.getResolution();
      Assert::AreEqual(static_cast<Size2::ValueType>(1), x(resolution), L"default width is 1",
                       LINE_INFO());
      Assert::AreEqual(static_cast<Size2::ValueType>(1), y(resolution), L"default height is 1",
                       LINE_INFO());
    }

    TEST_METHOD(strideIsRoundedUpToFourBytes) {
      const Bitmap oneByOne{Size2{1, 1}};
      // One pixel is 3 bytes, padded to a 4-byte multiple → stride = 4.
      Assert::AreEqual(static_cast<ASizeT>(4), oneByOne.getStride(), L"1x1 stride", LINE_INFO());

      const Bitmap twoByTwo{Size2{2, 2}};
      // 2 * 3 = 6, padded to 8.
      Assert::AreEqual(static_cast<ASizeT>(8), twoByTwo.getStride(), L"2x2 stride", LINE_INFO());

      const Bitmap fourByFour{Size2{4, 4}};
      // 4 * 3 = 12, already a multiple of 4 → stride = 12.
      Assert::AreEqual(static_cast<ASizeT>(12), fourByFour.getStride(), L"4x4 stride aligned",
                       LINE_INFO());
    }

    TEST_METHOD(operatorIndexReadWrite) {
      Bitmap bitmap{Size2{2, 1}};
      bitmap[0] = 0x10; // B
      bitmap[1] = 0x20; // G
      bitmap[2] = 0x30; // R
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(0x10), bitmap[0], L"B byte",
                       LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(0x20), bitmap[1], L"G byte",
                       LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(0x30), bitmap[2], L"R byte",
                       LINE_INFO());
    }

    TEST_METHOD(loadPPMReturnsNullForMissingFile) {
      const auto loaded = Bitmap::loadPPM("this_file_should_not_exist_xyzzy.ppm");
      Assert::IsNull(loaded.get(), L"missing file returns null", LINE_INFO());
    }

    TEST_METHOD(loadPPMRoundTripOnePixel) {
      TempFile temp{".ppm"};
      // PPM is RGB triples, top-to-bottom. Single red pixel.
      const std::vector<unsigned char> rgb{255, 0, 0};
      writePPM(temp.path, 1, 1, rgb);

      const auto loaded = Bitmap::loadPPM(temp.str());
      Assert::IsNotNull(loaded.get(), L"loadPPM should succeed", LINE_INFO());
      Assert::AreEqual(static_cast<Size2::ValueType>(1), x(loaded->getResolution()),
                       L"resolution x", LINE_INFO());
      Assert::AreEqual(static_cast<Size2::ValueType>(1), y(loaded->getResolution()),
                       L"resolution y", LINE_INFO());

      // Bitmap stores the pixel as BGR triples, as BITMAPINFO, saveAsBMP and saveAsPNG read them:
      // data[0]=B, [1]=G, [2]=R for a max-value=255 PPM.
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(0), (*loaded)[0],
                       L"channel 0 = B", LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(0), (*loaded)[1],
                       L"channel 1 = G", LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(255), (*loaded)[2],
                       L"channel 2 = R", LINE_INFO());
    }

    TEST_METHOD(loadPPMFlipsYAxis) {
      TempFile temp{".ppm"};
      // 1x2 PPM: top row red, bottom row green (PPM is top-to-bottom on disk).
      const std::vector<unsigned char> rgb{255, 0, 0, 0, 255, 0};
      writePPM(temp.path, 1, 2, rgb);

      const auto loaded = Bitmap::loadPPM(temp.str());
      Assert::IsNotNull(loaded.get(), L"load must succeed", LINE_INFO());

      // The loader writes the bottom-most row (last on disk) into the first scanline of memory.
      // So data row 0 corresponds to disk row 1 (green).
      // Stride may include padding; only the first three bytes are pixel data for a 1-pixel row.
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(0), (*loaded)[0],
                       L"row 0 B", LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(255), (*loaded)[1],
                       L"row 0 G (was disk-bottom green)", LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(0), (*loaded)[2],
                       L"row 0 R (was disk-bottom green)", LINE_INFO());
    }

    TEST_METHOD(loadPPMRejectsNonP6) {
      TempFile temp{".ppm"};
      std::ofstream out(temp.path, std::ios::binary | std::ios::trunc);
      out << "P3\n1 1\n255\n255 0 0\n";
      out.close();

      const auto loaded = Bitmap::loadPPM(temp.str());
      Assert::IsNull(loaded.get(), L"P3 (ascii) PPM is rejected", LINE_INFO());
    }

    TEST_METHOD(saveAsBMPCreatesFileWithBMMagic) {
      TempFile temp{".bmp"};
      Bitmap bitmap{Size2{4, 4}};
      // Fill with a deterministic pattern.
      for (ASizeT i = 0; i < bitmap.getStride() * 4; ++i) {
        bitmap[i] = static_cast<Bitmap::VectorType::ValueType>(i & 0xff);
      }
      Assert::IsTrue(bitmap.saveAsBMP(temp.str()), L"saveAsBMP returns success", LINE_INFO());
      Assert::IsTrue(std::filesystem::exists(temp.path), L"file exists", LINE_INFO());
      Assert::IsTrue(fileStartsWith(temp.path, "BM"), L"BMP magic 'BM'", LINE_INFO());
      Assert::IsTrue(fileSize(temp.path) > sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER),
                     L"file holds header + pixel data", LINE_INFO());
    }

    TEST_METHOD(saveAsPNGCreatesFileWithPNGMagic) {
      TempFile temp{".png"};
      Bitmap bitmap{Size2{2, 2}};
      for (ASizeT i = 0; i < bitmap.getStride() * 2; ++i) {
        bitmap[i] = static_cast<Bitmap::VectorType::ValueType>(i & 0xff);
      }
      Assert::IsTrue(bitmap.saveAsPNG(temp.str()), L"saveAsPNG returns success", LINE_INFO());
      Assert::IsTrue(std::filesystem::exists(temp.path), L"file exists", LINE_INFO());
      // PNG signature: 89 50 4E 47 0D 0A 1A 0A.
      const std::string pngMagic{'\x89', 'P', 'N', 'G', '\r', '\n', '\x1a', '\n'};
      Assert::IsTrue(fileStartsWith(temp.path, pngMagic), L"PNG magic", LINE_INFO());
    }

    TEST_METHOD(saveAsBMPHandlesOneByOne) {
      TempFile temp{".bmp"};
      Bitmap bitmap{Size2{1, 1}};
      bitmap[0] = 200; // B
      bitmap[1] = 100; // G
      bitmap[2] = 50;  // R
      Assert::IsTrue(bitmap.saveAsBMP(temp.str()), L"1x1 saveAsBMP succeeds", LINE_INFO());
      Assert::IsTrue(fileStartsWith(temp.path, "BM"), L"1x1 BMP magic", LINE_INFO());
    }

    TEST_METHOD(saveAsBMPHandlesWideImage) {
      TempFile temp{".bmp"};
      Bitmap bitmap{Size2{64, 1}};
      Assert::IsTrue(bitmap.saveAsBMP(temp.str()), L"64x1 saveAsBMP succeeds", LINE_INFO());
      Assert::IsTrue(fileStartsWith(temp.path, "BM"), L"wide BMP magic", LINE_INFO());
    }

    TEST_METHOD(saveAsBMPHandlesTallImage) {
      TempFile temp{".bmp"};
      Bitmap bitmap{Size2{1, 64}};
      Assert::IsTrue(bitmap.saveAsBMP(temp.str()), L"1x64 saveAsBMP succeeds", LINE_INFO());
      Assert::IsTrue(fileStartsWith(temp.path, "BM"), L"tall BMP magic", LINE_INFO());
    }

    TEST_METHOD(saveAsBMPRejectsBadPath) {
      Bitmap bitmap{Size2{2, 2}};
      // A path containing a non-existent directory should fail to open.
      const std::string badPath = "Z:/__nonexistent_directory__/file.bmp";
      Assert::IsFalse(bitmap.saveAsBMP(badPath), L"saveAsBMP should fail on bad path",
                      LINE_INFO());
    }

    TEST_METHOD(saveAsBMPLoadBMPRoundTrip) {
      TempFile temp{".bmp"};
      const auto bitmap = patternBitmap();
      Assert::IsTrue(bitmap->saveAsBMP(temp.str()), L"saveAsBMP succeeds", LINE_INFO());
      const auto loaded = Bitmap::loadBMP(temp.str());
      Assert::IsNotNull(loaded.get(), L"loadBMP succeeds", LINE_INFO());
      assertSamePixels(*bitmap, *loaded);
    }

    TEST_METHOD(loadBMPReadsBottomUpRows) {
      TempFile temp{".bmp"};
      // two scanlines of one pixel, each padded to 4 bytes: bottom blue, top red
      writeBMP(temp.path, 2, 24, {255, 0, 0, 0, 0, 0, 255, 0});
      const auto loaded = Bitmap::loadBMP(temp.str());
      Assert::IsNotNull(loaded.get(), L"loadBMP succeeds", LINE_INFO());
      Assert::AreEqual(255, static_cast<int>((*loaded)[0]), L"row 0 B", LINE_INFO());
      Assert::AreEqual(255, static_cast<int>((*loaded)[loaded->getStride() + 2]), L"row 1 R", LINE_INFO());
    }

    TEST_METHOD(loadBMPFlipsTopDownRows) {
      TempFile temp{".bmp"};
      // a negative height stores the top scanline first: top red, bottom blue
      writeBMP(temp.path, -2, 24, {0, 0, 255, 0, 255, 0, 0, 0});
      const auto loaded = Bitmap::loadBMP(temp.str());
      Assert::IsNotNull(loaded.get(), L"loadBMP succeeds", LINE_INFO());
      Assert::AreEqual(static_cast<Size2::ValueType>(2), y(loaded->getResolution()), L"height", LINE_INFO());
      Assert::AreEqual(255, static_cast<int>((*loaded)[0]), L"row 0 B", LINE_INFO());
      Assert::AreEqual(255, static_cast<int>((*loaded)[loaded->getStride() + 2]), L"row 1 R", LINE_INFO());
    }

    TEST_METHOD(loadBMPRejectsOtherBitCounts) {
      TempFile temp{".bmp"};
      writeBMP(temp.path, 1, 32, {1, 2, 3, 4});
      Assert::IsNull(Bitmap::loadBMP(temp.str()).get(), L"32-bit BMP is rejected", LINE_INFO());
    }

    TEST_METHOD(loadBMPRejectsTruncatedFile) {
      TempFile temp{".bmp"};
      writeBMP(temp.path, 2, 24, {255, 0, 0, 0});
      Assert::IsNull(Bitmap::loadBMP(temp.str()).get(), L"missing scanline is rejected", LINE_INFO());
    }

    TEST_METHOD(loadBMPRejectsOtherFiles) {
      TempFile temp{".bmp"};
      writePPM(temp.path, 1, 1, {255, 0, 0});
      Assert::IsNull(Bitmap::loadBMP(temp.str()).get(), L"PPM is rejected", LINE_INFO());
      Assert::IsNull(Bitmap::loadBMP("this_file_should_not_exist_xyzzy.bmp").get(), L"missing file", LINE_INFO());
    }

    TEST_METHOD(saveAsPNGLoadPNGRoundTrip) {
      TempFile temp{".png"};
      const auto bitmap = patternBitmap();
      Assert::IsTrue(bitmap->saveAsPNG(temp.str()), L"saveAsPNG succeeds", LINE_INFO());
      const auto loaded = Bitmap::loadPNG(temp.str());
      Assert::IsNotNull(loaded.get(), L"loadPNG succeeds", LINE_INFO());
      assertSamePixels(*bitmap, *loaded);
    }

    TEST_METHOD(loadPNGRejectsOtherFiles) {
      TempFile temp{".png"};
      Assert::IsTrue(patternBitmap()->saveAsBMP(temp.str()), L"saveAsBMP succeeds", LINE_INFO());
      Assert::IsNull(Bitmap::loadPNG(temp.str()).get(), L"BMP is rejected", LINE_INFO());
      Assert::IsNull(Bitmap::loadPNG("this_file_should_not_exist_xyzzy.png").get(), L"missing file", LINE_INFO());
    }

    TEST_METHOD(allBlackPPMRoundTrip) {
      TempFile temp{".ppm"};
      const std::vector<unsigned char> rgb{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      writePPM(temp.path, 2, 2, rgb);

      const auto loaded = Bitmap::loadPPM(temp.str());
      Assert::IsNotNull(loaded.get(), L"load all-black succeeds", LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(0), (*loaded)[0],
                       L"black R", LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(0), (*loaded)[1],
                       L"black G", LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(0), (*loaded)[2],
                       L"black B", LINE_INFO());
    }

    TEST_METHOD(allWhitePPMRoundTrip) {
      TempFile temp{".ppm"};
      const std::vector<unsigned char> rgb{255, 255, 255, 255, 255, 255};
      writePPM(temp.path, 2, 1, rgb);

      const auto loaded = Bitmap::loadPPM(temp.str());
      Assert::IsNotNull(loaded.get(), L"load all-white succeeds", LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(255), (*loaded)[0],
                       L"white R", LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(255), (*loaded)[1],
                       L"white G", LINE_INFO());
      Assert::AreEqual(static_cast<Bitmap::VectorType::ValueType>(255), (*loaded)[2],
                       L"white B", LINE_INFO());
    }
  };
}
