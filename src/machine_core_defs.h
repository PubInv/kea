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

#ifndef MACHINE_CORE_DEFS_H
#define MACHINE_CORE_DEFS_H

namespace Kea
{
  enum MachineState
  {
    // Off is the initial state. It is a zero-power state.
    Off,
    // AwaitingPower - in a blackout where we do not have enough power to continue
    AwaitingPower,
    // Operating
    NormalOperation,
    // A critical fault has occurred or an acknowledgment has not been received
    CriticalFault,
    // Emergency Shutdown: stop power  as quickly as possible
    EmergencyShutdown,
    // Remain Off until a user releases this state
    OffUserAck
  };

  class CriticalError
  {
  public:
    bool fault_present;
    unsigned long begin_condition_ms;
    unsigned long toleration_ms;
    MachineState response_state;
  };

#define NUM_CRITICAL_ERROR_DEFINITIONS 4

  enum CriticalErrorCondition
  {  
    FAN_UNRESPONSIVE,
    PSU_UNRESPONSIVE,
    MAINS_LOSS_PWR,
    SYSTEM_OVER_TEMPERATURE,
  };

  struct MachineStatusReport
  {
    MachineState state;
    float fan_pwm;
    float fan_rpm;
    unsigned long timestamp;
    bool errors[NUM_CRITICAL_ERROR_DEFINITIONS];
  };

}
#endif
