#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xemacps_hw.h"
#include "Xil_types.h"
#include "string.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "netif/ethernet.h"
#include "lwip/ip.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"

#include "..\Ethernet\Ethernetif.h"
#include "..\GPIO\Gpio.h"
#include "UserApp.h"
#include "SysTimer.h"
#include "Commands.h"

const char Cnst_ownMACAdd[] = {0x00, 0x0a, 0x35, 0x01, 0x02, 0x03};

UserAppStruct usrAppLowLwvelDriver;
UserAppComStruct usrAppCom;

u8 App_TXbuf[1024];
u16 App_TXlen;
u8 App_RXbuf[10];
u16 App_RXlen;

static void App_CommReset(void);
static UserAppErrs App_CfgRead(void);
static void udp_receive_callback(void* arg,
                                  struct udp_pcb* upcb,
                                  struct pbuf* p,
                                  const ip_addr_t* addr,
                                  u16_t port );


void App_init(void)
{
   print("App_init: User Application Initialization\n");

   /* Initialize LWIP */
   lwip_init();

   /* Read Config data */
   if(App_CfgRead() != APP_ERR_OK)
   {
      /* wait infinite */
      while(1);
   }

   netif_set_default(usrAppLowLwvelDriver.netif_main);

   //netif_set_up(usrAppLowLwvelDriver.netif_main);

}/* End of App_init */

void App_CommInit(void)
{
   /* Reset Communication */
   App_CommReset();

   /* Create New pcb */
   usrAppCom.udpPcb = udp_new();
   if(usrAppCom.udpPcb == NULL)
   {
      printf("Unable to Create UDP communication\n");
      while(1);
   }

   usrAppCom.sndip = usrAppLowLwvelDriver.netif_main->ip_addr;
   usrAppCom.sndport = 100;
   usrAppCom.rcvip = usrAppLowLwvelDriver.server_ip;
   usrAppCom.rcvport = 100;
   usrAppCom.rcvptr = App_RXbuf;
   usrAppCom.rcvlen = &App_RXlen;

   /* Bind packet */
   udp_bind(usrAppCom.udpPcb, &usrAppCom.sndip, usrAppCom.sndport) ;

   /* Connect to server */
   udp_connect(usrAppCom.udpPcb, &usrAppCom.rcvip, usrAppCom.rcvport);

   /* Receive call back update */
   udp_recv( usrAppCom.udpPcb, udp_receive_callback, &usrAppCom ) ;

   /* Set link progress */
   usrAppCom.Comflags = usrAppCom.Comflags | UDP_LINK_PRG;

}/* End of App_CommInit */

void App_CheckConnection(void)
{
   u8 Linkstatus;

   /* Check for status change */
   if(PHYLinkStatusChanged || !(usrAppLowLwvelDriver.netif_main->flags & NETIF_FLAG_UP) )
   {
      /* Clear interrupt generated ny PHY by reading status reg */
      Linkstatus = EthernetLinkStatus(usrAppLowLwvelDriver.DevPtr);

      /* Clear flag */
      PHYLinkStatusChanged = FALSE;

      if(FALSE == Linkstatus)
      {
         printf("PHY: Link Down\r\n");
         netif_set_down(usrAppLowLwvelDriver.netif_main);

      }
      else
      {
         printf("PHY: Link Up\r\n");
         netif_set_up(usrAppLowLwvelDriver.netif_main);
      }
   }
}/* End of App_CheckConnection */

