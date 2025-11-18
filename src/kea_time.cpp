#include "kea_time.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <thread>
#include <chrono>
#endif

namespace Kea
{
#ifdef ARDUINO

// If you want to test the millisecond rollover mechanism,
// set this is a non-zero value with an extern declaration
// unsigned long ROLLOVER_FOR_TESTING = 120*1000;
unsigned long ROLLOVER_FOR_TESTING = 0;

unsigned long fake_addition_to_millis = 0;
// make this 0 to test.

unsigned long t_millis() {
  unsigned long m = millis();
  if (fake_addition_to_millis) {
    if (ROLLOVER_FOR_TESTING) {
     return (fake_addition_to_millis + m) % ROLLOVER_FOR_TESTING;
    } else {
      return fake_addition_to_millis + m;
    }
  } else {
    if (ROLLOVER_FOR_TESTING) {
     return m % ROLLOVER_FOR_TESTING;
    } else {
     return m;
    }
  }
}

// Note: m is the "now" time presumables set by calling t_millis();
// It is provided to allow "dependency injection" (informally), to
// allow the value to be set very close to the Rollover.
unsigned long set_t_millis(unsigned long new_millis_value_ms, unsigned long m) {

  if (ROLLOVER_FOR_TESTING) {
    if (new_millis_value_ms > m) {
      fake_addition_to_millis = (new_millis_value_ms-m) % ROLLOVER_FOR_TESTING;
    } else if (new_millis_value_ms < m) {
      fake_addition_to_millis = (ROLLOVER_FOR_TESTING - (m - new_millis_value_ms)) % ROLLOVER_FOR_TESTING;
    } else {
      fake_addition_to_millis = 0;
    }
  } else {
    fake_addition_to_millis = (m - new_millis_value_ms);
  }
  return t_millis();
}

// A testable version of millis, which can be used to
// test the rollover problem.
unsigned long x_millis() {
  return t_millis();
}

unsigned long t_millis_assert_no_rollover(unsigned long previous,
                                          bool& error) {
  unsigned long m = millis();
  if (m < previous) {
    error = true;
  } else {
    error = false;
  }
  // Do this in order to allow testing, rather than returning m
  return t_millis();
}
#endif // ARDUINO
    
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
                // Kea::DebugLn<const char *>("INTERNAL ERROR IN TIMER ROLLOVER!\n");
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
