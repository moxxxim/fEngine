#include <FEngine/Math/Matrix3.h>

namespace fengine
{
    const Matrix3 Matrix3::Zero
    {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };

    const Matrix3 Matrix3::Identity
    {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
}
