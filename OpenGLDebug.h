#pragma once

#include "types.h"

namespace dawn
{
  class OpenGLDebug
  {
  public:
    static void WriteFBO(const std::string &title);

    static void WriteStencil(const std::string &title);

    static void PrintUniformMap(UniformMap uniforms);
  };
}
