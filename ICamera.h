#pragma once
#include <boost/shared_ptr.hpp>

namespace dawn
{
  class ICamera
  {
  public:
    virtual ~ICamera() { }

    virtual CONSTANTS::CameraType type() const = 0;

    virtual mat4f projection() const = 0;
    virtual mat4f view() { return mat4f::Identity(); }
  };
}
