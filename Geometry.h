#pragma once
#include <boost/shared_ptr.hpp>

#include "constants.h"
#include "Object.h"

namespace dawn
{
  class Geometry : public Object
  {
  public:
    virtual ~Geometry() { }

    virtual CONSTANTS::GeometryType type() const = 0;
  };
}
