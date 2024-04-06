#pragma once

#include <Feng/Core/FengGL.h>
#include <array>
#include <cstdint>

namespace Feng
{
    enum eVertexAtributes : uint32_t
    {
        Empty = 0,
        Position = 1 << 0,
        Normal = 1 << 1,
        Uv0 = 1 << 2,
        Tangent = 1 << 3,
        Bitangent = 1 << 4,
    };

    enum class ePrimitiveType : uint32_t
    {
        Triangles = 0,
    };

    class AttributesParser final
    {
    public:
        static const std::array<eVertexAtributes, 5> Order;
        static const std::array<uint32_t, 5> DataSizes;
        static const std::array<GLint, 5> ComponentNumbers;
        static const std::array<GLenum, 5> DataTypes;
        static const std::array<GLboolean, 5> Normalizations;

        static uint32_t GetStride(eVertexAtributes aAttributes);

        AttributesParser(eVertexAtributes aAttributes);

        bool HasCurrent() const;
        bool IsCurrentSet() const;
        void GetCurrent(GLint &componentsNumber, GLenum& type, GLboolean& normalize, GLsizei& dataSize);
        void Next();

    private:
        eVertexAtributes attributes;
        int32_t index = 0;
    };

    GLenum ToOpenGLValue(ePrimitiveType primitiveType);
}

namespace Feng
{
    inline AttributesParser::AttributesParser(eVertexAtributes aAttributes)
        : attributes { aAttributes }
    { }

    inline bool AttributesParser::HasCurrent() const
    {
        return index < Order.size();
    }

    inline bool AttributesParser::IsCurrentSet() const
    {
        return (attributes & Order[index]) != 0;
    }

    inline void AttributesParser::GetCurrent(GLint &componentsNumber, GLenum& type, GLboolean& normalize, GLsizei& dataSize)
    {
        componentsNumber = ComponentNumbers[index];
        type = DataTypes[index];
        normalize = Normalizations[index];
        dataSize = DataSizes[index];
    }

    inline void AttributesParser::Next()
    {
        ++index;
    }
}
