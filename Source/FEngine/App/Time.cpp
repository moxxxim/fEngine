#include <FEngine/App/Time.h>

namespace fengine
{
    Time *Time::GetInstance()
    {
        static Time time;
        return &time;
    }
}
