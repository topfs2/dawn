#pragma once

#include <GL/glew.h>

#include "constants.h"
#include "Image.h"

namespace dawn
{
  class OpenGLUtils
  {
  public:
    static GLuint CreateTexture(Image *image);
    static GLuint CreateShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
    static void _check_gl_error(const char *file, int line);
 
  private:
    static GLuint Compile(GLenum type, const std::string &shader);

    static GLenum ParseFormat(CONSTANTS::PixelFormat format);
    static GLuint ParseBPP(CONSTANTS::PixelFormat format);
  };

  #define check_gl_error() OpenGLUtils::_check_gl_error(__FILE__,__LINE__)
}
