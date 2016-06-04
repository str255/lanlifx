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
#include "discoveredDevices.h"
#include "mac.h"

extern uint8_t          MAC_target[8];
extern discoveredDevice discoveredDevices[MAXDEVICES];
extern struct hsearch_data  *labtab; // *mactab,

deviceMessage_t deviceMessages[maxDeviceMessages] = {
    {  getService               , "getService", handle_getService, },
    {  stateService             ,"stateService", handle_stateService, },
    {  getHostInfo              ,"getHostInfo", handle_getHostInfo, },
    {  stateHostInfo            ,"stateHostInfo", handle_stateHostInfo, },
    {  getHostFirmware          ,"getHostFirmware", handle_getHostFirmware, },
    {  stateHostFirmware        ,"stateHostFirmware", handle_stateHostFirmware, },
    {  getWifiInfo              ,"getWifiInfo", handle_getWifiInfo, },
    {  stateWifiInfo            ,"stateWifiInfo", handle_stateWifiInfo, },
    {  getWifiFirmware          ,"getWifiFirmware", handle_getWifiFirmware, },
    {  stateWifiFirmware        ,"stateWifiFirmware", handle_stateWifiFirmware, },
    {  getPower                 ,"getPower", handle_getPower, },
    {  setPower                 ,"setPower", handle_setPower, },
    {  statePower               ,"statePower", handle_statePower, },
    {  getLabel                 ,"getLabel", handle_getLabel, },
    {  setLabel                 ,"setLabel", handle_setLabel, },
    {  stateLabel               ,"stateLabel", handle_stateLabel, },
    {  getVersion               ,"getVersion", handle_getVersion, },
    {  stateVersion             ,"stateVersion", handle_stateVersion, },
    {  getInfo                  ,"getInfo", handle_getInfo, },
    {  stateInfo                ,"stateInfo", handle_stateInfo, },
    {  acknowledgement          ,"acknowledgement", handle_acknowledgement, },
    {  getLocation              ,"getLocation", handle_getLocation, },
    {  stateLocation            ,"stateLocation", handle_stateLocation, },
    {  getGroup                 ,"getGroup", handle_getGroup, },
    {  stateGroup               ,"stateGroup", handle_stateGroup, },
    {  echoRequest              ,"echoRequest", handle_echoRequest, },
    {  echoResponse             ,"echoResponse", handle_echoResponse, },
};

typedef struct { 
   uint8_t                 service; 
   uint32_t                port; 
} lx_stateService_bom_t; 

void handle_getService(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  
}

void handle_stateService(pdu_header_t mylifx, uint8_t *bom, char *ip, int device_no)
{
  lx_stateService_bom_t   *ss = (lx_stateService_bom_t*)bom;
  discoveredDevice *dev = &discoveredDevices[device_no];
#if 0
  // check target against dev
  if (dev->mac[0] == mylifx->target[0] &&
      dev->mac[1] == mylifx->target[1] &&    
      dev->mac[2] == mylifx->target[2] &&
      dev->mac[3] == mylifx->target[3] &&  
      dev->mac[4] == mylifx->target[4] &&
      dev->mac[5] == mylifx->target[5] &&
      dev->mac[6] == mylifx->target[6] &&
      dev->mac[7] == mylifx->target[6]) printf("mac checks out OK\n");
  else printf("bad mac checks out OK\n");
  printf("discovery %s MAC: %x:%x:%x:%x:%x:%x:%x:%x\n",
	 ip,
	 mylifx->target[0], mylifx->target[1], mylifx->target[2], mylifx->target[3],
	 mylifx->target[4], mylifx->target[5], mylifx->target[6], mylifx->target[7]);
  printf("service: %d port: %d\n",
	 ss->service, ss->port);
#endif
}    

typedef struct { 
  float                   signal;
  uint32_t                tx;
  uint32_t                rx;   
  uint16_t                reserved;
} lx_hostInfo_bom_t; 

void handle_getHostInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
}

void handle_stateHostInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_hostInfo_bom_t   *hs = (lx_hostInfo_bom_t*)bom;
  printf("signal: %f tx: %d rx; %d\n",
	 hs->signal, hs->tx, hs->rx);
}

typedef struct { 
  double                  build;
  uint64_t                reserved;
  uint32_t                version;
} lx_hostFirmware_bom_t; 

