#pragma once
#include "Geometry.h"
#include "Path.h"

namespace dawn
{
    class StrokePathGeometry : public Geometry
    {
    public:
        StrokePathGeometry(Path *path, float strokewidth, vec4f uv) : m_path(path), m_strokewidth(strokewidth), m_uv(uv) { }

        CONSTANTS::GeometryType type() const { return CONSTANTS::StrokePathGeometry; }

        Path *path() const { return m_path; }
        void path(Path *path) { setChanged(m_path != path); m_path = path; }

        float strokewidth() const { return m_strokewidth; }
        void strokewidth(float strokewidth) { setChanged(strokewidth != m_strokewidth); m_strokewidth = strokewidth; }

        vec4f uv() const { return m_uv; }
        void uv(vec4f uv) { setChanged(uv != m_uv); m_uv = uv; }

        virtual bool isChanged(etag_t *etag, bool recursive) {
            bool changed = Geometry::isChanged(etag, recursive);

            if (recursive) {
                changed |= m_path->isChanged(etag, recursive);
            }

            return changed;
        }

    protected:
        Path *m_path;
        float m_strokewidth;
        vec4f m_uv;
    };
}
