#include <Feng/ScenesManager/MeshRenderer.h>

#include <Feng/Math/MatrixUtils.h>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Light.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/ScenesManager/RenderProperties.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Mesh.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/ResourcesManager/Texture.h>
#include <Feng/Utils/Render/ShaderParams.h>
#include <Feng/Utils/Render/RenderUtils.h>
#include <Feng/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace Feng
{
    MeshRenderer::~MeshRenderer()
    {
        DeleteGeomertyBuffers();
        DeleteTextureBuffers();
    }

    void MeshRenderer::SetMaterial(Material *aMaterial)
    {
        if (material != aMaterial)
        {
            material = aMaterial;
            DeleteTextureBuffers();
            if(material)
            {
                CreateTexturesBuffers();
            }
        }
    }

    bool MeshRenderer::IsShadowCaster() const
    {
        return shadowCaster;
    }
    
    void MeshRenderer::SetShadowCaster(bool aShadowCaster)
    {
        shadowCaster = aShadowCaster;
    }
    
    void MeshRenderer::SetMesh(Mesh *aMesh)
    {
        if (mesh != aMesh)
        {
            if(mesh)
            {
                DeleteGeomertyBuffers();
            }

            mesh = aMesh;
            if(mesh)
            {
                CreateGeometryBuffers();
            }
        }
    }

    void MeshRenderer::SetInstanceTransforms(const std::vector<Matrix4>& instances)
    {
        if(static_cast<uint32_t>(instances.size()) > instancesCount)
        {
            if(instancesCount > 0)
            {
                DeleteInstanceBuffer();
            }
            
            CreateInstanceBuffer();
            Print_Errors_OpengGL();
        }

        UpdateInstanceBuffer(instances);
        Print_Errors_OpengGL();

        instancesCount = static_cast<uint32_t>(instances.size());
    }

    void MeshRenderer::AddExternalTexture(const std::string_view& name, eTextureType type, int32_t bufferId)
    {
        externalTextures[name] = std::make_pair(type, bufferId);
    }
    
    void MeshRenderer::RemoveExternalTexture(const std::string_view& name)
    {
        externalTextures.erase(name);
    }
    
    void MeshRenderer::Draw(const RenderProperties &renderProperties, bool isShadowPass, Material *externalMaterial)
    {
        Material* workingMaterial = externalMaterial ? externalMaterial : material;
        
        if (mesh && workingMaterial)
        {
            std::map<std::string, uint32_t> externalTextureBuffers;
            if(externalMaterial)
            {
                externalTextureBuffers = Render::CreateTextureBuffers(*externalMaterial);
            }
            
            std::map<std::string, uint32_t> *workingTextures = externalMaterial
                ? &externalTextureBuffers
                : &textureBuffers;
            
            StartDraw(*workingMaterial);
            Print_Errors_OpengGL();
            Render::ResolveBindings(*workingMaterial->GetShader(), renderProperties.globalBindings, {});
            SetModelUniforms(*workingMaterial);
            Print_Errors_OpengGL();
            Render::ResolveBindings(*workingMaterial->GetShader(), workingMaterial->Bindings(), *workingTextures);
            BindExternalTextures(static_cast<uint32_t>(workingTextures->size()), *workingMaterial);
            Print_Errors_OpengGL();
            
            ExecuteDraw();
            FinishDraw();
            Print_Errors_OpengGL();
            
            for(auto&& [name, tbo] : externalTextureBuffers)
            {
                glDeleteTextures(1, &tbo);
            }
        }
    }

    void MeshRenderer::StartDraw(Material &workingMaterial)
    {
        workingMaterial.Apply();
        Print_Errors_OpengGL();
    }

    void MeshRenderer::FinishDraw()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::UndefinedBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
        Print_Errors_OpengGL();
    }

    void MeshRenderer::SetModelUniforms(Material &workingMaterial)
    {
        if(instancesCount == 0)
        {
            const Shader *shader = workingMaterial.GetShader();
            const Transform *myTransform = GetEntity()->GetComponent<Transform>();
            shader->SetUniformMatrix4(ShaderParams::ModelMatrix.data(), myTransform->GetGlobalMatrix());
            Print_Errors_OpengGL();
        }
    }

    void MeshRenderer::ExecuteDraw()
    {
        glBindVertexArray(vertexBuffer.vao);

        ePrimitiveType primitiveType = mesh->GetPrimitiveType();

        // No need to call 'glBindBuffer' because it was bound while VAO was still bound.

        if(vertexBuffer.ibo != Render::UndefinedBuffer)
        {
            const std::vector<uint32_t>& indices = mesh->GetIndices();
            if(instancesCount > 0)
            {
                glDrawElementsInstanced(
                               ToOpenGLValue(primitiveType),
                               indices.size(),
                               GL_UNSIGNED_INT,
                               reinterpret_cast<void*>(0),
                               instancesCount);
            }
            else
            {
                glDrawElements(
                               ToOpenGLValue(primitiveType),
                               indices.size(),
                               GL_UNSIGNED_INT,
                               reinterpret_cast<void*>(0));
            }
        }
        else
        {
            int32_t verticesCount = mesh->GetVerticesCount();
            if(instancesCount > 0)
            {
                glDrawArraysInstanced(ToOpenGLValue(primitiveType), 0, verticesCount, instancesCount);
            }
            else
            {
                glDrawArrays(ToOpenGLValue(primitiveType), 0, verticesCount);
            }
        }

        glBindVertexArray(Render::UndefinedBuffer);
        Print_Errors_OpengGL();
    }

    void MeshRenderer::CreateGeometryBuffers()
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        vertexBuffer.vao = vao;
        vertexBuffer.vbo = CreateVertexBuffer();

        if(mesh->HasIndices())
        {
            vertexBuffer.ibo = CreateIndexBuffer();
        }

        glBindVertexArray(Render::UndefinedBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::UndefinedBuffer);
    }

    void MeshRenderer::DeleteGeomertyBuffers()
    {
        glDeleteBuffers(1, &vertexBuffer.vbo);
        glDeleteBuffers(1, &vertexBuffer.ibo);
        glDeleteBuffers(1, &vertexBuffer.vao);
        glDeleteBuffers(1, &instancesBuffer);

        vertexBuffer.vao = vertexBuffer.vbo = vertexBuffer.ibo = instancesBuffer = Render::UndefinedBuffer;
    }

    void MeshRenderer::CreateTexturesBuffers()
    {
        textureBuffers = Render::CreateTextureBuffers(*material);
    }

    void MeshRenderer::DeleteTextureBuffers()
    {
        for(const auto& [name, tbo] : textureBuffers)
        {
            glDeleteTextures(1, &tbo);
        }
    }

    void MeshRenderer::CreateInstanceBuffer()
    {
        glGenBuffers(1, &instancesBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, instancesBuffer);
        glBindVertexArray(vertexBuffer.vao);

        glEnableVertexAttribArray(firstInstanceAttributeIndex);
        glVertexAttribPointer(firstInstanceAttributeIndex, sizeof(float), GL_FLOAT, GL_FALSE,
                              sizeof(Matrix4), reinterpret_cast<void*>(0));
        glEnableVertexAttribArray(firstInstanceAttributeIndex + 1);
        glVertexAttribPointer(firstInstanceAttributeIndex + 1, sizeof(float), GL_FLOAT, GL_FALSE,
                              sizeof(Matrix4), reinterpret_cast<void*>(sizeof(Vector4)));
        glEnableVertexAttribArray(firstInstanceAttributeIndex + 2);
        glVertexAttribPointer(firstInstanceAttributeIndex + 2, sizeof(float), GL_FLOAT, GL_FALSE,
                              sizeof(Matrix4), reinterpret_cast<void*>(2 * sizeof(Vector4)));
        glEnableVertexAttribArray(firstInstanceAttributeIndex + 3);
        glVertexAttribPointer(firstInstanceAttributeIndex + 3, sizeof(float), GL_FLOAT, GL_FALSE,
                              sizeof(Matrix4), reinterpret_cast<void*>(3 * sizeof(Vector4)));

        glVertexAttribDivisor(firstInstanceAttributeIndex, 1);
        glVertexAttribDivisor(firstInstanceAttributeIndex + 1, 1);
        glVertexAttribDivisor(firstInstanceAttributeIndex + 2, 1);
        glVertexAttribDivisor(firstInstanceAttributeIndex + 3, 1);

        glBindVertexArray(Render::UndefinedBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
    }

    void MeshRenderer::UpdateInstanceBuffer(const std::vector<Matrix4>& instances)
    {
        if(!instances.empty())
        {
            glBindBuffer(GL_ARRAY_BUFFER, instancesBuffer);
            Print_Errors_OpengGL();
            glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(Matrix4), instances.data(), GL_STATIC_DRAW);
            Print_Errors_OpengGL();
            glBindBuffer(GL_ARRAY_BUFFER, Render::UndefinedBuffer);
        }
    }

    void MeshRenderer::DeleteInstanceBuffer()
    {
        glDeleteBuffers(1, &instancesBuffer);
    }

    uint32_t MeshRenderer::CreateVertexBuffer()
    {
        uint32_t bufferObject;
        glGenBuffers(1, &bufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
        glBufferData(GL_ARRAY_BUFFER, mesh->GetDataSize(), mesh->GetData(), GL_STATIC_DRAW);

        firstInstanceAttributeIndex = Render::EnableVertexAttributes(mesh->GetAttributes());

        return bufferObject;
    }

    uint32_t MeshRenderer::CreateIndexBuffer()
    {
        const std::vector<uint32_t>& indices = mesh->GetIndices();

        GLuint bufferObject;
        glGenBuffers(1, &bufferObject);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

        return bufferObject;
    }
    
    void MeshRenderer::BindExternalTextures(uint32_t firstTextureUnit, Material &workingMaterial)
    {
        uint32_t textureUnit = firstTextureUnit;
        for(auto&& [name, typeAndId] : externalTextures)
        {
            uint32_t bufferId = typeAndId.second;
            if(bufferId != Render::UndefinedBuffer)
            {
                Shader *shader = workingMaterial.GetShader();
                Render::BindTexture(*shader, typeAndId.first, textureUnit, name, bufferId);
                ++textureUnit;
            }
        }
    }
}
