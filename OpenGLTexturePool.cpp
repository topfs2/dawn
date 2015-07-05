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

bool OpenGLTexturePool::IsResourceValid(Pixmap *k, OpenGLTexturePtr v)
{
    return !k->isChanged(v->m_etag, true);
}

OpenGLTexturePtr OpenGLTexturePool::LoadResource(Pixmap *k)
{
    GLuint texture = OpenGLUtils::CreateTexture(k);
    etag_t etag = k->lastChange(true); // TODO shouldn't need to get this again, possibly already did it in IsResourceValid!

    return OpenGLTexturePtr(new OpenGLTexture(texture, etag));
}
