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
    void geometry(Geometry *g) { markDirty(m_geometry != g); m_geometry = g; }

    Material *material() const { return m_material; }
    void material(Material *m) { markDirty(m_material != m); m_material = m; }

    virtual bool isDirty(bool recursive = false) const {
        if (Object3D::isDirty(recursive)) {
            return true;
        } else if (recursive) {
            return m_geometry->isDirty(recursive) || m_material->isDirty(recursive);
        }

        return false;
    }

    virtual void clean() {
        Object3D::clean();

        m_geometry->clean();
        m_material->clean();
    }

  protected:
    Geometry *m_geometry;
    Material *m_material;
  };
}
