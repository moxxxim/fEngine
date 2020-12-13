#include <Feng/Utils/Render/MeshParams.h>

#include <Feng/Math/Vector2.h>
#include <Feng/Math/Vector3.h>

namespace feng
{
    const std::array<eVertexAtributes, 5> AttributesParser::Order
    {
        eVertexAtributes::Position,
        eVertexAtributes::Normal,
        eVertexAtributes::Tangent,
        eVertexAtributes::Binormal,
        eVertexAtributes::Uv0
    };

    const std::array<uint32_t, 5> AttributesParser::DataSizes
    {
        static_cast<uint32_t>(sizeof(Vector3)),
        static_cast<uint32_t>(sizeof(Vector3)),
        static_cast<uint32_t>(sizeof(Vector3)),
        static_cast<uint32_t>(sizeof(Vector3)),
        static_cast<uint32_t>(sizeof(Vector2)),
    };

    const std::array<GLint, 5> AttributesParser::ComponentNumbers { 3, 3, 3, 3, 2 };

    const std::array<GLenum, 5> AttributesParser::DataTypes
    {
        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT
    };

    const std::array<GLboolean, 5> AttributesParser::Normalizations
    {
        GL_FALSE,
        GL_FALSE,
        GL_FALSE,
        GL_FALSE,
        GL_FALSE
    };

    uint32_t AttributesParser::GetStride(eVertexAtributes aAttributes)
    {
        uint32_t stride = 0;

        for(int i = 0; i < Order.size(); ++i)
        {
            eVertexAtributes attribute = Order[i];
            if((attribute & aAttributes) != 0)
            {
                stride += DataSizes[i];
            }
        }

        return stride;
    }

    GLenum ToOpenGLValue(ePrimitiveType primitiveType)
    {
        switch (primitiveType)
        {
            case ePrimitiveType::Triangles:
                return GL_TRIANGLES;
        }
    }
}
