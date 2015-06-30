#pragma once
#include "Object3D.h"
#include "Camera.h"

namespace dawn
{
  class IRenderer
  {
  public:
    virtual ~IRenderer() { }

    virtual void render(Camera *camera, Object3D *scene) = 0;
  };
}
