#pragma once

#include <Feng/ScenesManager/Component.h>

#include <cstdint>
#include <map>
#include <string>

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

        void SetMaterial(const Material *aMaterial);
        void SetMesh(const Mesh *aMesh);

        bool CanDraw() const;
        void Draw(const RenderProperties &renderProperties);

    private:
        void StartDraw();
        void FinishDraw();

        void SetGlobalUniforms(const RenderProperties &renderProperties);
        void SetMaterialUniforms();
        void ExecuteDraw();
        void ActivateTexture(const std::string& name, const Texture &texture, uint32_t unit);

        void CreateGeometryBuffers();
        void DeleteGeomertyBuffers();

        void CreateTexturesBuffers();
        void DeleteTextureBuffers();

        uint32_t CreateVertexBuffer();
        uint32_t CreateIndexBuffer();

        uint32_t CreateTextureBuffer(const Texture& texture);
        void GenerateTexture(const Texture& texture);
        void GenerateTexture2D(const Texture& texture);
        void GenerateTextureCube(const Texture& texture);

        void SetCameraUniforms(const RenderProperties &renderProperties);
        void SetLightUniforms(const RenderProperties &renderProperties);
        void SetFogUniforms(const RenderProperties &renderProperties);

        static constexpr uint32_t UndefinedBuffer = 0;

        uint32_t vao = UndefinedBuffer;
        uint32_t vbo = UndefinedBuffer;
        uint32_t ibo = UndefinedBuffer;
        std::map<std::string, uint32_t> textureBuffers;

        const Material *material = nullptr;
        const Mesh *mesh = nullptr;
    };
}
