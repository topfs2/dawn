#pragma once
#include "UUID.h"
#include <cassert>
#include "to_string.h" // Remove on c++11

namespace dawn
{
  typedef uint32_t etag_t; // TODO Move?

  class Object
  {
  public:
    Object() : uuid(UUID::Acquire()), m_etag(0) { }
    virtual ~Object() { UUID::Release(uuid); }

    virtual std::string id() { return to_string(uuid.uuid()); }

    bool changed(etag_t *etag)
    {
      assert(etag);

      if (*etag != m_etag) {
        *etag = m_etag;
        return true;
      }

      return false;
    }

    const uuid_t uuid;

  protected:
    void changed(etag_t etag) { m_etag = etag; }
    void changed() { m_etag += 1; }

  private:
    etag_t m_etag;
  };
}
