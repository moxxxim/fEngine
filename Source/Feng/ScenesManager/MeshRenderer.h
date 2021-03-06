#pragma once

#include <Feng/ScenesManager/Component.h>
#include <Feng/Math/Matrix4.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace feng
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
        void SetInstanceTransforms(const std::vector<feng::Matrix4>& instances);

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
        void UpdateInstanceBuffer(const std::vector<feng::Matrix4>& instances);
        void DeleteInstanceBuffer();

        uint32_t CreateVertexBuffer();
        uint32_t CreateIndexBuffer();

        void SetCameraUniforms(const RenderProperties &renderProperties);
        void SetLightUniforms(const RenderProperties &renderProperties);
        void SetFogUniforms(const RenderProperties &renderProperties);

        static constexpr uint32_t UndefinedBuffer = 0;

        uint32_t vao = UndefinedBuffer;
        uint32_t vbo = UndefinedBuffer;
        uint32_t ibo = UndefinedBuffer;
        uint32_t instancesBuffer = UndefinedBuffer;

        std::map<std::string, uint32_t> textureBuffers;

        Material *material = nullptr;
        Mesh *mesh = nullptr;
        uint32_t instancesCount = 0;
    };
}

namespace feng
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
