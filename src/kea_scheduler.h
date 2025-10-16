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

#ifndef KEA_SCHEDULER_H
#define KEA_SCHEDULER_H

#include "kea_map.h"
#include "kea_task.h"
#include "kea_debug.h"

namespace Kea
{

    // This is a lightweight co-operative scheduler.
    // There is no overhead in context switching,
    // there are some conditions:
    // - Tasks must 'yield' by returning
    //   true (success) or false (failure).
    // - Tasks should be small, fast and non-blocking.
    // - Interrupts are handled by the scheduler.

    // [ Task1, Task2, Task3, Task4, Empty ]
    //            ^             ^      ^
    //         current        number  max

    class IdleTask : public Task
    {
    public:
        int DEBUG_IDLETASK = 0;

    private:
        bool _init() override
        {
            Kea::Debug<const char *>("Init idle task\n");
            return true;
        }
        bool _run() override
        {
            if (DEBUG_IDLETASK > 0)
            {
                Kea::Debug<const char *>("Run idle task\n");
            }
            return true;
        }
    };

    enum class SchedulerMode
    {
        RoundRobin = 0,
        RealTime = 1,
    };

    struct SchedulerProperties
    {
        SchedulerMode mode;
        uint32_t tickPeriodMs = 0;
    };

    class Scheduler
    {
    private:
        // Task* _lastTaskRan = 0;
        TimeMs wallClockTimer_ms = 0;

        // virtualTime is relative to the tasks and the scheduler as a whole;
        // it is kept self-consistent (and low) by subtracing values whenever
        // possible. This whole system could probably be termed into an O(1)
        // scheduler by keeping the tasks sorted, but there is little payoff
        // in doing that for NASA.
        TimeMs virtualTime_ms = 0;
        static const int32_t MAX_TASKS = 40; // TODO: make this better
        Kea::Collections::Map<TaskId, Task *, MAX_TASKS> map;
        TaskId _currentRunningTaskId = 0;
        int32_t _numberOfTasks = 0;
        SchedulerProperties _properties;
        void setupIdleTask();
        Task *getNextTaskToRun();

    public:
        Scheduler();
        ~Scheduler() = default;
        IdleTask _idleTask; // Special task not part of the task map
        int DEBUG_SCHEDULER = 0;
        bool Init();
        bool AddTask(Task *task, TaskProperties *properties);

        //  void decrementVirtualTimesToBaseline(TimeMs ms);
        void incrementVirtualTimes(TimeMs ms);
        void advanceVirtualTimeByIncrementAndBaseline(TimeMs ms);
        TaskState RunNextTask();
        TaskState RunTaskById(TaskId id);
        TaskId GetRunningTaskId() const;
        Task *GetTaskById(TaskId id);
        void SetProperties(SchedulerProperties properties);
        SchedulerProperties GetProperties();
        uint32_t GetTickPeriod();
    };

}

#endif
