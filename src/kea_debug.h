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

#ifndef KEA_DEBUG_H
#define KEA_DEBUG_H

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <iostream>
#endif

// Simple debug output function for Arduino and native environments.

// Use like:
// Debug<const char*>("Some text");
// Debug<int>(myIntVar);
// Debug<float>(myFloatVar);

// Beware that this will use RAM, so you should use the F() macro
// for string literals in Arduino builds to store the string in
// flash memory instead of RAM., like this:
// Debug<const char*>(F("Some text"));

// You can enable or disable debug output by defining or not defining
// EFRAMEWORK_DEBUG before including this header file.

// Macros are avoided because they can cause problems with scope and
// unexpected multiple evaluations of arguments.

// The type must be specified because in the future we may want to
// do something special for certain types, such as printing out
// floating point numbers with a specific number of decimal places.

namespace Kea
{

  template <class myType>
  void Debug(myType a)
  {
#ifdef ARDUINO
    Serial.print(a);
    Serial.flush();
#else
    std::cout << a;
#endif
  }

  template <class myType>
  void DebugLn(myType a)
  {
#ifdef ARDUINO
    Serial.print(a);
    Serial.print("\n");
    Serial.flush();
#else
    std::cout << a << std::endl;
#endif
  }

  void serialBegin(unsigned long baud);
}

#endif
