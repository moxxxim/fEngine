#pragma once

#include <Feng/Math/Vector2.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/Vector4.h>
#include <Feng/Math/Matrix4.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Feng
{
    class Texture;
    
    class ShaderBindings final
    {
    public:
        ShaderBindings() = default;
        
        bool HasTexture(const std::string& name) const;
        bool HasFloat(const std::string& name) const;
        bool HasVector4(const std::string& name) const;
        
        const std::map<std::string, float>& GetFloats() const;
        bool TryGetFloat(const std::string& name, float& value) const;
        void SetFloat(const std::string& name, float value);

        const std::map<std::string, int>& GetInts() const;
        bool TryGetInt(const std::string& name, int& value) const;
        void SetInt(const std::string& name, int value);

        const std::map<std::string, Vector2>& GetVectors2() const;
        bool TryGetVector2(const std::string& name, Vector2& value) const;
        void SetVector2(const std::string& name, Vector2 value);

        const std::map<std::string, Vector3>& GetVectors3() const;
        bool TryGetVector3(const std::string& name, Vector3& value) const;
        void SetVector3(const std::string& name, Vector3 value);

        const std::map<std::string, Vector4>& GetVectors4() const;
        bool TryGetVector4(const std::string& name, Vector4& value) const;
        void SetVector4(const std::string& name, const Vector4& value);

        const std::map<std::string, const Texture*>& GetTextures() const;
        Texture* GetTexture(const std::string& name);
        const Texture* GetTexture(const std::string& name) const;
        void SetTexture(const std::string& name, const Texture *texture);
        
        bool TryGetArrayFloats(const std::string& name, std::vector<float>& value) const;
        void SetArrayFloats(const std::string& name, const std::vector<float>& value);
        
        bool TryGetArrayMatrices4(const std::string& name, std::vector<Matrix4>& matrices) const;
        void SetArrayMatrices4(const std::string& name, const std::vector<Matrix4>& matrices);
        
        bool TryGetBuffer(const std::string& name, uint32_t& buffer) const;
        void SetBuffer(const std::string& name, uint32_t buffer);
        
    private:
        template <typename T>
        static bool TryGetValue(const std::map<std::string, T>& values, const std::string& name, T& value);
        
        std::map<std::string, float> floats;
        std::map<std::string, std::vector<float>> arrayFloats;
        std::map<std::string, int> ints;
        std::map<std::string, uint32_t> buffers;
        std::map<std::string, Vector2> vectors2;
        std::map<std::string, Vector3> vectors3;
        std::map<std::string, Vector4> vectors4;
        std::map<std::string, const Texture*> textures;
        std::map<std::string, std::vector<Matrix4>> arrayMatrices;
    };
}
