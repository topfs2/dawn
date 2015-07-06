#pragma once
#include "Geometry.h"

namespace dawn
{
    class PolygonGeometry : public Geometry
    {
    public:
        PolygonGeometry(vec2farray vertices) : m_vertices(vertices) { }

        CONSTANTS::GeometryType type() const { return CONSTANTS::PolygonGeometry; }

        vec2farray vertices() const { return m_vertices; }
        void vertices(vec2farray vertices) { setChanged(); m_vertices = vertices; }

    protected:
        vec2farray m_vertices;
    };
}
