#pragma once
#include "IResourcePool.h"
#include "OpenGLTexture.h"
#include "Image.h"

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

namespace dawn
{
    class OpenGLTexturePool : public IResourcePool<Pixmap *, OpenGLTexturePtr>
    {
    public:
        OpenGLTexturePool();
        virtual ~OpenGLTexturePool();

    protected:
        virtual std::string id(Pixmap *k);
        virtual bool IsResourceValid(Pixmap *k, OpenGLTexturePtr v);
        virtual OpenGLTexturePtr LoadResource(Pixmap *k);
    };
}
