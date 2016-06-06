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
#include <sys/time.h>
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
#include <getopt.h>
#include "lifx_header.h"
#include "deviceMessages.h"
#include "lightMessages.h"
#include "discoveredDevices.h"
#include "mac.h"
#include "lightBom.h"

bomMessageHandler_t bomMessages[maxBomMessages] = {
  { light_setColor, "color", set_bom_color, },
  { setLabel, "label", set_bom_label, },
  { light_setPower, "power", set_bom_power, },
  { setPower, "powerDevice", set_bom_device_power, },
};

void
set_bom_color(uint8_t *bom, char *value_option)
{
  lx_light_set_color_bom_t     *light = (lx_light_set_color_bom_t*)bom;
  int     h, s, b, k, duration;
  
  sscanf(value_option,"%x:%x:%x:%x:%x", &h, &s, &b, &k, &duration);
  printf("value option: %x:%x:%x:%x:%x", h, s, b, k, duration);
  light->color[0]= h;
  light->color[1]= s;
  light->color[2]= b;
  light->color[3]= k;
  light->duration = duration;
}

void
set_bom_label(uint8_t *bom, char *value_option)
{
  printf("setting bom label %s\n", value_option);
  sprintf(bom, "%s", value_option); 
}

void
set_bom_power(uint8_t *bom, char *value_option)
{
  int           power;
  int           duration = 100000; // 1 sec
  lx_light_set_power_bom_t     *light = (lx_light_set_power_bom_t*)bom;

  printf("setting bom power %s\n", value_option);

  if (strncmp(value_option,"on",2)==0) power = 65535;
  else power = 0;

  light->level = power;
  light->duration = 0;
}

typedef struct {
  uint16_t                level;
} lx_device_power_bom_t;

void
set_bom_device_power(uint8_t *bom, char *value_option)
{
  int           power;
  lx_device_power_bom_t     *device = (lx_device_power_bom_t*)bom;

  printf("setting bom power %s\n", value_option);

  if (strncmp(value_option,"on",2)==0) power = 65535;
  else power = 0;

  device->level = power;
}
