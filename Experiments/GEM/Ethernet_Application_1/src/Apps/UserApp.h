#ifndef _USER_APP_H_
   #define _USER_APP_H_

   #include "lwip/netif.h"
   #include "lwip/udp.h"
   #include "..\Ethernet\Ethernetif.h"
   #include "SysTimer.h"

   #define MAX_Interfaces_Allowed    4

   /* Comm Flag definitions */
   #define UDP_LINK_ACTIVE                0x01
   #define UDP_LINK_PRG                   0x02
   #define UDP_PACK_ACK_RCV               0x04
   #define UDP_PACK_ACK_PND               0x08

   typedef enum
   {
      APP_ERR_OK = 0,
      IP_NOT_COFIG = -1
   }UserAppErrs;

   typedef struct
   {
      char *ownMACAddr;
      struct netif *netif_main;
      struct netif netif[MAX_Interfaces_Allowed];
      struct udp_pcb* Cfg_pcb;
      ip_addr_t server_ip;
      ip_addr_t static_ip[MAX_Interfaces_Allowed];
      ip_addr_t netmask[MAX_Interfaces_Allowed];
      ip_addr_t gw[MAX_Interfaces_Allowed];
      u8 infno;
      u8 Maxinf;
      EMACPS_device_struct *DevPtr;
   }UserAppStruct;

   typedef struct
   {
      struct udp_pcb* udpPcb;
      ip_addr_t sndip;
      u16 sndport;
      ip_addr_t rcvip;
      u16 rcvport;
      u8 Comflags;
      TIMEOUTTIMER ComTimer;
      u8 *rcvptr;
      u16 *rcvlen;
      u8 *sndptr;
      u16 sndlen;
   }UserAppComStruct;

   extern UserAppStruct usrAppLowLwvelDriver;

   extern void App_init(void);
   extern void App_CommInit(void);
   extern void App_CheckConnection(void);
   extern void App_CheckCommunication(void);
#endif
