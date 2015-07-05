#include "OpenGLTexture.h"

using namespace dawn;

OpenGLTexture::OpenGLTexture(GLuint texture, etag_t etag) : texture(texture), m_etag(etag)
{
}

OpenGLTexture::~OpenGLTexture()
{
/*
  if (texture)
    glDeleteProgram(texture);
*/
}

void OpenGLTexture::Bind(GLuint unit)
{
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, texture);
}
