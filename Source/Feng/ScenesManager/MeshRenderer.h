#pragma once

#include <Feng/Math/Matrix4.h>
#include <Feng/Render/RenderBase.h>
#include <Feng/Render/VertexBuffer.h>
#include <Feng/ScenesManager/Component.h>
#include <Feng/Utils/Render/MeshParams.h>
#include <Feng/Utils/Render/TextureParams.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Feng
{
    class Camera;
    class Material;
    class Mesh;
    class RenderProperties;
    class Texture;

    class MeshRenderer final : public Component
    {
    public:
        MeshRenderer() = default;
        ~MeshRenderer() override;

        const Material *GetMaterial() const;
        Material *GetMaterial();
        void SetMaterial(Material *aMaterial);
        bool IsShadowCaster() const;
        void SetShadowCaster(bool aShadowCaster);

        const Mesh *GetMesh() const;
        Mesh *GetMesh();
        void SetMesh(Mesh *aMesh);
        void SetInstanceTransforms(const std::vector<Matrix4>& instances);
        void AddExternalTexture(const std::string_view& name, eTextureType type, int32_t bufferId);
        void RemoveExternalTexture(const std::string_view& name);
        void ActivateAttributes(eVertexAtributes aAttributes);
        void DeactivateAttributes(eVertexAtributes aAttributes);

        void Draw(const RenderProperties &aRenderProperties, bool isShadowPass, Material *externalMaterial = nullptr);

    private:
        void StartDraw(Material &workingMaterial);
        void FinishDraw();

        void SetModelUniforms(Material &workingMaterial);
        void ExecuteDraw();

        void CreateGeometryBuffers();
        void DeleteGeomertyBuffers();

        void CreateTexturesBuffers();
        void DeleteTextureBuffers();

        void CreateInstanceBuffer();
        void UpdateInstanceBuffer(const std::vector<Matrix4>& instances);
        void DeleteInstanceBuffer();

        uint32_t CreateVertexBuffer();
        uint32_t CreateIndexBuffer();

        void BindExternalTextures(uint32_t firstTextureUnit, Material &workingMaterial);

        VertexBuffer vertexBuffer;
        uint32_t instancesBuffer = Render::UndefinedBuffer;
        std::map<std::string, uint32_t> textureBuffers;

        Material *material = nullptr;
        Mesh *mesh = nullptr;
        eVertexAtributes activeAttributes = eVertexAtributes::Empty;
        uint32_t instancesCount = 0;
        uint32_t firstInstanceAttributeIndex = 0;
        std::map<std::string_view, std::pair<eTextureType, uint32_t>> externalTextures;
        bool shadowCaster = false;
    };
}

namespace Feng
{
    inline const Material *MeshRenderer::GetMaterial() const
    {
        return material;
    }

    inline Material *MeshRenderer::GetMaterial()
    {
        return material;
    }

    inline const Mesh *MeshRenderer::GetMesh() const
    {
        return mesh;
    }

    inline Mesh *MeshRenderer::GetMesh()
    {
        return mesh;
    }
}
