//#include <FEngine/Utils/VertexAttributeInfo.h>
//
//#include <FEngine/Utils/Debug.h>
//#include <FEngine/ResourcesManager/ShaderParameters.h>
//
//namespace fengine
//{
//    VertexAttributeInfo::VertexAttributeInfo(const char *attributeName)
//    {
//        ShaderParameters::ReservedAttribute attribute = ShaderParameters::GetReservedAttribute(attributeName);
//        GLfloat *pointer = 0;
//        switch (attribute)
//        {
//            case ShaderParameters::ReservedAttribute::VertexPositionL:
//            {
//                Size = 3;
//                Type = GL_FLOAT;
//                IsNormalized = GL_FALSE;
//                Pointer = pointer;
//            }
//            break;
//
//            case ShaderParameters::ReservedAttribute::VertexUV:
//            {
//                Size = 2;
//                Type = GL_FLOAT;
//                IsNormalized = GL_FALSE;
//                Pointer = pointer + 3;
//            }
//            break;
//            case ShaderParameters::ReservedAttribute::VertexNorm:
//            {
//                Size = 3;
//                Type = GL_FLOAT;
//                IsNormalized = GL_FALSE;
//                Pointer = pointer + 5;
//            }
//            break;
//            case ShaderParameters::ReservedAttribute::VertexBinorm:
//            {
//                Size = 3;
//                Type = GL_FLOAT;
//                IsNormalized = GL_FALSE;
//                Pointer = pointer + 8;
//            }
//            break;
//            case ShaderParameters::ReservedAttribute::VertexTangent:
//            {
//                Size = 3;
//                Type = GL_FLOAT;
//                IsNormalized = GL_FALSE;
//                Pointer = pointer + 11;
//            }
//            break;
//            default:
//            {
//                Debug::LogWarning("Wrong attribute name specified.");
//            }
//            break;
//        }
//    }
//}
