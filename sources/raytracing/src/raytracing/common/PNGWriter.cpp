#include "raytracing/common/PNGWriter.h"

#include <iostream>
#include <memory>

#include <windows.h>
#include <wincodec.h>
#include <wrl/client.h>

namespace raytracer
{
  bool savePNG(const std::string& filename, int width, int height, int stride, const unsigned char* bgrData)
  {
    using Microsoft::WRL::ComPtr;

    const auto w = static_cast<UINT>(width);
    const auto h = static_cast<UINT>(height);

    const auto comInit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    const bool comOwned = SUCCEEDED(comInit);
    if (!comOwned && comInit != RPC_E_CHANGED_MODE)
    {
      std::cerr << "COM initialization failed" << std::endl;
      return false;
    }

    ComPtr<IWICImagingFactory> factory;
    if (FAILED(CoCreateInstance(
      CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory))))
    {
      std::cerr << "WIC factory creation failed" << std::endl;
      if (comOwned) CoUninitialize();
      return false;
    }

    const int wideLen = MultiByteToWideChar(CP_UTF8, 0, filename.c_str(), -1, nullptr, 0);
    auto wideFilename = std::make_unique<wchar_t[]>(wideLen);
    MultiByteToWideChar(CP_UTF8, 0, filename.c_str(), -1, wideFilename.get(), wideLen);

    ComPtr<IWICStream> stream;
    if (FAILED(factory->CreateStream(&stream)) ||
      FAILED(stream->InitializeFromFilename(wideFilename.get(), GENERIC_WRITE)))
    {
      std::cerr << "opening file " << filename << " for writing failed" << std::endl;
      if (comOwned) CoUninitialize();
      return false;
    }

    ComPtr<IWICBitmapEncoder> encoder;
    if (FAILED(factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder)) ||
      FAILED(encoder->Initialize(stream.Get(), WICBitmapEncoderNoCache)))
    {
      std::cerr << "PNG encoder initialization failed" << std::endl;
      if (comOwned) CoUninitialize();
      return false;
    }

    ComPtr<IWICBitmapFrameEncode> frame;
    if (FAILED(encoder->CreateNewFrame(&frame, nullptr)) || FAILED(frame->Initialize(nullptr)))
    {
      std::cerr << "PNG frame creation failed" << std::endl;
      if (comOwned) CoUninitialize();
      return false;
    }

    auto pixelFormat = GUID_WICPixelFormat24bppBGR;
    if (FAILED(frame->SetSize(w, h)) || FAILED(frame->SetPixelFormat(&pixelFormat)))
    {
      std::cerr << "PNG frame setup failed" << std::endl;
      if (comOwned) CoUninitialize();
      return false;
    }

    // BMP data is stored bottom-up, but PNG expects top-down — write rows in reverse
    const auto strideBytes = static_cast<UINT>(stride);
    for (UINT row = 0; row < h; ++row) {
      const auto srcRow = (h - 1 - row) * strideBytes;
      if (FAILED(frame->WritePixels(1, strideBytes, strideBytes, const_cast<BYTE *>(bgrData + srcRow)))) {
        std::cerr << "PNG pixel write failed at row " << row << std::endl;
        CoUninitialize();
        return false;
      }
    }

    const bool ok = SUCCEEDED(frame->Commit()) && SUCCEEDED(encoder->Commit());
    if (!ok)
    {
      std::cerr << "PNG commit failed" << std::endl;
    }

    if (comOwned) CoUninitialize();
    return ok;
  }
}
