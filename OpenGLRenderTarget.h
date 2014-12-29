#pragma once
#include "IResourcePool.h"
#include "ShaderMaterial.h"

#include "OpenGLTexture.h"

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

namespace dawn
{
  class OpenGLRenderTarget
  {
  public:
    OpenGLRenderTarget(GLuint width, GLuint height, unsigned int colorAttachments = 1);
    ~OpenGLRenderTarget();

    void Bind();
    static void revertToDisplayRenderTarget();

    typedef std::vector<OpenGLTexturePtr> TextureList;
    TextureList colorAttachments;
    OpenGLTexturePtr depthBuffer;

  private:
    GLuint m_frameBufferObject;
  };

  typedef boost::shared_ptr<OpenGLRenderTarget> OpenGLRenderTargetPtr;
}
