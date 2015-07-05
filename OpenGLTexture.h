#pragma once

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>
#include "types.h"

namespace dawn
{
  class OpenGLTexture
  {
  public:
    OpenGLTexture(GLuint texture, etag_t etag = 0);
    ~OpenGLTexture();

    void Bind(GLuint unit);

    GLuint texture;
    etag_t m_etag;
  };

  typedef boost::shared_ptr<OpenGLTexture> OpenGLTexturePtr;
}
