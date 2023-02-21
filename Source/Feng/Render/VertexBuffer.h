#pragma once

#include <Feng/Render/RenderBase.h>
#include <cstdint>

namespace Feng
{
    struct VertexBuffer final
    {
        uint32_t vao = Render::UndefinedBuffer;
        uint32_t vbo = Render::UndefinedBuffer;
        uint32_t ibo = Render::UndefinedBuffer;
    };
}
