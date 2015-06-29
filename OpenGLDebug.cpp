#include <GL/glew.h>
#include "OpenGLDebug.h"
#include "config.h"
#include "ImageUtils.h"

using namespace std;
using namespace dawn;

int order = 0;

void OpenGLDebug::WriteFBO(const std::string &title)
{
#ifdef DEBUG_WRITE_FBO
  GLubyte data[WINDOW_WIDTH * WINDOW_HEIGHT * 3];
  glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data);

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
