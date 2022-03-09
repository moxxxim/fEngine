#pragma once

#include <OpenGL/gl.h>
#include <cstdint>

namespace Feng
{
    class Texture;

    enum class eTextureWrapping : uint8_t
    {
        Repeat = 0,
        RepeatMirrored = 1,
        ClampToEdge = 2,
        ClampToBorder = 3,
    };

    enum class eTextureType : uint8_t
    {
        Tex2d = 0,
        Cubemap = 1,
    };

    enum class eTextureMinFilter : uint8_t
    {
        Nearest = 0,
        Linear = 1,
        NearestMipNearest = 2,
        LinearMipNearest = 3,
        NearestMipLinear = 4,
        LinearMipLinear = 5
    };

    enum class eTextureMagFilter : uint8_t
    {
        Nearest = 0,
        Linear = 1
    };

    enum class eCubemapFace : uint8_t
    {
        XPos = 0,
        XNeg = 1,
        YPos = 2,
        YNeg = 3,
        ZPos = 4,
        ZNeg = 5
    };

    GLenum ToOpenGLValue(eTextureType type);
    GLint ToOpenGLValue(eTextureWrapping wrapping);
    GLint ToOpenGLValue(eTextureMinFilter minFilter);
    GLint ToOpenGLValue(eTextureMagFilter magFilter);
    GLenum GetTextureFormat(const Texture& texture);
}
