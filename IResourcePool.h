#pragma once

#include "config.h"

#include <map>
#include <string>

namespace dawn
{
  template <class K, class V>
  class IResourcePool
  {
  public:
    IResourcePool()
    {
    }

    virtual ~IResourcePool()
    {
	    ReleaseAllResources();
    }

    void ReleaseAllResources()
    {
	    while (m_resources.begin() != m_resources.end()) {
		    m_resources.erase(m_resources.begin());
	    }
    }

    void ReleaseResource(K k)
    {
	    typename CResourceMap::iterator it;
	    it=m_resources.find(id(k));
	    if (it != m_resources.end()) {
		    m_resources.erase(it);
	    }
    }

    V GetResource(K k)
    {
	    typename CResourceMap::iterator it;
	    V resource;
      std::string name = id(k);
	    it=m_resources.find(name);

	    if(it != m_resources.end()){
		    resource = it->second;
	    } else {
		    resource = LoadResource(k);
  #if RESOURCE_STORE_POLICY != RESOURCE_NEVER_STORE
		    if (resource)
			    m_resources[name] = resource;
  #endif
	    }

	    return resource;
    }

  protected:
    virtual std::string id(K k) = 0;
    virtual V LoadResource(K k) = 0;

    typedef std::map<std::string, V> CResourceMap;
    CResourceMap m_resources;
  };
}
