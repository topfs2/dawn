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

    virtual bool isChanged(etag_t *etag, bool recursive) {
      return Object::isChanged(etag, recursive);
    }

    virtual CONSTANTS::GeometryType type() const = 0;
  };
}
