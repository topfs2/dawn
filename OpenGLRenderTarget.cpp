#include "OpenGLRenderTarget.h"

#include <iostream>

using namespace std;
using namespace dawn;

OpenGLRenderTarget::OpenGLRenderTarget(GLuint width, GLuint height, unsigned int colorAttachments)
{
  this->width = width;
  this->height = height;

  // create a framebuffer object
  m_frameBufferObject = 0;
  glGenFramebuffersEXT(1, &m_frameBufferObject);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_frameBufferObject);

  // create a renderbuffer object to store depth info
  GLuint rboId;
  glGenRenderbuffersEXT(1, &rboId);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  GLuint depthBufferID = 0;
  glGenRenderbuffersEXT(1, &depthBufferID);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBufferID);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);

	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
  depthBuffer = OpenGLTexturePtr(new OpenGLTexture(depthBufferID));

  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  if (colorAttachments > 0)
  {
    GLuint textures[colorAttachments];
    glGenTextures(colorAttachments, textures);

    for (unsigned int i = 0; i < colorAttachments; i++)
    {
      glBindTexture(GL_TEXTURE_2D, textures[i]);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

      glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, textures[i], 0);

      this->colorAttachments.push_back(OpenGLTexturePtr(new OpenGLTexture(textures[i])));
    }
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  // check FBO status
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
    cout << "Failed to create fbo" << endl;

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

OpenGLRenderTarget::~OpenGLRenderTarget()
{
}

void OpenGLRenderTarget::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
}

void OpenGLRenderTarget::revertToDisplayRenderTarget()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
