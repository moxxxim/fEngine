#pragma once

namespace feng
{
    typedef unsigned long int obj_id;

    class CachedResource
    {
    public:
        CachedResource() = default;
        virtual ~CachedResource() = default;

        obj_id Id = 0;
    };
}
