#pragma once
#include "PlaneGeometry.h"

namespace dawn
{
    class BorderedRectangleGeometry : public PlaneGeometry
    {
    public:
        BorderedRectangleGeometry(float width, float height, vec4f border, vec4f uv) : PlaneGeometry(width, height, uv), m_border(border) { }

        CONSTANTS::GeometryType type() const { return CONSTANTS::BorderedRectangleGeometry; }

        vec4f border() const { return m_border; }
        void border(vec4f border) { setChanged(m_border != border); m_border = border; }

    protected:
        vec4f m_border;
    };
}
