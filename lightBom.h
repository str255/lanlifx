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
#ifndef __LIGHTBOM__
#define __LIGHTBOM__

typedef struct {
  uint16_t            itype;
  char                str[32];
  void                (*handler)(uint8_t *bom, char *value_option);
} bomMessageHandler_t;

enum { maxBomMessages = 4
};

void set_bom_color(uint8_t *bom, char *value_option);
void set_bom_label(uint8_t *bom, char *value_option);
void set_bom_power(uint8_t *bom, char *value_option);
void set_bom_device_power(uint8_t *bom, char *value_option);


#pragma pack(push, 1)
typedef struct {
  uint8_t       reserved;
  uint16_t      color[4];              // HSBK
  uint32_t      duration;
} lx_light_set_color_bom_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
  uint16_t      level;           
  uint32_t      duration;
} lx_light_set_power_bom_t;
#pragma pack(pop)


#endif
