#pragma once
#include "Object3D.h"
#include "IGeometry.h"
#include "IMaterial.h"

namespace dawn
{
  class Mesh : public Object3D
  {
  public:
    Mesh(IGeometry *geometry, IMaterial *material) : m_geometry(geometry), m_material(material) { }
    virtual CONSTANTS::Type type() const { return CONSTANTS::Mesh; }

    IGeometry *geometry() const { return m_geometry; }
    void geometry(IGeometry *g) { markDirty(m_geometry != g); m_geometry = g; }

    IMaterial *material() const { return m_material; }
    void material(IMaterial *m) { markDirty(m_material != m); m_material = m; }

    virtual bool isDirty(bool recursive = false) const {
        if (Object::isDirty(recursive)) {
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
    IGeometry *m_geometry;
    IMaterial *m_material;
  };

  typedef boost::shared_ptr<Mesh> MeshPtr;
}
