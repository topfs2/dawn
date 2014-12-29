#include "UUID.h"

using namespace dawn;

uuid_t UUID::currentUUID = uuid_t(1, 0);
std::vector<uuid_t> UUID::freeUUIDs;

uuid_t UUID::Acquire()
{
  if (freeUUIDs.size()) {
    uuid_t uuid = freeUUIDs.front();
    freeUUIDs.erase(freeUUIDs.begin());

    return uuid_t(uuid.index(), uuid.version() + 1);
  }

  uuid_t i = currentUUID;
  currentUUID = uuid_t(currentUUID.index() + 1, 0);
  return i;
}

void UUID::Release(const uuid_t &uuid)
{
  // 0 is considered invalid
  if (uuid.index() > 0) {
    freeUUIDs.push_back(uuid);
  }
}
