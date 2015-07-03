#include "OpenGLTexturePool.h"
#include "OpenGLUtils.h"

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;
using namespace dawn;

OpenGLTexturePool::OpenGLTexturePool() { }

OpenGLTexturePool::~OpenGLTexturePool() { }

string OpenGLTexturePool::id(Pixmap *k)
{
  return k->id();
}

OpenGLTexturePtr OpenGLTexturePool::LoadResource(Pixmap *k)
{
  GLuint texture = OpenGLUtils::CreateTexture(k);
  return OpenGLTexturePtr(new OpenGLTexture(texture));
}
