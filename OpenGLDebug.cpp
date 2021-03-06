#include <GL/glew.h>
#include <iostream>
#include "OpenGLDebug.h"
#include "config.h"
#include "ImageUtils.h"
#include "OpenGLTexture.h"

#include "dawn.h"

using namespace std;
using namespace dawn;
using boost::any_cast;

int order = 0;

void OpenGLDebug::WriteFBO(const std::string &title)
{
#ifdef DEBUG_WRITE_FBO
  GLubyte data[WINDOW_WIDTH * WINDOW_HEIGHT * 4];
  glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, data);

  ImageUtils::WriteImage(title + ".png", WINDOW_WIDTH, WINDOW_HEIGHT, data);
#endif
}

void OpenGLDebug::WriteFilterInput(Object3D *object)
{
  std::stringstream ss;
  ss << order++ << "_object_" << object->id() << "_input";

  WriteFBO(ss.str());
}

void OpenGLDebug::WriteFilterPass(Object3D *object, unsigned int pass)
{
  std::stringstream ss;
  ss << order++ << "_object_" << object->id() << "_pass_" << pass;

  WriteFBO(ss.str());
}

void OpenGLDebug::WriteFilterOutput(Object3D *object)
{
  std::stringstream ss;
  ss << order++ << "_object_" << object->id() << "_final";

  WriteFBO(ss.str());
}

int orderStencil = 0;

void OpenGLDebug::WriteStencil(const std::string &title)
{
#ifdef DEBUG_WRITE_FBO
  std::stringstream ss;
  ss << orderStencil++ << "_stencil_" << title << ".png";

  size_t s = WINDOW_WIDTH * WINDOW_HEIGHT;

  GLubyte stencildata[s];
  glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, stencildata);

  GLubyte data[s * 4];
  int j = 0;
  for (int i = 0; i < s; i++) {
    data[j++] = stencildata[i];
    data[j++] = stencildata[i];
    data[j++] = stencildata[i];
    data[j++] = stencildata[i];
  }

  ImageUtils::WriteImage(ss.str(), WINDOW_WIDTH, WINDOW_HEIGHT, data);
#endif
}

void OpenGLDebug::PrintUniformMap(UniformMap uniforms)
{
  cout << "Uniforms = {" << endl;

  GLint textureUnit = 0;
  for (UniformMap::iterator itr = uniforms.begin(); itr != uniforms.end(); itr++) {
    uniform_t u = itr->second;

    if (u.type() == typeid(int)) {
      cout << "Binding " << itr->first << " as int " << any_cast<int>(u) << endl;
    } else if (u.type() == typeid(float)) {
      cout << "Binding " << itr->first << " as float " << any_cast<float>(u) << endl;
    } else if (u.type() == typeid(vec2f)) {
      cout << "Binding " << itr->first << " as vec2f " << any_cast<vec2f>(u) << endl;
    } else if (u.type() == typeid(vec3f)) {
      cout << "Binding " << itr->first << " as vec3f " << any_cast<vec3f>(u) << endl;
    } else if (u.type() == typeid(vec4f)) {
      cout << "Binding " << itr->first << " as vec4f " << any_cast<vec4f>(u) << endl;
    } else if (u.type() == typeid(mat4f)) {
//      cout << "Binding " << itr->first << " as mat4f " << any_cast<mat4f>(u) << endl;
    } else if (u.type() == typeid(Pixmap *)) {
      cout << "Binding " << itr->first << " as Pixmap " << endl;
    } else if (u.type() == typeid(OpenGLTexturePtr)) {
      cout << "Binding " << itr->first << " as OpenGLTexturePtr" << endl;
    }
  }

  cout << "}" << endl;
}
