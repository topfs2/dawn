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

string OpenGLTexturePool::id(Image *k)
{
  return k->path();
}

OpenGLTexturePtr OpenGLTexturePool::LoadResource(Image *k)
{
  GLuint texture = OpenGLUtils::CreateTexture(k);
  return OpenGLTexturePtr(new OpenGLTexture(texture));
}
