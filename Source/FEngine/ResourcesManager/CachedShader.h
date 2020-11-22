#pragma once

#include <FEngine/ResourcesManager/CachedResource.h>
#include <FEngine/ResourcesManager/ShaderStates.h>

namespace fengine
{
    class CachedShader : public CachedResource
    {
    public:
        CachedShader() = default;
        CachedShader(const CachedShader& other);
        CachedShader(CachedShader&& other);
        inline ~CachedShader();

        CachedShader& operator = (const CachedShader& other);
        CachedShader& operator = (CachedShader&& other);

        ShaderStates States = ShaderStates::None;
        char *Fs = nullptr;
        char *Vs = nullptr;
    };

    inline CachedShader::~CachedShader()
    {
        delete[] Fs;
        delete[] Vs;
    }
}
