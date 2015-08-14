#pragma once
#include "Object3D.h"
#include "Geometry.h"

namespace dawn
{
    class Mask3D : public Object3D
    {
    public:
        Mask3D(Geometry *geometry) : m_geometry(geometry) { }
        virtual CONSTANTS::Type type() const { return CONSTANTS::Mask3D; }

        Geometry *geometry() const { return m_geometry; }
        void geometry(Geometry *g) { setChanged(m_geometry != g); m_geometry = g; }

        virtual bool isChanged(etag_t *etag, bool recursive) {
            bool changed = Object3D::isChanged(etag, recursive);

            if (recursive) {
                changed |= m_geometry->isChanged(etag, recursive);
            }

            return changed;
        }

    protected:
        Geometry *m_geometry;
    };
}
