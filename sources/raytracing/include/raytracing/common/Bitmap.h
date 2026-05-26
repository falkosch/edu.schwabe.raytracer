#pragma once

#include "Image.h"

#include <limits>
#include <memory>
#include <windows.h>

namespace raytracer
{
  using namespace vectorization;

  // Bitmap with 8-bit-width red, green and blue color-channels but no alpha-channel.
  // The width of each scanline is padded to be a multiple of 4 bytes.
  class Bitmap final
  {
  public:
    typedef struct _BitmapVectorType
    {
      static constexpr ASizeT SIZE = VectorSizes::Z;
      typedef UInt_8 ValueType;
      typedef UInt_8 BoolType;
    } VectorType;

  private:
    Size2 resolution;

    ASizeT stride;

    std::unique_ptr<VectorType::ValueType[]> data;

    void init();

  public:
    Bitmap();

    explicit Bitmap(const Size2& resolution);

    template <typename ImageVectorType>
    explicit Bitmap(const Image<ImageVectorType>& image) : resolution(image.getResolution()), stride(), data()
    {
      typedef VectorType::ValueType BitmapValueType;
      typedef std::numeric_limits<BitmapValueType> BitmapValueLimits;

      init();
      const auto BMIN = Int4(convert<Int4::ValueType>(BitmapValueLimits::lowest()));
      const auto BMAX = Int4(convert<Int4::ValueType>(BitmapValueLimits::max()));
      const auto VMIN = convert<ImageVectorType>(BMIN);
      const auto VMAX = convert<ImageVectorType>(BMAX);
      const auto heighti = convert<int>(y(resolution));

#pragma omp parallel for
      for (auto yi = Zero<int>(); yi < heighti; ++yi)
      {
        const auto sy = convert<ASizeT>(yi);
        const auto rx = x(resolution);
        const auto scanlineIn = sy * rx;
        const auto scanlineOut = sy * stride;

        for (auto sx = Zero<ASizeT>(); sx < rx; ++sx)
        {
          const auto scaled = clamp(convert<Int4>(image[scanlineIn + sx] * VMAX + VMIN), BMIN, BMAX);
          BitmapValueType* dataOut = &data[scanlineOut + sx * VectorType::SIZE];
          // need to swap r,g,b to b,g,r
          *(dataOut++) = convert<BitmapValueType>(z(scaled)); // blue
          *(dataOut++) = convert<BitmapValueType>(y(scaled)); // green
          *dataOut = convert<BitmapValueType>(x(scaled)); // red
        }
      }
    }

    ~Bitmap();

    VectorType::ValueType& operator[](ASizeT index);

    const VectorType::ValueType& operator[](ASizeT index) const;

    Size2 getResolution() const;

    ASizeT getStride() const;

    BITMAPINFO getBITMAPINFO() const;

    BITMAP getBITMAP() const;

    bool saveAsBMP(const std::string& filename) const;

    bool saveAsPNG(const std::string& filename) const;

    static std::unique_ptr<Bitmap> loadPPM(const std::string& filename);
  };
}
