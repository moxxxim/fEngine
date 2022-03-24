#pragma once

#include <Feng/Math/Matrix4.h>
#include <Feng/Render/RenderBase.h>
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

        const Mesh *GetMesh() const;
        Mesh *GetMesh();
        void SetMesh(Mesh *aMesh);
        void SetInstanceTransforms(const std::vector<Matrix4>& instances);

        bool CanDraw() const;
        void Draw(const RenderProperties &renderProperties);

    private:
        void StartDraw();
        void FinishDraw();

        void SetGlobalUniforms(const RenderProperties &renderProperties);
        void ExecuteDraw();
        void ActivateTexture(const std::string& name, const Texture &texture, uint32_t unit);

        void CreateGeometryBuffers();
        void DeleteGeomertyBuffers();

        void CreateTexturesBuffers();
        void DeleteTextureBuffers();

        void CreateInstanceBuffer();
        void UpdateInstanceBuffer(const std::vector<Matrix4>& instances);
        void DeleteInstanceBuffer();

        uint32_t CreateVertexBuffer();
        uint32_t CreateIndexBuffer();

        void SetCameraUniforms(const RenderProperties &renderProperties);
        void SetLightUniforms(const RenderProperties &renderProperties);
        void SetFogUniforms(const RenderProperties &renderProperties);

        uint32_t vao = Render::UndefinedBuffer;
        uint32_t vbo = Render::UndefinedBuffer;
        uint32_t ibo = Render::UndefinedBuffer;
        uint32_t instancesBuffer = Render::UndefinedBuffer;

        std::map<std::string, uint32_t> textureBuffers;

        Material *material = nullptr;
        Mesh *mesh = nullptr;
        uint32_t instancesCount = 0;
        uint32_t firstInstanceAttributeIndex = 0;
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
