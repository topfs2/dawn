#pragma once

#include "constants.h"
#include "Object.h"

namespace dawn
{
  class Path : public Object
  {
  public:
    virtual ~Path() { }

    virtual bool isChanged(etag_t *etag, bool recursive) {
      return Object::isChanged(etag, recursive);
    }

    virtual CONSTANTS::PathType type() const = 0;
  };
}
