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
    void geometry(IGeometry *g) { m_geometry = g; changed(); }

    IMaterial *material() const { return m_material; }
    void material(IMaterial *m) { m_material = m; changed(); }

  protected:
    IGeometry *m_geometry;
    IMaterial *m_material;
  };

  typedef boost::shared_ptr<Mesh> MeshPtr;
}
