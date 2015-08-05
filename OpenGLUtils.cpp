#include "OpenGLUtils.h"
#include "OpenGLShaderProgram.h"

#include <cstdarg>

using namespace std;
using namespace dawn;

bool checkOpenGLError(const char *file, int line, const char *format, ...)
{
  GLenum glErr;
  bool error = false;

  glErr = glGetError();
  while (glErr != GL_NO_ERROR)
  {
    cerr << "glError in file " << file << " @ line " << line << ": " << gluErrorString(glErr) << endl;
    if (format != NULL)
    {
      va_list va;
      va_start(va, format);

      printf(format, va);
      printf("\n");
      va_end(va);
    }

    error = true;
    glErr = glGetError();
  }
  return error;
}

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = new char[infologLength];
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        delete [] infoLog;
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = new char[infologLength];
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        delete [] infoLog;
    }
}

GLenum OpenGLUtils::ParseFormat(CONSTANTS::PixelFormat format)
{
  switch (format)
  {
    case CONSTANTS::AlphaFormat:
      return GL_ALPHA;

    case CONSTANTS::RGBFormat:
      return GL_RGB;

    case CONSTANTS::RGBAFormat:
      return GL_RGBA;

    case CONSTANTS::BGRFormat:
      return GL_BGR;

    case CONSTANTS::BGRAFormat:
      return GL_BGRA;

    case CONSTANTS::LuminanceFormat:
      return GL_LUMINANCE;
  };
}

GLuint OpenGLUtils::ParseBPP(CONSTANTS::PixelFormat format)
{
  switch (format)
  {
    case CONSTANTS::AlphaFormat:
    case CONSTANTS::LuminanceFormat:
      return 1;

    case CONSTANTS::RGBFormat:
    case CONSTANTS::BGRFormat:
      return 3;

    case CONSTANTS::RGBAFormat:
    case CONSTANTS::BGRAFormat:
      return 4;
  };
}

GLuint OpenGLUtils::Compile(GLenum shaderType, const string &shader)
{
  GLuint s = glCreateShader(shaderType);
  if (s == 0 || checkOpenGLError(__FILE__, __LINE__, NULL))
  {
    cerr << "Failed to create shader from\n====" << endl;
    cerr << shader << endl;
    cerr << "===" << endl;

    return 0;
  }

  const char * cstr = shader.c_str();
  glShaderSource(s, 1, &cstr, NULL);
  if (checkOpenGLError(__FILE__, __LINE__, NULL))
  {
    cerr << "Failed to add shader source\n====" << endl;
    cerr << shader << endl;
    cerr << "===" << endl;
    glDeleteShader(s);

    return 0;
  }

  glCompileShader(s);

  printShaderInfoLog(s);

  GLint param;
  glGetShaderiv(s, GL_COMPILE_STATUS, &param);
  if (param != GL_TRUE)
  {
    cerr << "Failed to compile shader source\n====" << endl;
    cerr << shader << endl;
    cerr << "===" << endl;
    glDeleteShader(s);

    return 0;
  }

  if (checkOpenGLError(__FILE__, __LINE__, NULL))
  {
    cerr << "Failed to compile shader source\n====" << endl;
    cerr << shader << endl;
    cerr << "===" << endl;
    glDeleteShader(s);

    return 0;
  }

  return s;
}

GLuint OpenGLUtils::CreateTexture()
{
  GLuint texture = 0;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

GLuint OpenGLUtils::CreateTexture(Pixmap *pixmap)
{
  BufferPtr buffer = pixmap->buffer();
  CONSTANTS::PixelFormat format = pixmap->format();

  GLuint texture = 0;
  GLenum texture_format = ParseFormat(format);
  GLuint texture_bpp = ParseBPP(format);

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, texture_bpp, pixmap->width(), pixmap->height(), 0, texture_format, GL_UNSIGNED_BYTE, buffer->raw());
  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

GLuint OpenGLUtils::CreateShaderProgram(const std::string &vertexShader, const std::string &fragmentShader)
{
  std::cout << "Creating shader" << std::endl;
  std::cout << "==Vertex Shader==" << std::endl << vertexShader << std::endl;
  std::cout << "==Fragment Shader==" << std::endl << fragmentShader << std::endl;

  GLuint program = glCreateProgram();
  if (program == 0 || checkOpenGLError(__FILE__, __LINE__, "Failed to create program"))
    return 0;

  GLuint vs = Compile(GL_VERTEX_SHADER, vertexShader);
  GLuint fs = Compile(GL_FRAGMENT_SHADER, fragmentShader);

  if (vs && fs)
  {
    glBindAttribLocation(program, OpenGLShaderProgram::POSITION, "aPosition");
    glBindAttribLocation(program, OpenGLShaderProgram::COLOR,    "aColor");
    glBindAttribLocation(program, OpenGLShaderProgram::UV,       "aUV");

    glAttachShader(program, vs);
    if (checkOpenGLError(__FILE__, __LINE__, "Failed to attach vertex shader source"))
    {
      glDeleteShader(vs);
      glDeleteShader(fs);
      glDeleteProgram(program);
      return 0;
    }

    glAttachShader(program, fs);
    if (checkOpenGLError(__FILE__, __LINE__, "Failed to attach fragment shader source"))
    {
      glDetachShader(program, vs);
      glDeleteShader(vs);

      glDeleteShader(fs);

      glDeleteProgram(program);
      return 0;
    }

    glLinkProgram(program);
    if (checkOpenGLError(__FILE__, __LINE__, "Failed to link shader program"))
    {
      glDetachShader(program, vs);
      glDeleteShader(vs);

      glDetachShader(program, fs);
      glDeleteShader(fs);

      glDeleteProgram(program);
      return 0;
    }

    GLint param;
    glGetProgramiv(program, GL_LINK_STATUS, &param);
    if (param != GL_TRUE)
    {
      cerr << "Failed to link shader program" << endl;
      glDetachShader(program, vs);
      glDeleteShader(vs);

      glDetachShader(program, fs);
      glDeleteShader(fs);

      glDeleteProgram(program);
      return 0;
    }

    printProgramInfoLog(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    glUseProgram(program);

    return program;
  }
  cerr << "Failed to create program" << endl;

  if (vs)
    glDeleteShader(vs);

  if (fs)
    glDeleteShader(fs);

  checkOpenGLError(__FILE__, __LINE__, NULL);
  return 0;
}

void OpenGLUtils::_check_gl_error(const char *file, int line) {
        GLenum err (glGetError());
 
        while(err!=GL_NO_ERROR) {
                string error;
 
                switch(err) {
                        case GL_INVALID_OPERATION:              error="INVALID_OPERATION";              break;
                        case GL_INVALID_ENUM:                   error="INVALID_ENUM";                   break;
                        case GL_INVALID_VALUE:                  error="INVALID_VALUE";                  break;
                        case GL_OUT_OF_MEMORY:                  error="OUT_OF_MEMORY";                  break;
                        case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
                        default:                                error="UNKNOWN ERROR";                  break;
                }
 
                cerr << "GL_" << error.c_str() <<" - "<<file<<":"<<line<<endl;
                err=glGetError();
        }
}
