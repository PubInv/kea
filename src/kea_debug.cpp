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

#include "kea_debug.h"
#ifdef ARDUINO
#include <Arduino.h>
#else
#include <iostream>
#endif

namespace Kea {

  void serial_begin(unsigned long baud){
    #ifdef ARDUINO
      Serial.begin(baud);
    #else
      std::cout << "Serial init\n";
    #endif
  }

}
