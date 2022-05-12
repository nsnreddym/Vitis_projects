#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xemacps_hw.h"

#include "lwip/netif.h"
#include "lwip/ip.h"

#include "Ethernet\Ethernetif.h"
#include "Ethernet\EthernetHW.h"

const char MAC_ADDRESS[] = {0x00, 0x0a, 0x35, 0x01, 0x02, 0x03};
const char MAC_ADDRESS_broadcast[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

int main(void)
{
   ip_addr_t ipaddr, netmask, gw;
   struct netif Eth_netif;

   print("Entered Main loop\r\n");

   lwip_init();


   IP4_ADDR(&gw, 192,168,0,1);
   IP4_ADDR(&ipaddr, 192,168,1,10);
   IP4_ADDR(&netmask, 255,255,255,0);


	EMAC_add(&Eth_netif, &ipaddr, &netmask, &gw, MAC_ADDRESS);

	netif_set_default(&Eth_netif);

	netif_set_up(&Eth_netif);

	//EMACps_datasend(EMACPS_device_ptr);

	/*xemaps_init();

	xemacps_test();*/

	printf("Exit Main loop\r\n");

	while(1)
	{
	   EMAC_Read(&Eth_netif);
	}
}
