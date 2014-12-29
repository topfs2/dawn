#include "OpenGLShaderProgramPool.h"
#include "OpenGLUtils.h"

#include <iostream>

using namespace std;
using namespace dawn;
using namespace dawn::uniforms;

OpenGLRenderTarget::OpenGLRenderTarget()
{
}

OpenGLRenderTarget::~OpenGLRenderTarget()
{
}

OpenGLRenderTargetPool::OpenGLRenderTargetPool() { }

OpenGLRenderTargetPool::~OpenGLRenderTargetPool() { }

string OpenGLRenderTargetPool::id(ShaderMaterial *k)
{
  return k->path;
}

OpenGLShaderProgramPtr OpenGLRenderTargetPool::LoadResource(ShaderMaterial *k)
{
  size_t colorbuffers = 1;
  OpenGLRenderTarget *target = new OpenGLRenderTarget();

  target->colorTextures = new GLuint[colorbuffers];

  if (colorbuffers > 0)
  {
    glGenTextures(colorbuffers, target->colorTextures);

    for (unsigned int i = 0; i < colorbuffers; i++)
    {
      glBindTexture(GL_TEXTURE_2D, target->colorTextures[i]);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  // create a renderbuffer object to store depth info
  GLuint rboId;
  glGenRenderbuffersEXT(1, &rboId);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, rboId);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
                           width, height);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  // create a framebuffer object
  target->frameBufferObject = 0;
  glGenFramebuffersEXT(1, &target->frameBufferObject);
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, target->frameBufferObject);

  // attach the texture to FBO color attachment point

  for (unsigned int i = 0; i < colorbuffers; i++)
  {
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i,
                              GL_TEXTURE_2D, target->colorTextures[i], 0);
  }

  target->depthBufferID = 0;
  glGenRenderbuffersEXT(1, &target->depthBufferID);
  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, target->depthBufferID);
  glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
                           width, height);

	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                GL_RENDERBUFFER, target->depthBufferID);

  glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

  // check FBO status
  GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
  if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
    cout << "Failed to create fbo" << endl;

  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

  return OpenGLRenderTargetPtr(target);
}
