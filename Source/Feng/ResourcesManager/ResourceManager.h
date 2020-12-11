#pragma once

#include <Feng/App/Object.h>
#include <Feng/ResourcesManager/CachedResource.h>

#include <list>

namespace feng
{
    template <class T, class TCachedResource>
    class ResourceManager
    {
    public:
        virtual ~ResourceManager();
        unsigned int GetInstancesCount() const;
        T * GetInstance(obj_id id);
        void Purge();
        bool DestroyResource(obj_id id);

    protected:
        ResourceManager(TCachedResource *cachedResources, unsigned cachedResourcesCount);
        virtual T * Instantiate(TCachedResource cachedResource) = 0;

    private:
        bool TryGetCachedResource(obj_id id, TCachedResource& cachedResource) const;

        std::list<T> m_resources;
        TCachedResource *m_cachedResources;
        unsigned m_cachedResourcesCount;
    };
}

template <class T, class TCachedResource>
feng::ResourceManager<T, TCachedResource>::ResourceManager(TCachedResource *cachedResources, unsigned cachedResourcesCount) :
    m_cachedResources{cachedResources},
    m_cachedResourcesCount{cachedResourcesCount}
{
}

template <class T, class TCachedResource>
feng::ResourceManager<T, TCachedResource>::~ResourceManager()
{
	Purge();

	delete[] m_cachedResources;
}

template <class T, class TCachedResource>
inline unsigned int feng::ResourceManager<T, TCachedResource>::GetInstancesCount() const
{
	return m_resources.GetSize();
}

template <class T, class TCachedResource>
T * feng::ResourceManager<T, TCachedResource>::GetInstance(feng::obj_id id)
{
	auto iterator = m_resources.GetIterator();
	do
	{
		T *resource = iterator.GetCurrent();
		if (resource)
		{
			feng::Object *obj = dynamic_cast<feng::Object *>(resource);
			if (obj->GetId() == id)
			{
				return resource;
			}
		}

	} while (iterator.MoveNext());

	TCachedResource cachedResource;
	if (TryGetCachedResource(id, cachedResource))
	{
		T *instance = Instantiate(cachedResource);
		m_resources.Add(instance);
		return instance;
	}

	return nullptr;
}

template <class T, class TCachedResource>
void feng::ResourceManager<T, TCachedResource>::Purge()
{
	auto iterator = m_resources.GetIterator();
	do
	{
		T *resource = iterator.GetCurrent();
		if (resource)
		{
			delete resource;
		}

	} while (iterator.MoveNext());

	m_resources.Clear();
}

template <class T, class TCachedResource>
bool feng::ResourceManager<T, TCachedResource>::DestroyResource(feng::obj_id id)
{
	auto iterator = m_resources.GetIterator();
	T *target = nullptr;
	do
	{
		T *resource = iterator.GetCurrent();
		if (resource)
		{
			if (resource->GetId() == id)
			{
				target = resource;
				break;
			}
		}

	} while (iterator.MoveNext());

	if (target)
	{
		m_resources.Remove(target);
		delete target;
		target = nullptr;
		return true;
	}

	return false;
}

template <class T, class TCachedResource>
bool feng::ResourceManager<T, TCachedResource>::TryGetCachedResource(feng::obj_id id, TCachedResource& resource) const
{
	for (unsigned i = 0; i < m_cachedResourcesCount; ++i)
	{
		TCachedResource& cachedResource = m_cachedResources[i];
		if (cachedResource.Id == id)
		{
			resource = cachedResource;
			return true;
		}
	}

	return false;
}
