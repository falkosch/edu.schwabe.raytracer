#pragma once

#include <string>
#include <vector>

namespace raytracer
{
  // Reads a PNG file of any pixel format as 24-bit BGR, the layout of Bitmap: the bottom row first, each row padded to
  // a multiple of 4 bytes. The alpha channel is dropped.
  bool loadPNG(const std::string& filename, int& width, int& height, std::vector<unsigned char>& bgrData);
}
