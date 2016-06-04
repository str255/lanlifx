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
#include <getopt.h>
#include "mac.h"

#define isMAC (mylifx->target[0]|mylifx->target[1]|mylifx->target[2]|mylifx->target[3]|mylifx->target[4]|mylifx->target[5]|mylifx->target[6]|mylifx->target[7])

char *
sprint_MACaddr(char *buf, uint8_t mac[8])
{
  sprintf(buf, "%x:%x:%x:%x:%x:%x:%x:%x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6], mac[7]);
  return buf;
}
