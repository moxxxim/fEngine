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
        static constexpr std::string_view CameraPos {"uCamPos"};
        static constexpr std::string_view CameraDir {"uCamDir"};
        static constexpr std::string_view MainColor {"uMainColor"};
        static constexpr std::string_view Texture0 {"uTexture0"};
        static constexpr std::string_view Texture1 {"uTexture1"};
        static constexpr std::string_view Texture2 {"uTexture2"};

        static constexpr std::string_view AmbientColor {"uAmbientColor"};
        static constexpr std::string_view PointLightColor {"uPointLightColor"};
        static constexpr std::string_view PointLightPositionAndRange {"uPointLightPositionAndRange"};

        static constexpr std::string_view VertexPos {"aPos"};
        static constexpr std::string_view VertexUv0 {"aUv0"};
        static constexpr std::string_view VertexNorm {"aNorm"};
        static constexpr std::string_view VertexBinorm {"aBinorm"};
        static constexpr std::string_view VertexTangent {"aTangent"};
    };
}