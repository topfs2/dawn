#pragma once

#include "ICamera.h"
#include "LinearAlgebra.h"

namespace dawn
{
  class PerspectiveCamera : public ICamera
  {
  public:
    PerspectiveCamera(float fovy, float aspect, float near, float far) { projection(fovy, aspect, near, far); }

    CONSTANTS::CameraType type() const { return CONSTANTS::PerspectiveCamera; }

    virtual mat4f projection() const { return m_projection; }
    mat4f projection(float fovy, float aspect, float near, float far) { m_projection = perspective(fovy, aspect, near, far); markDirty(); }

  private:
    mat4f m_projection;
  };
}
