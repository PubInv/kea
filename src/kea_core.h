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

#ifndef KEA_CORE_H
#define KEA_CORE_H

#include "kea_scheduler.h"
#include "kea_error_handler.h"
#include "kea_time.h"
#include "kea_debug.h"

namespace Kea
{

    enum class CoreState
    {
        Undefined,
        Configured,
        Starting,
        Running,
        Error,
        Terminating
    };

    class Core
    {
    public:
        int DEBUG_CORE = 0;
        bool Configure(void *config);
        bool Boot();
        bool AddTask(Task *task, TaskProperties *properties);
        bool Run();
        static void RaiseCriticalError();
        static bool _criticalError;

        // make this public to allow debug to be set.
        Scheduler _scheduler;

        // Core() : _state(CoreState::Undefined) {};
        // ~Core() = default;
        // // Cannot copy class
        // Core(const Core &) = delete;
        // Core &operator=(const Core) = delete;
        // // Cannot move class
        // Core(Task &&) = delete;
        // Core &operator=(Core &&) = delete;

        Scheduler *GetScheduler() { return &_scheduler; }

        void ResetHardwareWatchdog();

        void ResetAllWatchdogs();

    private:
        uint32_t _elapsed;
        Timer _primaryTimer;
        Timer _watchdogTimer;
        CoreState _state;
        
        void Tick();
        void CreateSoftwareWatchdog(uint32_t timeoutMs);
        void CreateHardwareWatchdog();

        bool ResetSoftwareWatchdog();
        uint32_t GetElapsedTime();
    };

}

#endif
