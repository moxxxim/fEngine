#pragma once

#include <cstdint>

namespace Feng
{
    class PostEffectPass;
    class PostEffectPrePass;

    class PostEffectPassBehaviour
    {
    public:
        virtual ~PostEffectPassBehaviour() = default;

        int32_t GetPassesCount() const;
        virtual void Initialize(PostEffectPrePass &prePass) = 0;
        virtual void SetupPass(int32_t passIndex, PostEffectPass &pass) = 0;

    protected:
        PostEffectPassBehaviour() = default;

    private:
        int32_t passesCount = 1;
    };
    
    inline int32_t PostEffectPassBehaviour::GetPassesCount() const
    {
        return passesCount;
    }
}
