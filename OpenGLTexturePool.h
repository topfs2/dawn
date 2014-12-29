#pragma once
#include "IResourcePool.h"
#include "OpenGLTexture.h"
#include "Image.h"

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

namespace dawn
{
  class OpenGLTexturePool : public IResourcePool<Image *, OpenGLTexturePtr>
  {
  public:
	  OpenGLTexturePool();
	  virtual ~OpenGLTexturePool();

  protected:
	  virtual std::string id(Image *k);
	  virtual OpenGLTexturePtr LoadResource(Image *k);
  };
}
