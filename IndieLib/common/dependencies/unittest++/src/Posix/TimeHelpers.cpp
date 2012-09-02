#include "TimeHelpers.h"
#include <unistd.h>

namespace UnitTest {

Timer::Timer()
{
    m_startTime.tv_sec = 0;
    m_startTime.tv_usec = 0;
}

void Timer::Start()
{
    gettimeofday(&m_startTime, 0);
}


int Timer::GetTimeInMs() const
{
    struct timeval currentTime;
    gettimeofday(&currentTime, 0);
    //MIGUEL: Fixed warning here by casting directly to int. Was warning as tv_sec is a long in OSX
    int dsecs = static_cast<int>(currentTime.tv_sec - m_startTime.tv_sec);
    int dus = currentTime.tv_usec - m_startTime.tv_usec;
    return dsecs*1000 + dus/1000;
}


void TimeHelpers::SleepMs (int ms)
{
    usleep(ms * 1000);
}

}
