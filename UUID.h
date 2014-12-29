#pragma once

#include "types.h"
#include <vector>

namespace dawn
{
  // TODO Move into Object?
  class UUID
  {
  public:
    static uuid_t Acquire();
    static void Release(const uuid_t &uuid);

  private:
    static uuid_t currentUUID;
    static std::vector<uuid_t> freeUUIDs;
  };
}
