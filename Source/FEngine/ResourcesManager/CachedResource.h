#pragma once

namespace fengine
{
    typedef unsigned long int obj_id;

    class CachedResource
    {
    public:
        CachedResource() = default;
        CachedResource(const CachedResource& other) = default;
        CachedResource(CachedResource&& other) = default;
        virtual ~CachedResource() = default;

        CachedResource& operator = (const CachedResource& other) = default;
        CachedResource& operator = (CachedResource&& other) = default;

        obj_id Id = 0;
    };
}