void handle_getHostFirmware(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_stateHostFirmware(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_hostFirmware_bom_t   *hf = (lx_hostFirmware_bom_t*)bom;
  printf("build: %lf version: %d\n",
	 hf->build, hf->version);
}

typedef struct { 
  float                   signal;
  uint32_t                tx;
  uint32_t                rx;   
  uint16_t                reserved;
} lx_wifiInfo_bom_t; 

void handle_getWifiInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}

void handle_stateWifiInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_wifiInfo_bom_t   *hs = (lx_wifiInfo_bom_t*)bom;
  printf("signal: %f tx: %d rx; %d\n",
	 hs->signal, hs->tx, hs->rx);
}

typedef struct { 
  double                  build;
  uint64_t                reserved;
  uint32_t                version;
} lx_wifiFirmware_bom_t; 

void handle_getWifiFirmware(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_stateWifiFirmware(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_wifiFirmware_bom_t   *hf = (lx_wifiFirmware_bom_t*)bom;
  printf("build: %lf version: %d\n",
	 hf->build, hf->version);
}

typedef struct {
  uint16_t                level;
} lx_power_bom_t;

void handle_getPower(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_setPower(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_statePower(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_power_bom_t   *hf = (lx_power_bom_t*)bom;
  printf("level: %d\n",
	 hf->level);
}

typedef struct {
  uint8_t                label[32];
} lx_label_bom_t;

void handle_getLabel(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_setLabel(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}

/* add label to the device */
void handle_stateLabel(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  ENTRY            e, *ep;
  int              success;

  lx_label_bom_t   *hf = (lx_label_bom_t*)bom;
  //  printf("label: %s\n", hf->label);
  /* lookup label */
  e.key = hf->label;
  e.data = (void*)NULL;
  success = hsearch_r(e, FIND, &ep, labtab);  
  /* device found */
  if (ep){
    printf("device found: %s %s %d\n", 
	   ((discoveredDevice*)ep->data)->label,
	   ((discoveredDevice*)ep->data)->ip,
	   ((discoveredDevice*)ep->data)->device_no);    
    char macstr[32];
    sprint_MACaddr(macstr, ((discoveredDevice*)ep->data)->mac);
    printf("\tMAC=%s\n", macstr);
  }  /* add device */
  else {
    discoveredDevice *dev = &discoveredDevices[device_no];
    strcpy(dev->label, hf->label);
    strcpy(dev->ip, ip);                                
    memcpy(dev->mac, header->target, sizeof(header->target));
    e.key = dev->label;
    e.data = (void *)dev;
    success = hsearch_r(e, ENTER, &ep, labtab);    
    /* there should be no failures */
    if (ep == NULL) fprintf(stderr, "entry failed\n");
    else dev->labelled = true;     // device is labelled.
  }
}

typedef struct {
  uint32_t                vendor;
  uint32_t                product;
  uint32_t                version;
} lx_version_bom_t;

void handle_getVersion(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_stateVersion(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_version_bom_t   *hf = (lx_version_bom_t*)bom;
  printf("vendor: %d product: %d version: %d\n",
	 hf->vendor, hf->product, hf->version);
}

typedef struct {
  uint64_t                time;
  uint64_t                uptime;
  uint64_t                downtime;
} lx_info_bom_t;

void handle_getInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_stateInfo(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_info_bom_t   *hf = (lx_info_bom_t*)bom;
  printf("time: %ld uptime: %ld downtime: %ld\n",
	 hf->time, hf->uptime, hf->downtime);
}
void handle_acknowledgement(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}

typedef struct {
  uint8_t                 location[16];
  uint8_t                 label[32];
  uint64_t                updated_at;
} lx_location_bom_t;

void handle_getLocation(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_stateLocation(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_location_bom_t   *hf = (lx_location_bom_t*)bom;
  printf("location: %s label: %s updated_at: %ld\n",
	 hf->location, hf->label, hf->updated_at);
}

typedef struct {
  uint8_t                 group[16];
  uint8_t                 label[32];
  uint64_t                updated_at;
} lx_group_bom_t;

void handle_getGroup(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}
void handle_stateGroup(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_group_bom_t   *hf = (lx_group_bom_t*)bom;
  printf("group: %s label: %s updated_at: %ld\n",
	 hf->group, hf->label, hf->updated_at);
}

typedef struct {
  uint8_t                 payload[64];
} lx_echo_bom_t;

void handle_echoRequest(pdu_header_t header, uint8_t *bom, char *ip, int device_no){}

void handle_echoResponse(pdu_header_t header, uint8_t *bom, char *ip, int device_no)
{
  lx_echo_bom_t   *hf = (lx_echo_bom_t*)bom;
  printf("payload: %s\n",
	 hf->payload);
}
