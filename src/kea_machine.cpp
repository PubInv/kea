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

#include "kea_machine.h"
#include "kea_core.h"
#include <assert.h>
#include <cmath>
#include <cstring>
#include <cstdio>

namespace Kea {

void MachineConfig::outputReport(MachineStatusReport *msr)
{
  Kea::Debug<const char *>("\n");
  Kea::Debug<const char *>("Timestamp: ");
  Kea::Debug<unsigned long>(msr->timestamp);
  Kea::Debug<const char *>("\n");
  Kea::Debug<const char *>("Machine State: ");
  Kea::Debug<const char *>(MachineConfig::MachineStateNames[msr->state]);
  Kea::Debug<const char *>("\n");
  Kea::Debug<const char *>("Fan PWM   0-1: ");
  Kea::DebugLn<float>(msr->fan_pwm);
  Kea::Debug<const char *>("Fan RPM      : ");
  Kea::DebugLn<float>(msr->fan_rpm);
  // add a state for error: no error
  for (int i = 0; i < NUM_CRITICAL_ERROR_DEFINITIONS; i++)
  {
    if (msr->errors[i]) // critical error detected
    {
      Kea::Debug<const char *>("CriticalError: ");
      Kea::Debug<const char *>(CriticalErrorNames[i]);
      Kea::Debug<const char *>("\n");
    }
  }
}

void MachineConfig::createJSONReport(MachineStatusReport *msr, char *buffer)
{
  sprintf(buffer + strlen(buffer), "\"RawMillis\": %ld", msr->timestamp);
  strcat(buffer, ",\n");
  sprintf(buffer + strlen(buffer), "\"MachineState\": %d", msr->state);
  strcat(buffer, ",\n");
  sprintf(buffer + strlen(buffer), "\"FanPWM\": %.2f", msr->fan_pwm);
  strcat(buffer, ",\n");
  sprintf(buffer + strlen(buffer), "\"FanRPM\": %.2f", msr->fan_rpm);
  strcat(buffer, ",\n");
  sprintf(buffer + strlen(buffer), "\"CriticalError\": [");
  bool is_fault_present = false;
  for (int i = 0; i < NUM_CRITICAL_ERROR_DEFINITIONS; i++)
  {
    if (msr->errors[i]) // critical error detected
    {
      if (is_fault_present)
        strcat(buffer, ",");
      is_fault_present = true;
      strcat(buffer, "\"");
      sprintf(buffer + strlen(buffer), CriticalErrorNames[i]);
      strcat(buffer, "\"");
      strcat(buffer, "\n");
    }
  }
  if (!is_fault_present)
  {
    strcat(buffer, "\"No Error\"\n");
  }
  strcat(buffer, "],\n");
}

bool MachineHAL::init()
{
  return true;
}

bool MachineConfig::init()
{
  Kea::Debug<const char *>("error toleration times (ms)\n");
  for (int i = 0; i < 3; i++)
  {
    Kea::Debug<const char *>("Errors index: ");
    Kea::Debug<int>(i);
    Kea::Debug<const char *>(" ");
    Kea::Debug<uint32_t>(errors[i].toleration_ms);
    Kea::Debug<const char *>("\n");
  }
  return true;
}

}