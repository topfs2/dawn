#pragma once
#include <boost/shared_ptr.hpp>

#include "constants.h"
#include "Object.h"

namespace dawn
{
  class Material : public Object
  {
  public:
    virtual ~Material() { }

    virtual CONSTANTS::MaterialType type() const = 0;
  };
}
