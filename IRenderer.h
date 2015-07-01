#pragma once
#include "Scene3D.h"

namespace dawn
{
  class IRenderer
  {
  public:
    virtual ~IRenderer() { }

    virtual void render(Scene3D *scene) = 0;
    virtual void render(Camera *camera, Object3D *stage, unsigned int width, unsigned int height) = 0;
  };
}
