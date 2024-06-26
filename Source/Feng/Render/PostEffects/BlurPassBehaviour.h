#pragma once

#include <Feng/Render/PostEffects/Core/PostEffectPassBehaviour.h>

namespace Feng
{
    class Material;

    class BlurPassBehaviour final : public PostEffectPassBehaviour
    {
    public:
        BlurPassBehaviour() = default;
        BlurPassBehaviour(float *blurStrength, int passesCount);

        void Initialize(PostEffectPrePass& prePass) override;
        void SetupPass(int passIndex, PostEffectPass& pass) override;

    private:
        void SetupMaterial(int passIndex, Material *material);

        float *m_blurStrength = nullptr;
        int32_t m_passesCount = 1;
    };
}
