#pragma once
#include "Geometry.h"

namespace dawn
{
    class PolygonGeometry : public Geometry
    {
    public:
        PolygonGeometry(vec2farray vertices, vec4f uv) : m_vertices(vertices), m_uv(uv) { }

        CONSTANTS::GeometryType type() const { return CONSTANTS::PolygonGeometry; }

        vec2farray vertices() const { return m_vertices; }
        void vertices(vec2farray vertices) { setChanged(); m_vertices = vertices; }

        vec4f uv() const { return m_uv; }
        void uv(vec4f uv) { setChanged(uv != m_uv); m_uv = uv; }

    protected:
        vec2farray m_vertices;
        vec4f m_uv;
    };
}
