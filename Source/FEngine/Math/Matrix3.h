#ifndef Matrix3_hpp
#define Matrix3_hpp

namespace fengine
{
    class Matrix3 final
    {
    public:
        union
        {
            float data[9];
            struct
            {
                float m00;
                float m01;
                float m02;
                float m10;
                float m11;
                float m12;
                float m20;
                float m21;
                float m22;
            };
        };
    };
}
#endif
