#pragma once
#include "Element.h"

namespace dawn
{
  class IRenderer
  {
  public:
    virtual ~IRenderer() { }

    virtual void render(const Element &scene) = 0;
  };
}
