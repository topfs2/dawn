#pragma once
#include "Object3D.h"
#include "Geometry.h"
#include "Material.h"

namespace dawn
{
    class Mesh3D : public Object3D
    {
    public:
        Mesh3D(Geometry *geometry, Material *material) : m_geometry(geometry), m_material(material) { }
        virtual CONSTANTS::Type type() const { return CONSTANTS::Mesh; }

        Geometry *geometry() const { return m_geometry; }
        void geometry(Geometry *g) { setChanged(m_geometry != g); m_geometry = g; }

        Material *material() const { return m_material; }
        void material(Material *m) { setChanged(m_material != m); m_material = m; }

        virtual bool isChanged(etag_t *etag, bool recursive) {
            bool changed = Object3D::isChanged(etag, recursive);

            if (recursive) {
                changed |= m_geometry->isChanged(etag, recursive);
                changed |= m_material->isChanged(etag, recursive);
            }

            return changed;
        }

    protected:
        Geometry *m_geometry;
        Material *m_material;
    };
}
