#include <Feng/Math/Matrix3.h>

namespace feng
{
    const Matrix3 Matrix3::Zero
    {
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f
    };

    const Matrix3 Matrix3::Identity
    {
        1.f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f
    };
}
