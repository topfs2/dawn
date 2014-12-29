#pragma once
#include <boost/shared_ptr.hpp>

#include "constants.h"
#include "Object.h"

namespace dawn
{
  class IGeometry : public Object
  {
  public:
    virtual ~IGeometry() { }

    virtual CONSTANTS::GeometryType type() const = 0;
  };

  typedef boost::shared_ptr<IGeometry> GeometryPtr;
}
