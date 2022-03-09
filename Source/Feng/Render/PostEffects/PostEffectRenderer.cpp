#include <Feng/Render/PostEffects/PostEffectRenderer.h>

#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/Utils/Render/TextureParams.h>
#include <Feng/Utils/Render/RenderUtils.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace Feng
{
    PostEffectRenderer::PostEffectRenderer(
                                    Material *aMaterial,
                                    const std::map<std::string, uint32_t>& aFrameBufferTextures)
        : material { aMaterial }
        , frameBufferTextures { aFrameBufferTextures }
    { }

    void PostEffectRenderer::Draw()
    {
        const Shader *shader = material->GetShader();
        shader->StartUse();

        SetupBufferedTextures();
        Render::BindMaterialUniforms(*material, materialTextures, frameBufferTextures.size());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader->StopUse();
    }

    void PostEffectRenderer::SetupBufferedTextures()
    {
        const Shader* shader = material->GetShader();
        unsigned int textureUnit = 0;
        for(const auto& [name, buffer] : frameBufferTextures)
        {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, buffer);
            shader->SetUniformInt(name.c_str(), textureUnit);
            ++textureUnit;
        }
    }

    void PostEffectRenderer::SetupTexturesBuffers()
    {
        materialTextures = Render::CreateTextureBuffers(*material);
    }

    void PostEffectRenderer::ClearTexturesBuffers()
    {
        for(const auto& [name, unit] : materialTextures)
        {
            glDeleteTextures(1, &unit);
        }

        materialTextures.clear();
    }
}
