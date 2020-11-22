//#pragma once
//
//#include <FEngine/ScenesManager/PostEffectContext.h>
//
//#include <map>
//#include <vector>
//
//namespace fengine
//{
//    class Material;
//    class Shader;
//    class FrameBuffersPool;
//    class MaterialPostEffect;
//
//    class PostEffectPass final
//    {
//        friend MaterialPostEffect;
//    public:
//        enum class InputTextureType : int
//        {
//            Color = 0,
//            Depth = 1
//        };
//
//        PostEffectPass(
//                       Material *material,
//                       std::map<const char *, unsigned>& m_shaderInputTextures,
//                       int customBuffersCount,
//                       FrameBuffersPool *m_buffersPool);
//
//        ~PostEffectPass();
//
//        void SetTextureFromOriginal(InputTextureType type, const char *textureName);
//        void SetTextureFromInput(InputTextureType type, const char *textureName);
//        void SetTextureFromCustom(InputTextureType type, const char *textureName, int index);
//        void SetRenderBufferCustom(int index);
//        void SetRenderBufferOutput();
//        Material * GetMaterial() const;
//
//    private:
//        void Setup(const PostEffectContext& context);
//        void Reset();
//        void SetShaderInput(const char *textureName, unsigned int buffer);
//
//        PostEffectContext m_context;
//        Material *m_material = nullptr;
//        FrameBuffersPool *m_buffersPool = nullptr;
//        StringMap<unsigned>& m_shaderInputTextures;
//        int m_customBuffersCount = 0;
//        FrameBuffer *m_buffers = nullptr;
//    };
//
//    Material * PostEffectPass::GetMaterial() const
//    {
//        return m_material;
//    }
//}
