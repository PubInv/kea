#include "kea_debug.h"
#include "kea_scheduler.h"
#include "kea_error_handler.h"
#include "kea_time.h"
// #include "util.h"

namespace Kea
{
    // TODO: move
    bool WithinArrayBounds(int index, int length) {
        return ((index >= 0) && (index < length));
    }


    Scheduler::Scheduler()
    {
        _properties.mode = SchedulerMode::RealTime;
        setupIdleTask();
    }

    void Scheduler::setupIdleTask()
    {
        _idleTask._properties.priority = static_cast<TaskPriority>(TaskPriorityOS::Idle);
        _idleTask._properties.period = 1;
        _idleTask._properties.id = 0;
    }

    void Scheduler::incrementVirtualTimes(TimeMs ms)
    {
        for (int i = 0; i < map.getCount(); i++)
        {
            Task *task = map.getValueByIndex(i);
            task->_ms_since_last_run += ms;
        }
        virtualTime_ms += ms;
    }

    void Scheduler::advanceVirtualTimeByIncrementAndBaseline(TimeMs increment)
    {
        // In order to atomically change the VirtualTime, must we must add the
        // same amount to all of the _lastRunAgo_ms fields.
        // This is a very large sentinel value;

        if (DEBUG_SCHEDULER > 4)
        {
            Kea::Debug<const char *>("increment: ");
            Kea::DebugLn<uint32_t>(increment);
        }

        incrementVirtualTimes(increment);
    }
    Task *Scheduler::getNextTaskToRun()
    {
        // Record how long the previous task took to run
        if (DEBUG_SCHEDULER > 1)
        {
            Kea::Debug<const char *>("virtualTime_ms: ");
            Kea::DebugLn<uint32_t>(virtualTime_ms);
        }

        Task *nextTask = nullptr;
        TimeMs maxTimeUntilDeadline = -99999;
        for (int i = 0; i < map.getCount(); i++)
        {
            Task *task = map.getValueByIndex(i);

            TimeMs ms_since_last_run = task->TimeSinceLastRunMs();
            TimeMs period = task->GetPeriod();

            // The next task to run is the most positive one
            // < 0 time remaining to run
            // = 0 due now
            // > 0 time-to-runoverrun
            task->_timeUntilDeadline = ms_since_last_run - period;

            if (DEBUG_SCHEDULER > 1)
            {
                double x = ms_since_last_run;
                double y = period;
                double z = virtualTime_ms;
                double q = z - (x + y);
                Kea::Debug<const char *>("Q Deadline: ");
                Kea::DebugLn<double>(q);
                Kea::Debug<const char *>("LastRunTimeAgo: ");
                Kea::DebugLn<uint32_t>(ms_since_last_run);
                Kea::Debug<const char *>("timeUntilDeadline: ");
                Kea::Debug<const char *>(task->_properties.name);
                Kea::Debug<const char *>(" ");
                Kea::DebugLn<int32_t>(task->_timeUntilDeadline);
                // Kea::Debug<int32_t>(task->_timeUntilDeadline);
                Kea::Debug<const char *>("\n");
            }

            if (task->_timeUntilDeadline > maxTimeUntilDeadline)
            {
                maxTimeUntilDeadline = task->_timeUntilDeadline;
                nextTask = task;
            }
        }
        if (DEBUG_SCHEDULER > 0)
        {
            if (nextTask == nullptr)
            {
                Kea::Debug<const char *>("nextTask: IDLE\n");
            }
            else
            {
                Kea::Debug<const char *>("nextTask\n");
                Kea::Debug<const char *>(nextTask->_properties.name);
                Kea::Debug<const char *>("\n");
                if (DEBUG_SCHEDULER > 2)
                    DelayMs(50);
            }
        }

        if (nextTask == nullptr)
        {
            nextTask = &_idleTask;
        }
        return nextTask;
    }

    bool Scheduler::AddTask(Task *task, TaskProperties *properties)
    {
        if (WithinArrayBounds(_numberOfTasks, MAX_TASKS))
        {
            TaskState state = task->Init(properties);
            if (state == TaskState::Ready)
            {
                map.add(properties->id, task);
                _numberOfTasks++;
                return true;
            }
            else
            {
                ErrorHandler::Log(ErrorLevel::Error, ErrorCode::CoreFailedToAddTask);
            }
        }
        // Out of bounds

        return false;
    }

    bool Scheduler::Init()
    {
        switch (_properties.mode)
        {
        case SchedulerMode::RoundRobin:
            //
            return true;
            break;
        case SchedulerMode::RealTime:
            //
            return true;
            break;
        default:
            return false;
            break;
        }
    }

    // Note our naming here is inconsistent...
    // This makes this look like it is a timestamp,
    // but when it is called it looks like a duration.
    // I think this is actoung the time elapsed since the timer started,
    // Though that is not Init'ed only once in the current code.

    TaskState Scheduler::RunNextTask()
    {

        Task *nextTask = nullptr;

        switch (_properties.mode)
        {
        case SchedulerMode::RoundRobin:
            ErrorHandler::Log(ErrorLevel::Critical, ErrorCode::NotImplemented);
            break;
        case SchedulerMode::RealTime:
            nextTask = getNextTaskToRun();
            break;
        default:
            ErrorHandler::Log(ErrorLevel::Critical, ErrorCode::NotImplemented);
            break;
        }

        // Calling the clock is dangerous, but this is short-duration
        // and inside debugging code.
        unsigned long ms;
        if (DEBUG_SCHEDULER > 1)
        {
            Kea::Debug<const char *>("About to Run task!\n");
            Kea::Debug<const char *>(nextTask->_properties.name);
            Kea::Debug<const char *>(" : ");
            ms = Kea::Millis(); //t_millis();
            Kea::DebugLn<uint32_t>(ms);
            Kea::Debug<const char *>("\n");
        }
        nextTask->Run();
        if (DEBUG_SCHEDULER > 1)
        {
            Kea::Debug<const char *>("Finished Run! ");
            Kea::Debug<const char *>(" : ");
            Kea::DebugLn<uint32_t>(Kea::Millis() - ms);
            Kea::Debug<const char *>("\n");
        }
        //    _lastTaskRan = nextTask;
        return nextTask->GetState();
    }

    TaskState Scheduler::RunTaskById(TaskId id)
    {
        Task *tp = map.getValue(id);
        if (tp == nullptr)
        {
            return TaskState::Error;
        }
        else
        {
            _currentRunningTaskId = id;
            tp->Run();
            return TaskState::Running;
        }
    }

    TaskId Scheduler::GetRunningTaskId() const
    {
        return _currentRunningTaskId;
    }

    Task *Scheduler::GetTaskById(TaskId id)
    {
        Task *tp = map.getValue(id);
        return tp;
    }

    void Scheduler::SetProperties(SchedulerProperties properties)
    {
        _properties = properties;
    }

    SchedulerProperties Scheduler::GetProperties()
    {
        return _properties;
    }

    uint32_t Scheduler::GetTickPeriod()
    {
        return _properties.tickPeriodMs;
    }

}
