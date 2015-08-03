#pragma once
#include "Geometry.h"

namespace dawn
{
    class RawGeometry : public Geometry
    {
    public:
        RawGeometry(vec3farray position, vec4farray color, vec2farray uv, iarray indices) : m_position(position), m_color(color), m_uv(uv), m_indices(indices) { }

        CONSTANTS::GeometryType type() const { return CONSTANTS::RawGeometry; }

        vec3farray position() const { return m_position; }
        void position(vec3farray position) { setChanged(); m_position = position; }

        vec4farray color() const { return m_color; }
        void color(vec4farray color) { setChanged(); m_color = color; }

        vec2farray uv() const { return m_uv; }
        void uv(vec2farray uv) { setChanged(); m_uv = uv; }

        iarray indices() const { return m_indices; }
        void indices(iarray indices) { setChanged(); m_indices = indices; }

    protected:
        vec3farray m_position;
        vec4farray m_color;
        vec2farray m_uv;

        iarray m_indices;
    };
}
