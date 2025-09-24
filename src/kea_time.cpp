#include "kea_time.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <thread>
#include <chrono>
#endif

namespace Kea {
    void delay_ms(unsigned long ms) {
#ifdef ARDUINO
        delay(ms);
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
    }
}
