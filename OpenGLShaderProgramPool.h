#pragma once
#include "IResourcePool.h"
#include "ShaderMaterial.h"

#include "OpenGLShaderProgram.h"

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

namespace dawn
{
  class OpenGLShaderProgramPool : public IResourcePool<std::string, OpenGLShaderProgramPtr>
  {
  public:
	  OpenGLShaderProgramPool();
	  virtual ~OpenGLShaderProgramPool();

  protected:
	  virtual std::string id(std::string key);
	  virtual OpenGLShaderProgramPtr LoadResource(std::string key);
  };
}
