#pragma once
#include "Object3D.h"
#include "ICamera.h"

namespace dawn
{
  class IRenderer
  {
  public:
    virtual ~IRenderer() { }

    virtual void render(ICamera *camera, Object3D *scene) = 0;
  };
}
