#pragma once
#include "IResourcePool.h"
#include "ShaderMaterial.h"

#include "OpenGLTexturePool.h"

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

namespace dawn
{
  class OpenGLRenderTargetPool : public IResourcePool<ShaderMaterial *, OpenGLRenderTargetPtr>
  {
  public:
	  OpenGLRenderTargetPool();
	  virtual ~OpenGLRenderTargetPool();

  protected:
	  virtual std::string id(ShaderMaterial *k);
	  virtual OpenGLShaderProgramPtr LoadResource(ShaderMaterial *k);
  };
}
