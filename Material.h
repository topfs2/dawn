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

    virtual bool isChanged(etag_t *etag, bool recursive) {
      return Object::isChanged(etag, recursive);
    }

    virtual CONSTANTS::MaterialType type() const = 0;
  };
}
