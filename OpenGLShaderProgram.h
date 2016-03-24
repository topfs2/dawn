#pragma once

#include "types.h"

#include <GL/glew.h>
#include <boost/shared_ptr.hpp>

namespace dawn
{
  class OpenGLShaderProgram
  {
  public:
    OpenGLShaderProgram();
    OpenGLShaderProgram(const std::string &vs, const std::string &fs);
    OpenGLShaderProgram(GLuint program);
    ~OpenGLShaderProgram();

    void Bind();

    void uniform(const std::string &name, GLint v0);
    void uniform(const std::string &name, GLfloat v0);
    void uniform(const std::string &name, vec2f v);
    void uniform(const std::string &name, vec3f v);
    void uniform(const std::string &name, vec4f v);
    void uniform(const std::string &name, mat3f m);
    void uniform(const std::string &name, mat4f m);

    enum AttributeLocation
    {
      POSITION = 0,
      COLOR,
      UV
    };

  private:
    GLint GetUniformLocation(const std::string &name);

    GLuint program;
    typedef std::map<std::string, GLint> UniformLUTMap;
    UniformLUTMap uniformLUT;
  };

  typedef boost::shared_ptr<OpenGLShaderProgram> OpenGLShaderProgramPtr;
}
