#pragma once
#include "Material.h"
#include "Filter.h"
#include "Pixmap.h"
#include "Scene3D.h"
#include "types.h"

namespace dawn
{
    class FilterMaterial : public Material
    {
    public:
        FilterMaterial(Pixmap *pixmap, FilterList filters) : m_pixmap(pixmap), m_filters(filters) { }

        CONSTANTS::MaterialType type() const { return CONSTANTS::FilterMaterial; }

        Pixmap *pixmap() const { return m_pixmap; }
        void pixmap(Pixmap *pixmap) { setChanged(m_pixmap != pixmap); m_pixmap = pixmap; }

        FilterList filters() const { return m_filters; }
        void filters(FilterList filters) { setChanged(); m_filters = filters; }

        virtual bool isChanged(etag_t *etag, bool recursive) {
            bool changed = Material::isChanged(etag, recursive);

            if (recursive) {
                for (FilterList::const_iterator itr = m_filters.begin(); itr != m_filters.end(); itr++) {
                    changed |= (*itr)->isChanged(etag, recursive);
                }
            }

            return changed;
        }

    private:
        Pixmap *m_pixmap;
        FilterList m_filters;
    };
}
