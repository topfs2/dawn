#pragma once
#include "IGeometry.h"

namespace dawn
{
  class PlaneGeometry : public IGeometry
  {
  public:
    PlaneGeometry(float width, float height) : m_width(width), m_height(height) { }

    CONSTANTS::GeometryType type() const { return CONSTANTS::PlaneGeometry; }

    float width() { return m_width; }
    void width(float width) { m_width = width; changed(); }

    float height() { return m_height; }
    void height(float height) { m_height = height; changed(); }

    void size(float width, float height) { m_width = width; m_height = height; changed(); }

  protected:
    float m_width;
    float m_height;
  };
}
