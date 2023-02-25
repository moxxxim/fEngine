#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace Feng
{
    class ShaderParams final
    {
    public:
        static constexpr std::string_view CamUniformBlock {"ubCamera"};
        static constexpr std::string_view ViewProjMatrix {"uViewProjMatrix"};
        static constexpr std::string_view ProjMatrix {"uProjMatrix"};
        static constexpr std::string_view CameraPos {"uCamPos"};
        static constexpr std::string_view CameraDir {"uCamDir"};
        static constexpr std::string_view CameraRotation {"uCamRotation"};

        static constexpr std::string_view ModelMatrix {"uModelMatrix"};
        static constexpr std::string_view MainColor {"uMainColor"};
        static constexpr std::string_view Texture0 {"uTexture0"};
        static constexpr std::string_view Texture1 {"uTexture1"};
        static constexpr std::string_view Texture2 {"uTexture2"};

        static constexpr std::string_view AmbientColor {"uAmbientColor"};
        static constexpr std::string_view PointLightColor {"uPointLight.Color"};
        static constexpr std::string_view PointLightPositionAndRange {"uPointLight.PositionAndRange"};

        static constexpr std::string_view DirectLightColor {"uDirectLight.Color"};
        static constexpr std::string_view DirectLightDir {"uDirectLight.Dir"};

        static constexpr std::string_view SpotLightColor {"uSpotLight.Color"};
        static constexpr std::string_view SpotLightPositionAndRange {"uSpotLight.PositionAndRange"};
        static constexpr std::string_view SpotLightDirAndAngle {"uSpotLight.DirAndAngle"};
        static constexpr std::string_view NearClipPlane {"uNearClipPlane"};
        static constexpr std::string_view FarClipPlane {"uFarClipPlane"};

        static constexpr std::string_view ScreenColorOriginal {"uScreenColorOriginal"};
        static constexpr std::string_view ScreenDepthOriginal {"uScreenDepthOriginal"};
        static constexpr std::string_view ScreenColorProcessed {"uScreenColorProcessed"};
        static constexpr std::string_view ShadowLightView {"uShadowLightView"};
        static constexpr std::string_view ShadowLightViewProj {"uShadowLightViewProj"};
        static constexpr std::string_view ShadowMap {"uShadowMap"};

        static constexpr std::string_view VertexPos {"aPos"};
        static constexpr std::string_view VertexUv0 {"aUv0"};
        static constexpr std::string_view VertexNorm {"aNorm"};
        static constexpr std::string_view VertexBinorm {"aBinorm"};
        static constexpr std::string_view VertexTangent {"aTangent"};
    };
}
