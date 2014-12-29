#pragma once
#include "IGeometry.h"

namespace dawn
{
  class CircleGeometry : public IGeometry
  {
  public:
    CircleGeometry(float width, float height, unsigned int segments) : m_width(width), m_height(height), m_segments(segments) { }

    CONSTANTS::GeometryType type() const { return CONSTANTS::CircleGeometry; }

    float width() const { return m_width; }
    void width(float width) { m_width = width; changed(); }

    float height() const { return m_height; }
    void height(float height) { m_height = height; changed(); }

    unsigned int segments() const { return m_segments; }
    void segments(unsigned int segments) { m_segments = segments; changed(); }

  protected:
    float m_width;
    float m_height;
    unsigned int m_segments;
  };
}
