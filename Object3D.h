#pragma once
#include <vector>
#include <boost/shared_ptr.hpp>

#include "constants.h"
#include "Object.h"
#include "types.h"

namespace dawn
{
  enum FilterType
  {
    FILTER_INVALID = 0,
    FILTER_GRAYSCALE,
    FILTER_GAUSIAN_BLUR
  };

  class Filter
  {
  public:
    Filter() : type(FILTER_INVALID) { }
    Filter(FilterType type, UniformMap uniforms) : type(type), uniforms(uniforms) { }

    FilterType type;
    UniformMap uniforms;
  };

  typedef std::vector<Filter> FilterList;


  class Object3D : public Object
  {
  public:
    Object3D() : m_transform(mat4f::Identity()), m_visible(true) { }
    virtual ~Object3D() {
      for (unsigned int i = 0; i < m_children.size(); i++) {
        delete m_children[0]; // TODO Use shared_ptr?
      }
    }

    virtual CONSTANTS::Type type() const { return CONSTANTS::Object3D; }

    mat4f transform() const { return m_transform; }
    void transform(mat4f transform) { m_transform = transform; markDirty(); }

    bool visible() const { return m_visible; }
    void visible(bool visible) { m_visible = visible; markDirty(); }

    FilterList filters() const { return m_filters; }
    void filters(Filter filter) { m_filters.clear(); m_filters.push_back(filter); markDirty(); } // Helper
    void filters(FilterList filters) { m_filters = filters; markDirty(); }

    typedef std::vector<Object3D *> Object3DList; // TODO Move to types.h?
    typedef Object3DList::iterator iterator;
    typedef Object3DList::const_iterator const_iterator;

    const Object3DList children() const { return m_children; }
    iterator begin() { return m_children.begin(); }
    const_iterator begin() const { return m_children.begin(); }
    iterator end() { return m_children.end(); }
    const_iterator end() const { return m_children.end(); }
    void children(Object3DList children) { m_children = children; markDirty(); }

    void appendChild(Object3D *c) {
        if (c) {
            m_children.push_back(c);
            markDirty();
        }
    }

    void removeChild(Object3D *c) {
        if (c) {
            std::remove(m_children.begin(), m_children.end(), c);
            markDirty();
        }
    }

    void replaceChild(Object3D *a, Object3D *b) {
        // TODO Should we replace if a exist
        if (a && b) {
            std::replace(m_children.begin(), m_children.end(), a, b);
            markDirty();
        }
    }

    virtual bool isDirty(bool recursive = false) const {
        if (Object::isDirty(recursive)) {
            return true;
        } else if (recursive) {
            for (const_iterator itr = m_children.begin(); itr != m_children.end(); itr++) {
              if ((*itr)->isDirty(recursive)) {
                  return true;
              }
            }
        }

        return false;
    }

    virtual void clean() {
        Object::clean();

        for (iterator itr = m_children.begin(); itr != m_children.end(); itr++) {
            (*itr)->clean();
        }
    }

  protected:
    mat4f m_transform;
    bool m_visible;
    FilterList m_filters;

    Object3DList m_children;
  };

  typedef boost::shared_ptr<Object3D> Object3DPtr;
}
