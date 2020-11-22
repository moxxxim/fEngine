//#pragma once
//
//#include <FEngine/ScenesManager/FrameBuffer.h>
//
//#include <vector>
//
//namespace fengine
//{
//    class FrameBuffersPool final
//    {
//    public:
//        FrameBuffersPool();
//        ~FrameBuffersPool();
//
//        void Prewarm(int count, bool requireDepth);
//        FrameBuffer GetBuffer(bool requireDepth);
//        void PutBuffer(FrameBuffer buffer);
//
//    private:
//        bool TryGetRequiredBuffer(bool requireDepth, FrameBuffer& buffer);
//        FrameBuffer CreateBuffer(bool requireDepth);
//
//        std::vector<FrameBuffer> m_buffers;
//    };
//}
