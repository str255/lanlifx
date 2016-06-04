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
#ifndef __DEVICE_MSGS__
#define __DEVICE_MSGS__

enum deviceMessage_e {
  getService               = 2,
  stateService             = 3,
  getHostInfo              = 12,
  stateHostInfo            = 13,
  getHostFirmware          = 14,
  stateHostFirmware        = 15,
  getWifiInfo              = 16,
  stateWifiInfo            = 17,
  getWifiFirmware          = 18,
  stateWifiFirmware        = 19,
  getPower                 = 20,
  setPower                 = 21,
  statePower               = 22,
  getLabel                 = 23,
  setLabel                 = 24,
  stateLabel               = 25,
  getVersion               = 32,
  stateVersion             = 33,
  getInfo                  = 34,
  stateInfo                = 35,
  acknowledgement          = 45,
  getLocation              = 48,
  stateLocation            = 50,
  getGroup                 = 51,
  stateGroup               = 53,
  echoRequest              = 58,
  echoResponse             = 59,
};


#include "lifx_header.h"

typedef struct {
  uint16_t            itype;
  char                str[32];
  void                (*handler)(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
} deviceMessage_t;

void handle_getService(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_stateService(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_getHostInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_stateHostInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_getHostFirmware(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_stateHostFirmware(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_getWifiInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_stateWifiInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_getWifiFirmware(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_stateWifiFirmware(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_getPower(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_setPower(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_statePower(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_getLabel(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_setLabel(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_stateLabel(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_getVersion(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_stateVersion(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_getInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_stateInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_acknowledgement(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_getLocation(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_stateLocation(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_getGroup(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_stateGroup(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_echoRequest(pdu_header_t header, uint8_t *bom, char *ip, int device_no);
void handle_echoResponse(pdu_header_t header, uint8_t *bom, char *ip, int device_no);

enum { maxDeviceMessages = 27
};

#endif
