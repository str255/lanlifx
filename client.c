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

int find_device(int doIP, int doLabel, int doMAC, char *ip, char *label, char *mac);
int first_unlabelled_device();
void print_labelled_devices();
void make_pdu(pdu_header_t mylifx, int tagged, int msgType, discoveredDevice *dev, int sequenceNumber);

char        usage[]="USAGE: lanlifx --help --segment [ip-address] --timeout [timeout]\n"
                    "\t[--ip|--label|--mac] --get [location|group|color]\n"
                    "--set [color|label|group|location] --value [specified-value]\n"
                    "\tlists all LIFX devices on LAN segment, then,\n"
                    "\tget/set action on [IP|label|MAC] selected device.\n";

int
  doIP = false,
  doLabel = false, 
  doMAC = false,
  num_device = 0,
  current_device = 0,
  num_discovered = 0,
  discovery_timeout = 1,        // seconds
  ibroadcast = true,
  doSet = false,
  doGet = false,
  doGetLocation = false,
  doGetGroup = false,
  doGetColor = false,
  doGetLabel = false,
  getReq;
char 
  choseIP[MAXLABEL],
  choseLabel[MAXLABEL], 
  choseMac[MAXLABEL],
  segment[16]="192.168.1.255",
  prop_option[32],
  value_option[80],
  buffer[BUFLEN]={'A'},          // write buffer
  readBuffer[BUFLEN]={'A'};      // read buffer
uint8_t     
sequenceNumber = 0;            // unique message sequence no
struct hsearch_data 
  *mactab,
  *labtab;
discoveredDevice   discoveredDevices[MAXDEVICES];
extern deviceMessage_t deviceMessages[maxDeviceMessages];
extern deviceMessage_t lightMessages[maxLightMessages];
extern bomMessageHandler_t  bomMessages[maxBomMessages];

