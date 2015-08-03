#pragma once
#include "Geometry.h"
#include "Path.h"

namespace dawn
{
    class FillPathGeometry : public Geometry
    {
    public:
        FillPathGeometry(Path *path, vec4f uv) : m_path(path), m_uv(uv) { }

        CONSTANTS::GeometryType type() const { return CONSTANTS::FillPathGeometry; }

        Path *path() const { return m_path; }
        void path(Path *path) { setChanged(m_path != path); m_path = path; }

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
        vec4f m_uv;
    };
}
