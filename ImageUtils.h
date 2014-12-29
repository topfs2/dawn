#pragma once

#include <string>
#include <cstdint>

namespace dawn
{
  class ImageUtils
  {
  public:
    static int WriteImage(const std::string &path, int width, int height, uint8_t *pixels);
  };
}
