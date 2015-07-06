#pragma once

#include "Object3D.h"

namespace dawn
{
  class OpenGLDebug
  {
  public:
    static void WriteFBO(const std::string &title);

    static void WriteFilterInput(Object3D *object);
    static void WriteFilterPass(Object3D *object, unsigned int pass);
    static void WriteFilterOutput(Object3D *object);

    static void PrintUniformMap(UniformMap uniforms);
  };
}
