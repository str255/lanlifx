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
#ifndef __LIFX_HEADER__
#define __LIFX_HEADER__
/*  LIFX Protocol message 
    frame, frame address, 
    protocol header,
    payload
*/
#pragma pack(push, 1)
typedef struct {
  /* frame */
  uint16_t size;
  uint16_t protocol:12;
  uint8_t  addressable:1;
  uint8_t  tagged:1;
  uint8_t  origin:2;
  uint32_t source;
  /* frame address */
  uint8_t  target[8];
  uint8_t  reserved[6];
  uint8_t  res_required:1;
  uint8_t  ack_required:1;
uint8_t  :6;
  uint8_t  sequence;
  /* protocol header */
uint64_t :64;
  uint16_t type;
uint16_t :16;
  /* variable length payload follows */
} lx_protocol_header_t;

typedef struct {
  lx_protocol_header_t          header;
  uint8_t                       bom[0];
} lx_protocol_pdu_t;
#pragma pack(pop)


typedef lx_protocol_header_t *pdu_header_t;
typedef lx_protocol_pdu_t *pdu_t;

enum truth { false = 0, true = 1};
#define     BUFLEN          16384 

#endif