void error(const char *msg)
{
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[])
{
  int 
    success,
    italk  = true,
    ilisten = true,
    sockfd,
    portno = 56700, 
    val = 1,
    errno,
    c,
    device_no;
  socklen_t  client_addr_len;
  struct sockaddr_in 
    serv_addr = {
    .sin_family = AF_INET,
    .sin_addr.s_addr = inet_addr(segment),// { INADDR_BROADCAST },
    .sin_port = htons(portno),
    .sin_zero = { 0 }
  },
    client_addr = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = inet_addr(segment),
      .sin_port =  htons(portno),
      .sin_zero = { 0 }
    },
      *an_addr;
      pdu_header_t    mylifx = (pdu_header_t)buffer;
      uint8_t        *bom;

      // argument processing
      while (1){
	static struct option long_options[] =
	  {
	    {"help", no_argument, 0, 'h'},             /* help. */
	    {"timeout", required_argument,   0, 'd'},/* timeout */
	    {"segment", required_argument,  0, 'u'},   /* seqment */
	    {"ip",     required_argument,   0, 'p'},   /* ip or */
	    {"label",  required_argument,   0, 'l'},   /* label or */
	    {"mac",    required_argument,   0, 'm'},   /* mac */
	    {"set", required_argument,   0, 's'},      /* set */	  	  
	    {"get",  required_argument, 0, 'g'},       /* get */
	    {"value",  required_argument, 0, 'v'},     /* value */
	    {0, 0, 0, 0}	                       /* end */
	  };
	/* getopt_long stores the option index here. */
	int option_index = 0;

	c = getopt_long (argc, argv, "hdu:p:l:m:s:g:", long_options, &option_index);

	/* Detect the end of the options. */
	if (c == -1)
	  break;

	switch (c){
	case 0:
	  /* If this option set a flag, do nothing else now. */
	  if (long_options[option_index].flag != 0)
	    break;
	  printf ("option %s", long_options[option_index].name);
	  if (optarg)
	    printf (" with arg %s", optarg);
	  printf ("\n");
	  break;

	  /* help option */
	case '?':
	case 'h':
	  printf("%s", usage);
	  exit(1);
	  break;

	  /* segment option */
	case 'u':
	  strcpy(segment, optarg);
	  printf("using %s as segment\n", segment);
	  break;
	
	  /* timeout option */
	case 'd':
	  discovery_timeout = atoi(optarg);
	  break;

	  /* ip option */
	case 'p':
	  doIP = true;
	  strcpy(choseIP, optarg);
	  break;

	  /* label option */
	case 'l':
	  doLabel = true;
	  strcpy(choseLabel, optarg);
	  break;

	  /* MAC option */
	case 'm':
	  doMAC = true;
	  strcpy(choseMac, optarg);
	  break;

	  // "set"
	case 's':
	  doSet = true;
	  strcpy(prop_option, optarg);
	  break;

	  // "get"
	case 'g':
	  doGet = true;
	  if (strcmp(optarg, "location")==0) doGetLocation = true;
	  else if (strcmp(optarg, "group")==0) doGetGroup = true;
	  else if (strcmp(optarg, "color")==0) doGetColor = true;
	  else if (strcmp(optarg, "label")==0) doGetLabel = true;			  
	  else doGet = false;
	  break;

	case 'v':
	  strcpy(value_option, optarg);
	  printf("value=%s\n", value_option);
	  break;
	default:
	  abort ();
	}
      }
      /* mac index */
      mactab=calloc(MAXDEVICES,sizeof(struct hsearch_data));
      success=hcreate_r(MAXDEVICES,mactab);
      if (!success)
	printf("hcreate_r: ran out of memory %d\n", errno);
      /* label index */
      labtab=calloc(MAXDEVICES,sizeof(struct hsearch_data));
      success=hcreate_r(MAXDEVICES,labtab);
      if (!success)
	printf("hcreate_r: ran out of memory %d\n", errno);

    
      /* open socket */
      sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if (sockfd < 0)  error("ERROR opening socket");

      /* broadcast socket */
      errno = setsockopt(
			 sockfd,
			 SOL_SOCKET,
			 SO_BROADCAST,
			 &val,
			 sizeof(val)
			 );
      if (errno) error("setsockopt");

      int option = 1;

      if(setsockopt(sockfd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char*)&option, sizeof(option)) < 0){
	printf("setsockopt failed\n");
	close(sockfd);
	exit(2);
      }

      /* bind socket to portno */
      errno = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
      if (errno) error("bind");

      // DISCOVER DEVICES
      make_pdu(mylifx, 1, getService, NULL, sequenceNumber++);

      while (italk && current_device >= 0){
	an_addr = (struct sockaddr_in*)&client_addr;	
	size_t nbytes=sendto(
			     sockfd, 
			     mylifx, 
			     mylifx->size,
			     0, 
			     (struct sockaddr*)an_addr,
			     sizeof(serv_addr));
	if (nbytes < 0) error("ERROR writing to socket");

	/* broadcast (tagged=1), listen for packet responses, 
	   discover devices, continue until max_discovery_time */
	ilisten = true;
	while (ilisten){
	  fd_set select_fds;           
	  struct timeval timeout;      

	  FD_ZERO(&select_fds);        
	  FD_SET(sockfd, &select_fds); 

	  timeout.tv_sec = discovery_timeout;  
	  timeout.tv_usec = 0;


	  if ( select(32, &select_fds, NULL, NULL, &timeout) == 0 ) {
	    ilisten = false;   // timed out, time to talk
	  }
	  else {
	    nbytes=recvfrom(sockfd, readBuffer, BUFLEN, 0, (struct sockaddr *)&client_addr, &client_addr_len);
	    if (nbytes<0) error("recvfrom()");

	    mylifx = (pdu_header_t)readBuffer;
	  
	    // do not handle my own packets.
	    if (mylifx->origin==0) continue;       

	    /* in discovery mode keep MAC and IP address. */
	    /* lookup by MAC  */
	    char        *IP = inet_ntoa(client_addr.sin_addr);
	    ENTRY        e, *ep;
	    char         macstr[32];

	    sprint_MACaddr(macstr, mylifx->target);
	    e.key = macstr;
	    e.data = (void*)NULL;
	    success = hsearch_r(e, FIND, &ep, mactab);  
	    /* device found */
	    if (ep){
	      uint8_t *mac = ((discoveredDevice*)ep->data)->mac;
	      device_no = ((discoveredDevice*)ep->data)->device_no;
	    }  /* add device */
	    else {
	      discoveredDevice *dev = &discoveredDevices[num_device];
	      dev->device_no = num_device;
	      dev->labelled = false;
	      strcpy(dev->ip, IP);
	      memcpy(dev->mac, mylifx->target, sizeof(mylifx->target));
	      e.key = macstr;
	      e.data = (void *)dev;
	      device_no = num_device;
	      num_device++;
	      success = hsearch_r(e, ENTER, &ep, mactab);    
	      /* there should be no failures */
	      if (ep == NULL) fprintf(stderr, "entry failed\n");
	    }
	    bom = ((pdu_t)readBuffer)->bom;
	    int itype;
	    // handle device messages
	    for (itype=0;itype<maxDeviceMessages;itype++){
	      if (deviceMessages[itype].itype==mylifx->type){
		(deviceMessages[itype].handler)(mylifx, bom, inet_ntoa(client_addr.sin_addr), device_no);
		break;
	      }
	    }
	  }
	}// end ilisten
	// GET LABELS
	current_device = first_unlabelled_device();
	if (current_device >= 0)
	  make_pdu(mylifx, 0, getLabel, &discoveredDevices[current_device], sequenceNumber++);
      }// end italk

      print_labelled_devices();
      // SELECT DEVICE FOR GET/SET
      if (!(doIP || doLabel || doMAC)){
	close(sockfd);
	exit(0);
      }
      current_device = find_device(doIP, doLabel, doMAC, choseIP, choseLabel, choseMac);
      printf("current_device=%d\n", current_device);

      // GET COLOR/GROUP/LOCATION/LABEL
      if (doGet){
	mylifx = (pdu_header_t)buffer;      
	bom = ((pdu_t)buffer)->bom;
	if (doGetLocation) getReq = getLocation;
	else if (doGetGroup) getReq = getGroup; 
	else if (doGetColor) getReq = light_getColor;
	else if (doGetLabel) getReq = getLabel;      
	make_pdu(mylifx, 0, getReq, &discoveredDevices[current_device], sequenceNumber++);      
	size_t nbytes=sendto(
			     sockfd, 
			     mylifx, 
			     mylifx->size+16,
			     0, 
			     (struct sockaddr*)an_addr,
			     sizeof(serv_addr));
	if (nbytes < 0) error("ERROR writing to socket");
	
	ilisten = true;
	while (ilisten){
	  fd_set select_fds;           
	  struct timeval timeout;      

	  FD_ZERO(&select_fds);        
	  FD_SET(sockfd, &select_fds); 

	  timeout.tv_sec = discovery_timeout;  
	  timeout.tv_usec = 0;
	
	  if ( select(32, &select_fds, NULL, NULL, &timeout) == 0 ) {
	    ilisten = false;   // timed out, time to talk
	  }
	  else {

	    nbytes=recvfrom(sockfd, readBuffer, BUFLEN, 0, (struct sockaddr *)&client_addr, &client_addr_len);
	    if (nbytes<0) error("recvfrom()");

	    mylifx = (pdu_header_t)readBuffer;

	    // do not handle my own packets.
	    if (mylifx->origin==0) continue;       

	    /* lookup by MAC  */
	    char        *IP = inet_ntoa(client_addr.sin_addr);
	    ENTRY        e, *ep;
	    char         macstr[32];

	    sprint_MACaddr(macstr, mylifx->target);
	    e.key = macstr;
	    e.data = (void*)NULL;
	    success = hsearch_r(e, FIND, &ep, mactab);  
	    /* device found */
	    if (ep){
	      uint8_t *mac = ((discoveredDevice*)ep->data)->mac;
	      device_no = ((discoveredDevice*)ep->data)->device_no;
	    }
	    else printf("error ep not found\n");

	    bom = ((pdu_t)readBuffer)->bom;
	    int itype;
	    // handle device messages
	    for (itype=0;itype<maxDeviceMessages;itype++){
	      if (deviceMessages[itype].itype==mylifx->type){
		(deviceMessages[itype].handler)(mylifx, bom, inet_ntoa(client_addr.sin_addr), device_no);
		break;
	      }
	    }
	    // handle light messages
	    for (itype=0;itype<maxLightMessages;itype++){
	      if (lightMessages[itype].itype==mylifx->type){
		(lightMessages[itype].handler)(mylifx, bom, inet_ntoa(client_addr.sin_addr), device_no);
		break;
	      }
	    }
	  }
	}// end ilisten
      }//doGet
      // SEND MESSAGE
      if (doSet == true){
	if (current_device < 0) error("doSet current device %d not found\n");
	int itype;
	mylifx = (pdu_header_t)buffer;      
	bom = ((pdu_t)buffer)->bom;
	for (itype=0;itype<maxBomMessages;itype++){
	  if (strcmp(bomMessages[itype].str,prop_option)==0){
	    getReq = bomMessages[itype].itype;
	    break;
	  }
	}
	make_pdu(mylifx, 0, getReq, &discoveredDevices[current_device], sequenceNumber++);      
	(bomMessages[itype].handler)(bom, value_option);
	size_t nbytes=sendto(
		      sockfd, 
		      mylifx, 
		      mylifx->size+16,
		      0, 
		      (struct sockaddr*)an_addr,
		      sizeof(serv_addr));
	if (nbytes < 0) error("ERROR writing to socket");
      }


      close(sockfd);
      return 0;
}

