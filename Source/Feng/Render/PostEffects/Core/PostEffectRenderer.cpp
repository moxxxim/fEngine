#include <Feng/Render/PostEffects/Core/PostEffectRenderer.h>

#include <Feng/Core/FengGL.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/Utils/Debug.h>
#include <Feng/Utils/Render/TextureParams.h>
#include <Feng/Utils/Render/RenderUtils.h>

namespace Feng
{
    PostEffectRenderer::PostEffectRenderer(
                                    Material *aMaterial,
                                    const std::map<std::string, uint32_t>* aFrameBufferTextures)
        : material { aMaterial }
        , frameBufferTextures { aFrameBufferTextures }
    { }

    void PostEffectRenderer::Draw()
    {
        Shader *shader = material->GetShader();
        shader->Use();

        SetupBufferedTextures();
        Render::ResolveBindings(*shader, material->Bindings(), materialTextures, static_cast<uint32_t>(frameBufferTextures->size()));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        shader->Use();
    }

    void PostEffectRenderer::SetupBufferedTextures()
    {
        const Shader* shader = material->GetShader();
        unsigned int textureUnit = 0;
        for(const auto& [name, buffer] : *frameBufferTextures)
        {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, buffer);
            Print_Errors_OpengGL();
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
