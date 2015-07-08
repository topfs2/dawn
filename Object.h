#pragma once
#include <cassert>
#include "to_string.h" // Remove on c++11
#include "types.h"

namespace dawn
{
    class Object
    {
    public:
        Object() : m_id(to_string(this)), m_etag(now()) { }
        Object(std::string id) : m_id(id), m_etag(now()) { }

        virtual bool isChanged(etag_t *etag, bool recursive) const {
            assert(etag);

            if (*etag < m_etag) {
                *etag = m_etag;
                return true;
            }

            return false;
        }

        etag_t lastChange(bool recursive) const {
            etag_t etag = 0;
            isChanged(&etag, recursive);

            return etag;
        }

        std::string id() const { return m_id; }

    protected:
        void setChanged(etag_t etag) {
            m_etag = etag;
        }

        void setChanged(bool changed = true) {
            if (changed) {
                setChanged(now());
            }
        }

    private:
        // TODO Move elsewhere?
        etag_t now() {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }

        std::string m_id;
        etag_t m_etag;
    };
}
