#pragma once

#include <Feng/Math/Matrix4.h>
#include <Feng/Render/RenderBase.h>
#include <Feng/Render/VertexBuffer.h>
#include <Feng/ScenesManager/Component.h>

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
        void SetDirectShadowTexture(int32_t bufferId);
        void SetPointShadowTexture(int32_t bufferId);

        void Draw(const RenderProperties &aRenderProperties, bool isShadowPass, Material *externalMaterial = nullptr);

    private:
        void StartDraw(Material &workingMaterial);
        void FinishDraw();

        void SetGlobalUniforms(const RenderProperties &renderProperties, Material &workingMaterial, bool isShadowPass);
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

        void BindShadowMaps(uint32_t firstTextureUnit, Material &workingMaterial);
        void SetCameraUniforms(const RenderProperties &renderProperties, Material &workingMaterial);
        void SetLightUniforms(const RenderProperties &renderProperties, Material &workingMaterial);
        void SetShadowLightUniform(const RenderProperties &renderProperties, Material &workingMaterial, bool isShadowPass);

        VertexBuffer vertexBuffer;
        uint32_t instancesBuffer = Render::UndefinedBuffer;

        std::map<std::string, uint32_t> textureBuffers;

        Material *material = nullptr;
        Mesh *mesh = nullptr;
        uint32_t instancesCount = 0;
        uint32_t firstInstanceAttributeIndex = 0;
        uint32_t directShadowMapId = Render::UndefinedBuffer;
        uint32_t pointShadowMapId = Render::UndefinedBuffer;
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
