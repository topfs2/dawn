#pragma once
#include "Geometry.h"

namespace dawn
{
  class EllipsisGeometry : public Geometry
  {
  public:
    EllipsisGeometry(float width, float height, unsigned int segments) : m_width(width), m_height(height), m_segments(segments) { }

    CONSTANTS::GeometryType type() const { return CONSTANTS::EllipsisGeometry; }

    float width() const { return m_width; }
    void width(float width) { markDirty(m_width != width); m_width = width; }

    float height() const { return m_height; }
    void height(float height) { markDirty(m_height != height); m_height = height; }

    unsigned int segments() const { return m_segments; }
    void segments(unsigned int segments) { markDirty(m_segments != segments); m_segments = segments; }

  protected:
    float m_width;
    float m_height;
    unsigned int m_segments;
  };
}
