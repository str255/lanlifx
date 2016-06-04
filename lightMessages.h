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
#ifndef __LIGHT_MSGS__
#define __LIGHT_MSGS__

enum lightMessage_e {
  light_getColor = 101,
  light_setColor = 102,
  light_stateColor = 107,
  light_getPower = 116,
  light_setPower = 117,
  light_statePower = 118,
};

enum { maxLightMessages = 6
};

void handle_light_getColor(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_light_setColor(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_light_stateColor(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_light_getPower(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_light_setPower(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_light_statePower(pdu_header_t header, uint8_t *bom, char *ip, int device_no);

#endif
