#pragma once
#include "Geometry.h"

namespace dawn
{
    class EllipsisGeometry : public Geometry
    {
    public:
        EllipsisGeometry(float width, float height, unsigned int segments, vec4f uv) : m_width(width), m_height(height), m_segments(segments), m_uv(uv) { }

        CONSTANTS::GeometryType type() const { return CONSTANTS::EllipsisGeometry; }

        float width() const { return m_width; }
        void width(float width) { setChanged(m_width != width); m_width = width; }

        float height() const { return m_height; }
        void height(float height) { setChanged(m_height != height); m_height = height; }

        unsigned int segments() const { return m_segments; }
        void segments(unsigned int segments) { setChanged(m_segments != segments); m_segments = segments; }

        vec4f uv() const { return m_uv; }
        void uv(vec4f uv) { setChanged(uv != m_uv); m_uv = uv; }

    protected:
        float m_width;
        float m_height;
        unsigned int m_segments;
        vec4f m_uv;
    };
}
