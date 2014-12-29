#pragma once

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

namespace dawn
{
  class OpenGLTexture
  {
  public:
    OpenGLTexture(const std::string &vs, const std::string &fs);
    OpenGLTexture(GLuint texture);
    ~OpenGLTexture();

    void Bind(GLuint unit);

    GLuint texture;
  };

  typedef boost::shared_ptr<OpenGLTexture> OpenGLTexturePtr;
}
