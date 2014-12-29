#include "OpenGLShaderProgramPool.h"
#include "OpenGLUtils.h"

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;
using namespace dawn;
using namespace dawn::uniforms;

static string readFile(const string &path)
{
  ifstream t(path);
  return string((istreambuf_iterator<char>(t)),
                 istreambuf_iterator<char>());
}

OpenGLShaderProgramPool::OpenGLShaderProgramPool() { }

OpenGLShaderProgramPool::~OpenGLShaderProgramPool() { }

string OpenGLShaderProgramPool::id(string key)
{
  return key;
}

OpenGLShaderProgramPtr OpenGLShaderProgramPool::LoadResource(string path)
{
  cout << "Loading shader in " << path << endl;
  string vs = readFile(path + ".vert.glsl");
  string fs = readFile(path + ".frag.glsl");

  GLuint program = OpenGLUtils::CreateShaderProgram(vs, fs);

  return OpenGLShaderProgramPtr(new OpenGLShaderProgram(program));
}
