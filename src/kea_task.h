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

#ifndef KEA_TASK_H
#define KEA_TASK_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <stdint.h>
#endif

// Forward declaration of global MachineConfig (defined in application code)
class MachineConfig;

namespace Kea {

typedef int64_t TimeMs;
typedef int32_t TaskId;

enum class TaskState {
    Undefined = 0,
    Ready,
    Running,
    Blocked,
    Suspended,
    Error
};

enum class TaskPriorityOS {
    Kernal = 0,
    Idle = 250,
};

enum class TaskPriority {
    High = 60,
    Medium = 70,
    Low = 80,
    Undefined = 255
};

struct TaskProperties {
    const char * name;
    TaskId id;
    TaskPriority priority;
    TimeMs period;
    // We place a void pointer here to hold a pointer
    // to the state and configuration of the machine under control.
    // In general, tasks will need to know this. However,
    // I don't want to go so far as the create type correctness for this.
    // so a task that uses this point is responsible for correctly
    // casting it to the correct type. This let's this code remain
    // absolutely "core" functionality, and localize all code
    // into specific task overrides.
    void* state_and_config;
};

class Task {
    private:
        bool _initialized;
        // User defined:
        virtual bool _init() = 0;
        virtual bool _run() = 0;

        // Only the scheduler should call these:
        TaskState Init(TaskProperties *properties);
        void Run();
        TaskState Wait(TimeMs now);
    protected:
        TaskState _state;
        // This is a VirtualTimeStamp that is meaningful only relative
        // to the virtualTime_ms in the scheduler.
        TimeMs _ms_since_last_run;
        // This is a duration.
        TimeMs _lastRunDuration;
        // This should probably be removed....
        TimeMs _timeUntilDeadline; // this is a duration
        TaskProperties _properties;
    public:
        Task(): _initialized(false),
                _state(TaskState::Undefined),
                //                _lastRun(0),
                _properties({"noname", -1,TaskPriority::Undefined,0})
                {};
        int DEBUG_TASK = 0;
        ::MachineConfig *getConfig();
        virtual ~Task() = default;
        // Cannot copy class
        Task(const Task&) = delete;
        Task& operator=(const Task&) = delete;
        // Cannot move class
        Task(Task&&) = delete;
        Task& operator=(Task&&) = delete;

        //bool Callback(char *message);

        TaskId GetId() const;
        TaskPriority GetPriority() const;
        TaskState GetState() const;
        TimeMs GetPeriod() const;
		void SetPeriod( TimeMs );
        TimeMs TimeSinceLastRunMs() const;
        bool IsHardTiming() const;

        // The scheduler has privileged access to tasks
        friend class Scheduler;
};

}

#endif // KEA_TASK_H
