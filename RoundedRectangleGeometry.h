#pragma once
#include "PlaneGeometry.h"

namespace dawn
{
    class RoundedRectangleGeometry : public PlaneGeometry
    {
    public:
        RoundedRectangleGeometry(float width, float height, vec4f radius, vec4f uv) : PlaneGeometry(width, height, uv), m_radius(radius) { }

        CONSTANTS::GeometryType type() const { return CONSTANTS::RoundedRectangleGeometry; }

        vec4f radius() const { return m_radius; }
        void radius(vec4f radius) { setChanged(m_radius != radius); m_radius = radius; }

    protected:
        vec4f m_radius;
    };
}