void App_CheckCommunication(void)
{
   struct pbuf *p;
   u16 len;

   /* Check for network link */
   if(usrAppLowLwvelDriver.netif_main->flags & NETIF_FLAG_UP)
   {
      if(usrAppCom.Comflags == 0) //Comming from Reset
      {
         App_CommInit();
         printf("COMM: Communication Initiated\n");
      }
      else if(usrAppCom.Comflags & UDP_PACK_ACK_PND) //Data send ACK pending
      {
         printf("COMM: Ack pending\n");
      }
      else if(usrAppCom.Comflags & UDP_PACK_ACK_RCV) //Data ACK received
      {
         printf("COMM: Ack received\n");
         CMD_process(&usrAppCom);

      }
      else if((usrAppCom.Comflags & UDP_LINK_ACTIVE) || (usrAppCom.Comflags & UDP_LINK_PRG))
      {
         /* Check for timer expire */
         if(!(TimerIsExpired(&usrAppCom.ComTimer) == TIMER_NOTEXPIRED))
         {
            /* Start timer */
            TimerCreate(&usrAppCom.ComTimer,1000);

            if(usrAppCom.Comflags & UDP_LINK_ACTIVE) //ACK processed waiting to send data
            {
               /* Prepare Command */
               len = CMD_prepare(DATA_SND, App_TXbuf);

               printf("COMM: UDP data Sending...");
            }
            else if(usrAppCom.Comflags & UDP_LINK_PRG)//Link under progress
            {
               /* Prepare Command */
               len = CMD_prepare(LINK_REQ, App_TXbuf);

               printf("COMM: UDP Link Sending...");
            }

            /* Allocate pbuf */
            p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
            if (p == NULL)
            {
               printf("\n");
               printf("UserApp: could not allocate pbuf for udp link.\n");
               return;
            }

            /* Copy data */
            memcpy(p->payload, App_TXbuf, len);

            p->next = NULL;
            p->len = len;
            p->tot_len = len;

            /* Send udp link packet */
            udp_send(usrAppCom.udpPcb, p);

            printf("Sent\n");

            /* Free pbuf */
            pbuf_free(p);

         }
      }
   }
   else if((usrAppCom.Comflags & UDP_LINK_ACTIVE) || (usrAppCom.Comflags & UDP_LINK_PRG))
   {
      App_CommReset();
      printf("COMM: Communication Reset\n");
   }

}/* End of App_CheckCommunication */

static void App_CommReset(void)
{
   /* Check for udp structure */
   if(usrAppCom.udpPcb != NULL)
   {
      udp_remove(usrAppCom.udpPcb);
   }

   usrAppCom.Comflags = 0;
   usrAppCom.ComTimer.state = TIMER_NOT_RUNNING;

}/* End of App_CommReset */

static UserAppErrs App_CfgRead(void)
{
   /* Initialize device */
   usrAppLowLwvelDriver.DevPtr = &EMACPS_device;

   /* Read from CFG flash */

   /* Update IP Address */
   IP4_ADDR(&usrAppLowLwvelDriver.server_ip, 192,168,1,2);
   usrAppLowLwvelDriver.Maxinf = 1;
   for(int i = 0; i < usrAppLowLwvelDriver.Maxinf; i++)
   {
      IP4_ADDR(&usrAppLowLwvelDriver.gw[i], 192,168,1,1);
      IP4_ADDR(&usrAppLowLwvelDriver.static_ip[0], 192,168,1,10);
      IP4_ADDR(&usrAppLowLwvelDriver.netmask[i], 255,255,255,0);

      usrAppLowLwvelDriver.netif[i].hwaddr_len = 6;
      for (int j = 0; j < 6; j++)
      {
         usrAppLowLwvelDriver.netif->hwaddr[j] = Cnst_ownMACAdd[j];
      }
      usrAppLowLwvelDriver.netif[i].name[0] = 'E';
      usrAppLowLwvelDriver.netif[i].name[1] = '0' + i;

      netif_add(&usrAppLowLwvelDriver.netif[i],
                &usrAppLowLwvelDriver.static_ip[i],
                &usrAppLowLwvelDriver.netmask[i],
                &usrAppLowLwvelDriver.gw[i],
                (void*)usrAppLowLwvelDriver.DevPtr,
                EMACps_init,
                ethernet_input);
   }

   usrAppLowLwvelDriver.netif_main = &usrAppLowLwvelDriver.netif[0];

   return APP_ERR_OK;
}

static void udp_receive_callback(void* arg,              // User argument - udp_recv `arg` parameter
                                  struct udp_pcb* upcb,   // Receiving Protocol Control Block
                                  struct pbuf* p,         // Pointer to Datagram
                                  const ip_addr_t* addr,  // Address of sender
                                  u16_t port )    // Sender port
{
   UserAppComStruct *myAppStruct;
   struct udp_pcb* mypcb;

   myAppStruct = (UserAppComStruct *)arg;
   mypcb = myAppStruct->udpPcb;

   /* Check for server */
   if(ip4_addr_cmp(&mypcb->remote_ip ,addr))
   {
      /* Copy datagram */
      for(int i = 0; i < p->len; i++)
      {
         myAppStruct->rcvptr[i] = ((u8 *)p->payload)[i];
      }

      *myAppStruct->rcvlen = p->len;

      myAppStruct->Comflags |= UDP_PACK_ACK_RCV;

      myAppStruct->Comflags &= ~UDP_PACK_ACK_PND;
   }

   pbuf_free(p);
}

