#include "raytracing/common/load_png.h"

#include <logging.h>
#include <memory>
#include <string>

#include <windows.h>
#include <wincodec.h>
#include <wrl/client.h>

static const auto Log = logging::scope("PNG");

namespace raytracer
{
  namespace
  {
    // Decodes with an initialized COM; the COM objects are released before the caller uninitializes it
    bool decodePNG(const std::string& filename, int& width, int& height, std::vector<unsigned char>& bgrData)
    {
      using Microsoft::WRL::ComPtr;

      ComPtr<IWICImagingFactory> factory;
      if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory))))
      {
        Log.error([] { return "WIC factory creation failed"; });
        return false;
      }

      const int wideLen = MultiByteToWideChar(CP_UTF8, 0, filename.c_str(), -1, nullptr, 0);
      const auto wideFilename = std::make_unique<wchar_t[]>(wideLen);
      MultiByteToWideChar(CP_UTF8, 0, filename.c_str(), -1, wideFilename.get(), wideLen);

      ComPtr<IWICBitmapDecoder> decoder;
      if (FAILED(factory->CreateDecoderFromFilename(
        wideFilename.get(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
      {
        Log.error([fn = filename] { return "opening file " + fn + " for reading failed"; });
        return false;
      }

      GUID containerFormat;
      if (FAILED(decoder->GetContainerFormat(&containerFormat)) || containerFormat != GUID_ContainerFormatPng)
      {
        Log.error([fn = filename] { return "File " + fn + " is not a PNG file"; });
        return false;
      }

      ComPtr<IWICBitmapFrameDecode> frame;
      ComPtr<IWICFormatConverter> converter;
      if (FAILED(decoder->GetFrame(0, &frame)) || FAILED(factory->CreateFormatConverter(&converter)) ||
        FAILED(converter->Initialize(
          frame.Get(), GUID_WICPixelFormat24bppBGR, WICBitmapDitherTypeNone, nullptr, 0.0,
          WICBitmapPaletteTypeCustom)))
      {
        Log.error([fn = filename] { return "PNG decoding of " + fn + " failed"; });
        return false;
      }

      UINT w = 0;
      UINT h = 0;
      if (FAILED(converter->GetSize(&w, &h)))
      {
        Log.error([fn = filename] { return "PNG size of " + fn + " unknown"; });
        return false;
      }

      // PNG stores the top row first, the bitmap the bottom row first: read the rows in reverse
      const auto stride = (3u * w + 3u) & ~3u;
      bgrData.assign(static_cast<std::size_t>(stride) * h, 0);
      for (UINT row = 0; row < h; ++row)
      {
        const WICRect rect{0, static_cast<INT>(row), static_cast<INT>(w), 1};
        if (FAILED(converter->CopyPixels(&rect, stride, stride, bgrData.data() + (h - 1 - row) * stride)))
        {
          Log.error([r = row] { return "PNG pixel read failed at row " + std::to_string(r); });
          return false;
        }
      }

      width = static_cast<int>(w);
      height = static_cast<int>(h);
      return true;
    }
  }

  bool loadPNG(const std::string& filename, int& width, int& height, std::vector<unsigned char>& bgrData)
  {
    const auto comInit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    const bool comOwned = SUCCEEDED(comInit);
    if (!comOwned && comInit != RPC_E_CHANGED_MODE)
    {
      Log.error([] { return "COM initialization failed"; });
      return false;
    }

    const auto decoded = decodePNG(filename, width, height, bgrData);

    if (comOwned) CoUninitialize();
    return decoded;
  }
}
