#ifndef Matrix4Utils_hpp
#define Matrix4Utils_hpp

#include <FEngine/Math/Matrix3.h>
#include <FEngine/Math/Matrix4.h>
#include <FEngine/Math/Vector3.h>
#include <FEngine/Math/Vector4.h>

namespace fengine
{
    namespace matutils
    {
        Matrix4 MakeScale(const Vector3& scale);
        Matrix4 MakeRotation(const Matrix3& rotation);
        Matrix4 MakeTranslation(const Vector3& translation);
        Matrix4 MakeTransformation(const Vector3& scale, const Matrix3& rotation, const Vector3& translation);
        Matrix4 MakePerspectiveProjection(float fov, float aspect, float near, float far);
    }
}

#endif
 
