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

#include "kea_error_handler.h"
#include "kea_core.h"

#ifndef ARDUINO
#include <iostream>
#endif

namespace Kea
{

    ErrorMode ErrorHandler::errorMode = ErrorMode::StdOut;
    Kea::Collections::CircularArray<Error, MAX_ERRORS> ErrorHandler::errors;

    const char *ErrorLevelText[] = {
        "Info",
        "Debug",
        "Warning",
        "Error",
        "Critical",
    };

    const char *ErrorMessage[] = {
        "Null",
        "Less Than Zero",
        "Out Of Bounds",
        "Core Failed To Add Task",
        "Core Failed To Boot",
        "Core Failed To Run",
        "Task Priority Time Exceeded",
        "Watchdog Exceeded",
        "Not Implemented",
    };

    void ErrorHandler::SetErrorMode(ErrorMode mode)
    {
        errorMode = mode;
    }

    void ErrorHandler::Log(ErrorLevel level, ErrorCode type)
    {
        Error error = {level, type};
        errors.add(error);
        switch (errorMode)
        {
        case ErrorMode::Log:
            break;
        case ErrorMode::StdOut:
#ifdef ARDUINO
            Kea::Debug<const char *>(ErrorLevelText[static_cast<int>(level)]);
            Kea::Debug<const char *>(": ");
            Kea::Debug<const char *>(ErrorMessage[static_cast<int>(type)]);
            Kea::Debug<const char *>("\n");
#else
            std::cout << ErrorMessage[static_cast<int>(type)] << std::endl;
#endif
            break;
        default:

            break;
        }
        if (level == ErrorLevel::Critical)
        {
            Core::RaiseCriticalError();
        }
    }

}
