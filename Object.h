#pragma once
#include <cassert>
#include "to_string.h" // Remove on c++11
#include "types.h"

namespace dawn
{
    class Object
    {
    public:
        Object() : m_id(to_string(this)), m_dirty(true) { }
        Object(std::string id) : m_id(id), m_dirty(true) { }

        virtual bool isDirty(bool recursive = false) const { return m_dirty; }
        void markDirty(bool dirty = true) { m_dirty = dirty; }

        // TODO combine with markDirty?
        virtual void clean() { markDirty(false); }

        std::string id() const { return m_id; }

    private:
        std::string m_id;
        bool m_dirty;
    };
}