void
make_pdu(pdu_header_t mylifx, int tagged, int msgType, discoveredDevice *dev, int sequenceNumber)
{
  memset(mylifx, 0, BUFLEN);                  /* broadcast discovery */      
  mylifx->size       = sizeof(lx_protocol_header_t);
  mylifx->origin     = 0;
  mylifx->tagged     = 1;                  /* discovery */
  mylifx->protocol   = 1024;
  mylifx->addressable = 1;
  mylifx->source      = 0;                  /* uniq source */
  /* frame address */
  mylifx->res_required = 1;
  mylifx->ack_required = 1;
  mylifx->tagged = tagged;             /* not discovery */
  /* frame address */
  if (dev) memcpy(mylifx->target, dev->mac , 8);  
  mylifx->sequence = sequenceNumber;
  mylifx->type = msgType;      
}

int
find_device(int doIP, int doLabel, int doMAC, char *ip, char *label, char *mac)
{
  int               idev;
  char              devmac[16];

  discoveredDevice *dev;
  for (idev=0;idev<num_device;idev++){
    dev = &discoveredDevices[idev];
    sprint_MACaddr(devmac, dev->mac);
    if (doIP && strcmp(dev->ip, ip)== 0) return idev;
    if (doMAC && strcmp(devmac, mac)== 0) return idev;
    if (dev->labelled && doLabel && strcmp(dev->label, label)== 0) return idev;        
  }
  return -1;
}

int
first_unlabelled_device()
{
  int               idev;
  discoveredDevice *dev;
  for (idev=0;idev<num_device;idev++){
    dev = &discoveredDevices[idev];    
    if (dev->labelled == false) return idev;
  }
  return -1;
}

void
print_labelled_devices()
{
  int
    labelled_devices = 0,
    idev;
  discoveredDevice *dev;
  for (idev=0;idev<num_device;idev++){
    dev = &discoveredDevices[idev];    
    if (dev->labelled == true){
      labelled_devices++;
      uint8_t *mac = dev->mac;
      printf("*****************************\n");
      printf("device %d\n", idev);
      printf("\tip %s\n", dev->ip);
      printf("\tlabel %s\n", dev->label);      
      printf("\tMAC %x:%x:%x:%x:%x:%x:%x:%x\n",  
	     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6], mac[7]);
      printf("*****************************\n");      
    }
  }
  printf("total devices %d labeled %d\n", 
	 num_device, 
	 labelled_devices);
}
