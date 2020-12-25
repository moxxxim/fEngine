#pragma once

#include <Feng/ScenesManager/PostEffectPass.h>
#include <Feng/ScenesManager/PostEffectPrePass.h>
#include <Feng/Serialization/SerializableObject.h>

namespace feng
{
    class PostEffectPassBehaviour
    {
    public:
        int32_t GetPassesCount() const;
        virtual void Initialize(PostEffectPrePass *prePass) = 0;
        virtual void SetupPass(int passIndex, PostEffectPass *pass) = 0;

    protected:
        PostEffectPassBehaviour() = default;

    private:
        int32_t passesCount = 1;
    };
}

namespace feng
{
    inline int32_t PostEffectPassBehaviour::GetPassesCount() const
    {
        return passesCount;
    }
}
