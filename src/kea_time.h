/*
Copyright (C) 2025 Public Invention.

This program includes free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

See the GNU Affero General Public License for more details.
You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#ifndef KEA_TIME_H
#define KEA_TIME_H

#include <cstdint>

namespace Kea {
    void DelayMs(unsigned long ms);
    unsigned long Millis();

    class Timer {
    private:
        uint32_t _msElapsed;
        uint32_t _msStart;
    public:
        Timer(): _msElapsed(0), _msStart(0) {}
        ~Timer() = default;
        // Cannot copy class
        Timer(const Timer&) = delete;
        Timer& operator=(const Timer&) = delete;
        // Cannot move class
        Timer(Timer&&) = delete;
        Timer& operator=(Timer&&) = delete;

        uint32_t TimeSinceEpochMs() const;
        void Init(uint32_t msStart);
        void Init();
        uint32_t Update();
        uint32_t GetElapsed();
        void Reset();
    };
}

#endif // KEA_TIME_H
