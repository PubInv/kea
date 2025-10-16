#include "kea_time.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <thread>
#include <chrono>
#endif

namespace Kea
{
    void DelayMs(unsigned long ms)
    {
#ifdef ARDUINO
        delay(ms);
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
    }

    unsigned long Millis()
    {
#ifdef ARDUINO
        return millis();
#else
        return static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
#endif
    }

    uint32_t Timer::TimeSinceEpochMs() const
    {
#ifdef ARDUINO
        // Time since device powered on
        return static_cast<uint32_t>(x_millis());
#else
        // Time since Unix epoch
        return static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
#endif
    }

    void Timer::Init(uint32_t msStart)
    {
        _msElapsed = 0;
        _msStart = msStart;
    }

    void Timer::Init()
    {
        _msElapsed = 0;
        _msStart = TimeSinceEpochMs();
    }

    uint32_t Timer::Update()
    {
#ifdef ARDUINO
        // The Arudino ms timer can "rollover" after 49.7 days.
        // We use x_millis() to test this. When this happens,
        // we definitely cannot perform a simple subtraction from
        // _msStart. The simplest thing to do is to set _msStart to
        // m in this case. This will create an incorrect time
        // (which lasts a little too long) once every 49.7 days.
        uint32_t m = x_millis();

        // Note, when this happens, we in fact need to update all the
        // schedule times
        if (m < _msStart)
        {
            _msStart = m;
            if (m < _msStart)
            {
                Kea::DebugLn<const char *>("INTERNAL ERROR IN TIMER ROLLOVER!\n");
            }
        }

        _msElapsed = (uint32_t)m - _msStart;
#else
        _msElapsed = static_cast<uint32_t>(TimeSinceEpochMs()) - _msStart;
#endif
        return _msElapsed;
    }

    uint32_t Timer::GetElapsed()
    {
        return _msElapsed;
    }

    void Timer::Reset()
    {
        Init();
    }
}
