#pragma once

#include <Feng/Math/Vector2.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/Vector4.h>
#include <Feng/Render/RenderBase.h>
#include <Feng/Render/ShaderBindings.hpp>
#include <Feng/ResourcesManager/Shader.h>

#include <map>
#include <utility>

namespace Feng
{
    class Texture;

    class Material final
    {
    public:
        explicit Material(std::unique_ptr<Shader>&& aShader);
        ~Material();

        bool HasShader() const;

        Shader* GetShader();
        const Shader* GetShader() const;
        void SetShader(std::unique_ptr<Shader>&& aShader);

        const ShaderBindings& Bindings() const;
        ShaderBindings& Bindings();
        
        bool IsTransparent() const;
        void SetTransparent(bool aTransparent);
        void SetDrawFace(eDrawFace face);
        void Apply();

    private:
        void ApplyStates();
        std::unique_ptr<Shader> shader;
        
        ShaderBindings bindings;
        eDrawFace drawFace = eDrawFace::Cw;
        bool transparent = false;
    };

    inline Shader* Material::GetShader()
    {
        return const_cast<Shader*>(static_cast<const Material*>(this)->GetShader());
    }
    
    inline const Shader* Material::GetShader() const
    {
        return shader.get();
    }

    inline void Material::SetShader(std::unique_ptr<Shader>&& aShader)
    {
        shader = std::move(aShader);
    }
    
    inline const ShaderBindings& Material::Bindings() const
    {
        return bindings;
    }
    
    inline ShaderBindings& Material::Bindings()
    {
        return const_cast<ShaderBindings&>(static_cast<const Material*>(this)->Bindings());
    }
}
