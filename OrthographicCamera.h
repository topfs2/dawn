#pragma once

#include "ICamera.h"
#include "LinearAlgebra.h"

namespace dawn
{
  class OrthographicCamera : public ICamera
  {
  public:
    OrthographicCamera(float width, float height, float near, float far) { projection(width / -2.0f, width / 2.0f, height / 2.0f, height / -2.0f, near, far); }
    OrthographicCamera(float left, float right, float top, float bottom, float near, float far) { projection(left, right, top, bottom, near, far); }

    CONSTANTS::CameraType type() const { return CONSTANTS::OrthographicCamera; }

    virtual mat4f projection() const { return m_projection; }
    mat4f projection(float left, float right, float top, float bottom, float near, float far) { m_projection = ortho(left, right, top, bottom, near, far); }

  private:
    mat4f m_projection;
  };
}
