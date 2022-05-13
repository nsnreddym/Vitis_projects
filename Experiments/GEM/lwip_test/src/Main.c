#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xemacps_hw.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/udp.h"

#include "Ethernet\Ethernetif.h"
#include "Ethernet\EthernetHW.h"

const char MAC_ADDRESS[] = {0x00, 0x0a, 0x35, 0x01, 0x02, 0x03};
const char MAC_ADDRESS_broadcast[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

void udp_receive_callback( void* arg,              // User argument - udp_recv `arg` parameter
                           struct udp_pcb* upcb,   // Receiving Protocol Control Block
                           struct pbuf* p,         // Pointer to Datagram
                           const ip_addr_t* addr,  // Address of sender
                           u16_t port )            // Sender port
{

  // Process datagram here (non-blocking code)
  printf("Data received on Port no :%d\n",port) ;
  for(int i =0; i< p->len; i++)
  {
     printf("%02x ", ((u8 *)p->payload)[i]);
  }
  printf("\n");

  udp_sendto(upcb,p,addr,port);

  // Must free receive pbuf before return
  pbuf_free(p);
}

int main(void)
{
   ip_addr_t ipaddr, netmask, gw;
   struct netif Eth_netif;
   struct udp_pcb* udp_port;

   print("Entered Main loop\r\n");

   lwip_init();


   IP4_ADDR(&gw, 192,168,0,1);
   IP4_ADDR(&ipaddr, 192,168,1,10);
   IP4_ADDR(&netmask, 255,255,255,0);


	EMAC_add(&Eth_netif, &ipaddr, &netmask, &gw, MAC_ADDRESS);

	udp_port = udp_new();
	udp_bind( udp_port, &ipaddr, 100 ) ;
   udp_recv( udp_port, udp_receive_callback, udp_port ) ;

   netif_set_default(&Eth_netif);

   netif_set_up(&Eth_netif);


	printf("Exit Main loop\r\n");

	while(1)
	{
	   EMAC_Read(&Eth_netif);
	}
}
