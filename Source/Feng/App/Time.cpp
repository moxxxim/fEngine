#include <Feng/App/Time.h>

namespace feng
{
    Time *Time::GetInstance()
    {
        static Time time;
        return &time;
    }
}
