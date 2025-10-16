#ifdef ARDUINO
#include <Arduino.h>
#else
#include <thread>
#include <chrono>
#endif

#include "kea_debug.h"
#include "kea_task.h"

namespace Kea
{
    TaskState Task::Init(TaskProperties *properties)
    {
        if (_state == TaskState::Undefined)
        {
            _properties = *properties;
            _state = _init() ? TaskState::Ready : TaskState::Error;
        }
        return _state;
    }

    // Argument is the "virtual time" in the scheduler
    void Task::Run()
    {
        if (_state == TaskState::Ready)
        {
            _state = TaskState::Running;
            // We are initating the run 0 ms from the current virtual time
            _ms_since_last_run = 0;
            if (DEBUG_TASK > 0)
            {
                Kea::Debug<const char *>("about to _run\n");
            }
            _run(); // TODO: use result
            if (DEBUG_TASK > 0)
            {
                Kea::Debug<const char *>("finished _run\n");
            }
        }
        else
        {
            // Not ready to run
            // TODO: report
        }
        _state = TaskState::Ready;
        if (DEBUG_TASK > 0)
        {
            Kea::Debug<const char *>("Returning Run\n");
        }
    }

    TaskState Task::Wait(TimeMs now)
    {
        if (_state == TaskState::Running)
        {
            // TODO: do waiting stuff
            _state = TaskState::Ready;
        }
        return _state;
    }

    TaskId Task::GetId() const
    {
        return _properties.id;
    }

    TaskPriority Task::GetPriority() const
    {
        return _properties.priority;
    }

    TaskState Task::GetState() const
    {
        return _state;
    }

    // This is going to change to how long ago the task was run
    // against the virtual time (not the actual time.)
    // That means that each time we call get NextTaskToRun,
    // we have to advance virtual time. The Scheduler is a
    // Friend class and can set this directly.
    TimeMs Task::TimeSinceLastRunMs() const
    {
        return _ms_since_last_run;
    }

    TimeMs Task::GetPeriod() const
    {
        return _properties.period;
    }

    void Task::SetPeriod(TimeMs newPeriod)
    {
        if (newPeriod != _properties.period)
        {
            _properties.period = newPeriod;
        }
    }

    void *Task::getConfig()
    {
        return _properties.state_and_config;
    }

    /*bool Task::Callback(char *message)
    {
        std::cout << "Task Callback: " << message << std::endl;
        return true;
    }*/

}
