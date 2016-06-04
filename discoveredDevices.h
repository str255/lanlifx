/*  
    Copyright 2016 nicholas c. strauss (nicholas.strauss@gmail.com)

    This file is part of LANLIFX.

    LANLIFX is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      LANLIFX is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LANLIFX.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __DISCOVERED_DEVICES__
#define __DISCOVERED_DEVICES__

#include <stdbool.h>

#define     MAXDEVICES      100
#define     MAXLABEL        32

/* devices are identified by label, ipstring, mac. 
   The label is user interface and should be unique. */
typedef struct {
  int          device_no;
  bool         labelled;
  char         label[MAXLABEL];
  char         ip[MAXLABEL];
  uint8_t      mac[8];
  int          service;
  int          port;
} discoveredDevice;

#endif
