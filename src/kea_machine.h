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

#ifndef KEA_MACHINE_H
#define KEA_MACHINE_H

#include "kea_circular_array.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

#include "machine_core_defs.h"

namespace Kea
{

  class MachineHAL
  {
  public:
    int DEBUG_HAL = 0;

    virtual bool init() = 0;
  };

  constexpr inline static char const *CriticalErrorNames[NUM_CRITICAL_ERROR_DEFINITIONS] = {
      "Fan Unresponsive",
      "PSU Unresponsive",
      "Mains Power Loss",
      "System Over Temperature"
  };

  class MachineConfig
  {
  public:
    MachineConfig(){};
    ~MachineConfig(){};


    CriticalError errors[NUM_CRITICAL_ERROR_DEFINITIONS];

    static const int NUM_MACHINE_STATES = 9;

    constexpr inline static char const *MachineStateNames[9] = {
        "Off",
        "AwaitingPower",
        "Warmup",
        "NormalOperation",
        "Cooldown",
        "CriticalFault",
        "EmergencyShutdown",
        "OffUserAck"};

    MachineState state;
    // This is used to make decisions that happen at transition time.
    MachineState previous_state;

    // MachineScript* script;

    // MachineHAL* hal;

    // MachineStatusReport *report;

    bool init();

    void outputReport(MachineStatusReport *msr);
    void createJSONReport(MachineStatusReport *msr, char *buffer);

    /********************************************
     Begin compile-time parameters
     ********************************************/

    // The "heartbeat" on the OEDCS v.1.1 is both a red LED and PIN 13.
    static const int INIT_HEARTBEAT_PERIOD_MS = 500;   // heartbeat task period
    static const int INIT_PANEL_BLINK_PERIOD_MS = 500; // heartbeat task period

    // The is the basic recording of values in "emergency logging mode"
    static const int INIT_LOG_RECORDER_PERIOD_MS = 1000;

    // Task2 asked for 10 minutes of logging data at 1 Hz, so 600 records
    static constexpr unsigned int MAX_RECORDS = 600;
    MachineStatusReport _log_entry[MAX_RECORDS];

    // If we are LOW_TEMP_TRIGGER below our setpoint, we will lower the fan speed
    const float LOW_TEMP_TRIGGER = 20;
  };
}
#endif
