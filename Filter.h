#pragma once

#include "constants.h"
#include "Object.h"

namespace dawn
{
  class Filter : public Object
  {
  public:
    virtual ~Filter() { }

    virtual bool isChanged(etag_t *etag, bool recursive) {
      return Object::isChanged(etag, recursive);
    }

    virtual CONSTANTS::FilterType type() const = 0;
  };

  typedef std::vector<Filter *> FilterList;
}
