#include <Feng/Utils/Render/TextureParams.h>

#include <Feng/ResourcesManager/Texture.h>
#include <Feng/Utils/Debug.h>>
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace Feng
{
    GLenum ToOpenGLValue(eTextureType type)
    {
        switch(type)
        {
            case eTextureType::Tex2d:
                return GL_TEXTURE_2D;

            case eTextureType::Cubemap:
                return GL_TEXTURE_CUBE_MAP;

            case eTextureType::Array2d:
                return GL_TEXTURE_2D_ARRAY;
        }
        
        Debug::LogError("Invalid texture type specified.");
        return 0;
    }

    GLint ToOpenGLValue(eTextureWrapping wrapping)
    {
        switch (wrapping)
        {
            case eTextureWrapping::Repeat:
                return GL_REPEAT;

            case eTextureWrapping::RepeatMirrored:
                return GL_MIRRORED_REPEAT;

            case eTextureWrapping::ClampToEdge:
                return GL_CLAMP_TO_EDGE;

            case eTextureWrapping::ClampToBorder:
                return GL_CLAMP_TO_BORDER;
        }
        
        Debug::LogError("Invalid wrapping specified.");
        return 0;
    }

    GLint ToOpenGLValue(eTextureMinFilter minFilter)
    {
        switch (minFilter)
        {
            case eTextureMinFilter::Nearest:
                return GL_NEAREST;

            case eTextureMinFilter::Linear:
                return GL_LINEAR;

            case eTextureMinFilter::NearestMipNearest:
                return GL_NEAREST_MIPMAP_NEAREST;

            case eTextureMinFilter::NearestMipLinear:
                return GL_NEAREST_MIPMAP_LINEAR;

            case eTextureMinFilter::LinearMipNearest:
                return GL_LINEAR_MIPMAP_NEAREST;

            case eTextureMinFilter::LinearMipLinear:
                return GL_LINEAR_MIPMAP_LINEAR;
        }
        
        Debug::LogError("Invalid minification filter specified.");
        return 0;
    }

    GLint ToOpenGLValue(eTextureMagFilter magFilter)
    {
        switch (magFilter)
        {
            case eTextureMagFilter::Nearest:
                return GL_NEAREST;

            case eTextureMagFilter::Linear:
                return GL_LINEAR;
        }
        
        Debug::LogError("Invalid magnification filter specified.");
        return 0;
    }

    GLenum GetTextureFormat(const Texture& texture)
    {
        uint32_t chanels = texture.GetChanels();
        return (chanels == 3) ? GL_RGB : GL_RGBA;
    }
}
