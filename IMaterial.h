#pragma once
#include <boost/shared_ptr.hpp>

#include "constants.h"
#include "Object.h"

namespace dawn
{
  class IMaterial : public Object
  {
  public:
    virtual ~IMaterial() { }

    virtual CONSTANTS::MaterialType type() const = 0;
  };

  typedef boost::shared_ptr<IMaterial> MaterialPtr;
}
