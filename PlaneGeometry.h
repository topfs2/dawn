#pragma once
#include "Geometry.h"

namespace dawn
{
  class PlaneGeometry : public Geometry
  {
  public:
    PlaneGeometry(float width, float height) : m_width(width), m_height(height) { }

    CONSTANTS::GeometryType type() const { return CONSTANTS::PlaneGeometry; }

    float width() const { return m_width; }
    void width(float width) { markDirty(m_width != width); m_width = width; }

    float height() const { return m_height; }
    void height(float height) { markDirty(m_height != height); m_height = height; }

    void size(float width, float height) { markDirty(m_height != height || m_width != width); m_width = width; m_height = height; }

  protected:
    float m_width;
    float m_height;
  };
}
