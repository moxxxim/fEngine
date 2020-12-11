#pragma once

#include <Feng/ResourcesManager/CachedResource.h>
#include <Feng/ResourcesManager/ShaderStates.h>

#include <string>

namespace feng
{
    class CachedShader : public CachedResource
    {
    public:
        CachedShader() = default;
        CachedShader(const CachedShader& other) = default;
        CachedShader(CachedShader&& other) = default;

        CachedShader& operator = (const CachedShader& other) = default;
        CachedShader& operator = (CachedShader&& other) = default;

        ShaderStates States = ShaderStates::None;
        std::string Fs;
        std::string Vs;
    };
}
