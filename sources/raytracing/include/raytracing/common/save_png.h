#pragma once

#include <string>

namespace raytracer
{
  bool savePNG(const std::string& filename, int width, int height, int stride, const unsigned char* bgrData);
}
