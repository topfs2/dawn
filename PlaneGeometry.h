#pragma once
#include "Geometry.h"

namespace dawn
{
    class PlaneGeometry : public Geometry
    {
    public:
        PlaneGeometry(float width, float height, vec4f uv) : m_width(width), m_height(height), m_uv(uv) { }

        CONSTANTS::GeometryType type() const { return CONSTANTS::PlaneGeometry; }

        float width() const { return m_width; }
        void width(float width) { setChanged(m_width != width); m_width = width; }

        float height() const { return m_height; }
        void height(float height) { setChanged(m_height != height); m_height = height; }

        void size(float width, float height) { setChanged(m_height != height || m_width != width); m_width = width; m_height = height; }

        vec4f uv() const { return m_uv; }
        void uv(vec4f uv) { setChanged(uv != m_uv); m_uv = uv; }

    protected:
        float m_width;
        float m_height;
        vec4f m_uv;
    };
}
