#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace feng
{
    class ShaderParams final
    {
    public:
        static constexpr std::string_view ModelMatrix {"uModelMatrix"};
        static constexpr std::string_view ViewProjMatrix {"uViewProjMatrix"};
        static constexpr std::string_view Time {"uTime"};
        static constexpr std::string_view CameraPos {"uCamPos"};
        static constexpr std::string_view CameraDir {"uCamDir"};
        static constexpr std::string_view MainColor {"uMainColor"};
        static constexpr std::string_view Texture0 {"uTexture0"};
        static constexpr std::string_view Texture1 {"uTexture1"};
        static constexpr std::string_view Texture2 {"uTexture2"};
        static constexpr std::string_view FogStart {"uFogStart"};
        static constexpr std::string_view FogRange{"uFogRange"};
        static constexpr std::string_view FogDensity {"uFogDensity"};
        static constexpr std::string_view FogColor {"uFogColor"};
        static constexpr std::string_view ScreenColorOriginal {"uScreenColorOriginal"};
        static constexpr std::string_view ScreenDepthOriginal {"uScreenDepthOriginal"};
        static constexpr std::string_view ScreenColorProcessed {"uScreenColorProcessed"};

        static constexpr std::string_view DirLightDir {"uDirectLightDirection"};
        static constexpr std::string_view DirLightColor {"uDirLightColor"};

        static constexpr std::string_view VertexPos {"aPos"};
        static constexpr std::string_view VertexUv0 {"aUv0"};
        static constexpr std::string_view VertexNorm {"aNorm"};
        static constexpr std::string_view VertexBinorm {"aBinorm"};
        static constexpr std::string_view VertexTangent {"aTangent"};

//        const std::array<std::string_view, ReservedAttributesCount> ReservedAttributes;
//        const std::array<std::string_view, ReservedUniformsCount> ReservedUniforms;
    };
}
