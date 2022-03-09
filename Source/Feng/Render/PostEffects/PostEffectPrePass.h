#pragma once

#include <cstdint>

namespace Feng
{
    class FrameBuffer;

    class PostEffectPrePass final
    {
    public:
        PostEffectPrePass() = default;

        int32_t GetCustomBuffersCount() const;
        void SetCustomBuffersCount(int32_t count);

    private:
        int32_t customBuffersCount = 0;
    };
    
    inline int32_t PostEffectPrePass::GetCustomBuffersCount() const
    {
        return customBuffersCount;
    }

    inline void PostEffectPrePass::SetCustomBuffersCount(int32_t count)
    {
        customBuffersCount = count;
    }
}
