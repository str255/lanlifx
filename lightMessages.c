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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h> 
#define __USE_GNU
#include <search.h>
#include "lifx_header.h"
#include "deviceMessages.h"
#include "lightMessages.h"
#include "discoveredDevices.h"
#include "mac.h"

extern uint8_t          MAC_target[8];
extern discoveredDevice discoveredDevices[MAXDEVICES];
extern struct hsearch_data  *labtab; // *mactab,

deviceMessage_t lightMessages[maxDeviceMessages] = {
    {  light_getColor                 ,"light_getColor", handle_light_getColor, },
    {  light_setColor                 ,"light_setColor", handle_light_setColor, },
    {  light_stateColor               ,"light_stateColor", handle_light_stateColor, },
    {  light_getPower                 ,"light_getPower", handle_light_getPower, },
    {  light_setPower                 ,"light_setPower", handle_light_setPower, },
    {  light_statePower               ,"light_statePower", handle_light_statePower, },
};


typedef struct {
  uint16_t      color[4];              // HSBK
  int16_t       reserved;
  uint16_t      power;
  char          label[32];
  uint64_t      reserved2;
} lx_light_color_bom_t;

void handle_light_getColor(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_light_setColor(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_light_stateColor(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_light_color_bom_t   *hf = (lx_light_color_bom_t*)bom;
  printf("color: %x:%x:%x:%x power: %d label %s\n",
	 hf->color[0], hf->color[1], hf->color[2], hf->color[3],
	 hf->power,
	 hf->label);
}


typedef struct {
  uint16_t                level;
  uint32_t                duration;  
} lx_light_power_bom_t;

void handle_light_getPower(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_light_setPower(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_light_statePower(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_light_power_bom_t   *hf = (lx_light_power_bom_t*)bom;
  printf("level: %d duration: %d\n",
	 hf->level, hf->duration);
}
