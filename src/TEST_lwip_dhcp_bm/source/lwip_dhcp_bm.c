/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lwip/opt.h"

#if LWIP_IPV4 && LWIP_DHCP

#include "lwip/timeouts.h"
#include "lwip/ip_addr.h"
#include "lwip/init.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_phy.h"

#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"



#include "compiletime.h"
#include "datetime.h"
#include "datetime.cpp"
#include "I2C.h"
#include "I2C.cpp"

#include "PCF85063AT.h"
#include "PCF85063AT.cpp"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 3
#define configIP_ADDR3 102

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 192
#define configGW_ADDR1 168
#define configGW_ADDR2 3
#define configGW_ADDR3 1

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

/* MDIO operations. */
#define EXAMPLE_MDIO_OPS enet_ops

/* PHY operations. */
#define EXAMPLE_PHY_OPS phyksz8081_ops

/* ENET clock frequency. */
#define EXAMPLE_CLOCK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)

#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static mdio_handle_t mdioHandle = {.ops = &EXAMPLE_MDIO_OPS};
static phy_handle_t phyHandle   = {.phyAddr = EXAMPLE_PHY_ADDRESS, .mdioHandle = &mdioHandle, .ops = &EXAMPLE_PHY_OPS};

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {
        .enableClkOutput = true, .enableClkOutput500M = false, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}




static struct udp_pcb *udpecho_raw_pcb;



#define NTP_PORT 123
#define MAXUINT32 4294967295.
static const int NTP_PACKET_SIZE = 48;
static const int NTP_CLIENT_PACKET_SIZE = 68;
#define UTC_NTP_OFFSET 2208988800U /* 1970 - 1900 */
static uint32_t ntp_timestamp = 1627420166;
static bool ntp_timestamp_valid = false;
//static


static void udpecho_raw_recv(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port){
  LWIP_UNUSED_ARG(arg);
  if (p != NULL && p->len >= NTP_CLIENT_PACKET_SIZE) {



	struct pbuf* ntp_packet = pbuf_alloc(PBUF_TRANSPORT, NTP_PACKET_SIZE, PBUF_RAM);
	//LI LI VER VER VER MODE MODE MODE

	if(ntp_timestamp_valid){
		pbuf_put_at(ntp_packet,0,0b00011100);// LI, Version, Mode
		pbuf_put_at(ntp_packet,1, 1);   // stratum (GPS)
	}else{
		pbuf_put_at(ntp_packet,0,0b11011100);// INVALID LI
		pbuf_put_at(ntp_packet,1, 16);   // INVALID stratum 16
	}


	pbuf_put_at(ntp_packet,2, 3);   // polling minimum (64 seconds - default)
	pbuf_put_at(ntp_packet,3,0xF7); // precision (2^-9 ~2 milliseconds)


	pbuf_put_at(ntp_packet,4, 0);    // root delay
	pbuf_put_at(ntp_packet,5, 0);
	pbuf_put_at(ntp_packet,6, 0); //8
	pbuf_put_at(ntp_packet,7, 0);

	pbuf_put_at(ntp_packet,8, 0);   // root dispersion
	pbuf_put_at(ntp_packet,9, 0);
	pbuf_put_at(ntp_packet,10, 0); //0xC
	pbuf_put_at(ntp_packet,11, 0);

	if(ntp_timestamp_valid){
		pbuf_put_at(ntp_packet,12, 'G');
		pbuf_put_at(ntp_packet,13, 'P');
		pbuf_put_at(ntp_packet,14, 'S');
		pbuf_put_at(ntp_packet,15, 0);
	}else{
		pbuf_put_at(ntp_packet,12, 'I');
		pbuf_put_at(ntp_packet,13, 'V');
		pbuf_put_at(ntp_packet,14, 'A');
		pbuf_put_at(ntp_packet,15, 'L');
	}
	// Reference timestamp
	uint32_t tempval;
	tempval = ntp_timestamp;
	pbuf_put_at(ntp_packet,16, (tempval >> 24) & 0XFF);
	tempval = ntp_timestamp;
	pbuf_put_at(ntp_packet,17, (tempval >> 16) & 0xFF);
	tempval = ntp_timestamp;
	pbuf_put_at(ntp_packet,18, (tempval >> 8) & 0xFF);
	tempval = ntp_timestamp;
	pbuf_put_at(ntp_packet,19, (tempval) & 0xFF);
	//Decimal Fractional Part
	pbuf_put_at(ntp_packet,20, 0);
	pbuf_put_at(ntp_packet,21, 0);
	pbuf_put_at(ntp_packet,22, 0);
	pbuf_put_at(ntp_packet,23, 0);

	// Originate timestamp from incoming UDP transmit timestamp
	pbuf_put_at(ntp_packet, 24, pbuf_try_get_at(p,40));
	pbuf_put_at(ntp_packet, 25, pbuf_try_get_at(p,41));
	pbuf_put_at(ntp_packet, 26, pbuf_try_get_at(p,42));
	pbuf_put_at(ntp_packet, 27, pbuf_try_get_at(p,43));
	pbuf_put_at(ntp_packet, 28, pbuf_try_get_at(p,44));
	pbuf_put_at(ntp_packet, 29, pbuf_try_get_at(p,45));
	pbuf_put_at(ntp_packet, 30, pbuf_try_get_at(p,46));
	pbuf_put_at(ntp_packet, 31, pbuf_try_get_at(p,47));
	// Receive timestamp
	tempval = ntp_timestamp;    // Same as reference timestamp
	pbuf_put_at(ntp_packet, 32, (tempval >> 24) & 0XFF);
	tempval = ntp_timestamp;
	pbuf_put_at(ntp_packet, 33, (tempval >> 16) & 0xFF);
	tempval = ntp_timestamp;
	pbuf_put_at(ntp_packet, 34, (tempval >> 8) & 0xFF);
	tempval = ntp_timestamp;
	pbuf_put_at(ntp_packet, 35, (tempval) & 0xFF);
	//Decimal Fractional Part
	pbuf_put_at(ntp_packet,36, 0);
	pbuf_put_at(ntp_packet,37, 0);
	pbuf_put_at(ntp_packet,38, 0);
	pbuf_put_at(ntp_packet,39, 0);

	// Transmit timestamp
	pbuf_put_at(ntp_packet, 40, (tempval >> 24) & 0XFF);
	tempval = ntp_timestamp;
	pbuf_put_at(ntp_packet, 41, (tempval >> 16) & 0xFF);
	tempval = ntp_timestamp;
	pbuf_put_at(ntp_packet, 42, (tempval >> 8) & 0xFF);
	tempval = ntp_timestamp;
	pbuf_put_at(ntp_packet, 43, (tempval) & 0xFF);
	//Decimal Fractional Part
	pbuf_put_at(ntp_packet,44, 0);
	pbuf_put_at(ntp_packet,45, 0);
	pbuf_put_at(ntp_packet,46, 0);
	pbuf_put_at(ntp_packet,47, 0);

	//SEND PACKET
    udp_sendto(upcb, ntp_packet, addr, port);
    //FREE PACKET BUFFERS
    pbuf_free(p);
    pbuf_free(ntp_packet);
  }
}


void udpecho_raw_init(void)
{
  udpecho_raw_pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
  if (udpecho_raw_pcb != NULL) {
    err_t err;
    //REGISTER UDP PACKET CALLBACK
    err = udp_bind(udpecho_raw_pcb, IP_ANY_TYPE, NTP_PORT);
    if (err == ERR_OK) {
      udp_recv(udpecho_raw_pcb, udpecho_raw_recv, NULL);
    } else {
      /* abort? output diagnostic? */
    }
  } else {
    /* abort? output diagnostic? */
  }
}




/*!
 * @brief Interrupt service for SysTick timer.
 */
void SysTick_Handler(void)
{
    time_isr();
}

/*!
 * @brief Prints DHCP status of the interface when it has changed from last status.
 *
 * @param netif network interface structure
 */
static bool print_dhcp_state(struct netif *netif)
{
	bool dhcp_bound = false;
    static u8_t dhcp_last_state = DHCP_STATE_OFF;
    struct dhcp *dhcp           = netif_dhcp_data(netif);

    if (dhcp == NULL)
    {
        dhcp_last_state = DHCP_STATE_OFF;
    }
    else if (dhcp_last_state != dhcp->state)
    {
        dhcp_last_state = dhcp->state;

        PRINTF(" DHCP state       : ");
        switch (dhcp_last_state)
        {
            case DHCP_STATE_OFF:
                PRINTF("OFF");
                break;
            case DHCP_STATE_REQUESTING:
                PRINTF("REQUESTING");
                break;
            case DHCP_STATE_INIT:
                PRINTF("INIT");
                break;
            case DHCP_STATE_REBOOTING:
                PRINTF("REBOOTING");
                break;
            case DHCP_STATE_REBINDING:
                PRINTF("REBINDING");
                break;
            case DHCP_STATE_RENEWING:
                PRINTF("RENEWING");
                break;
            case DHCP_STATE_SELECTING:
                PRINTF("SELECTING");
                break;
            case DHCP_STATE_INFORMING:
                PRINTF("INFORMING");
                break;
            case DHCP_STATE_CHECKING:
                PRINTF("CHECKING");
                break;
            case DHCP_STATE_BOUND:
                PRINTF("BOUND");
                dhcp_bound = true;
                break;
            case DHCP_STATE_BACKING_OFF:
                PRINTF("BACKING_OFF");
                break;
            default:
                PRINTF("%u", dhcp_last_state);
                assert(0);
                break;
        }
        PRINTF("\r\n");

        if (dhcp_last_state == DHCP_STATE_BOUND)
        {
            PRINTF("\r\n IPv4 Address     : %s\r\n", ipaddr_ntoa(&netif->ip_addr));
            PRINTF(" IPv4 Subnet mask : %s\r\n", ipaddr_ntoa(&netif->netmask));
            PRINTF(" IPv4 Gateway     : %s\r\n\r\n", ipaddr_ntoa(&netif->gw));
        }
    }
    return dhcp_bound;
}

/*!
 * @brief Main function.
 */
int main(void)
{
    struct netif netif;
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {
        .phyHandle  = &phyHandle,
        .macAddress = configMAC_ADDR,
    };

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();


    PRINTF("\r\n************************************************\r\n");
    PRINTF(" I2C SETUP\r\n");
    PRINTF("************************************************\r\n");
    i2c_init_alt();
    int found_i2c_dev = i2c_scan_alt();

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" GPIO CLOCK SETUP\r\n");
    PRINTF("************************************************\r\n");
    BOARD_InitModuleClock();
    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    GPIO_PinInit(GPIO1, 4, &gpio_config);
    GPIO_PinInit(GPIO1, 22, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 22, 1);
    GPIO_WritePinOutput(GPIO1, 4, 0);
    delay();
    GPIO_WritePinOutput(GPIO1, 4, 1);

    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;

    time_init();





    PRINTF("\r\n************************************************\r\n");
    PRINTF(" DATETIME SETUP\r\n");
    PRINTF("************************************************\r\n");
    datetime_t curr_dt;
    timestamp_t cur_ts = UNIX_TIMESTAMP;
    breakTime(cur_ts, &curr_dt);
    PRINTF(" COMPILE TIMESTAMP: %ld\r\n",cur_ts);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" PCF85063AT RTC SETUP\r\n");
    PRINTF("************************************************\r\n");
    PCF85063AT_init();
    if(PCF85063AT_reset()){
    	PRINTF(" PCF85063AT RESET OK\r\n");
    	PCF85063AT_stop(false);
    }else{
    	PRINTF("!! PCF85063AT RESET ERROR !!\r\n");
    }

    datetime_t rtc_dt;
    if(PCF85063AT_time_get(&rtc_dt)){
    	PRINTF("%02d-%02d-%d %02d:%02d:%02d\n", (1970 + rtc_dt.year), rtc_dt.month, rtc_dt.day, rtc_dt.hour, rtc_dt.minute, rtc_dt.second);

    	if(rtc_dt.year <= 0 && PCF85063AT_time_set(&curr_dt)){
    		PRINTF(" PCF85063AT_time_set SET TIME TO COMPILE TIME OK\r\n");
    	}else{
    		PRINTF("!! PCF85063AT_time_set ERROR !!\r\n");
    	}
    }






    PRINTF("\r\n************************************************\r\n");
    PRINTF(" LWIP SETUP\r\n");
    PRINTF("************************************************\r\n");

    IP4_ADDR(&netif_ipaddr, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_netmask, 0U, 0U, 0U, 0U);
    IP4_ADDR(&netif_gw, 0U, 0U, 0U, 0U);

    lwip_init();

    netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN, ethernet_input);
    netif_set_default(&netif);
    netif_set_up(&netif);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" DHCP SERVER SETUP\r\n");
    PRINTF("************************************************\r\n");

    dhcp_start(&netif);
    PRINTF(" WAIT FOR VALID IP FROM DHCP SERVER\r\n");
    int counter = 0;
    while(!print_dhcp_state(&netif)){
    	ethernetif_input(&netif);
    	sys_check_timeouts();
    	delay();
    	counter++;
    	if(counter > 5000){
    		PRINTF(" SKIP WAIT FOR DHCP SERVER\r\n");
    		PRINTF("FALLBACK IP: %i.%i.%i.%i\r\n",configIP_ADDR0,configIP_ADDR1,configIP_ADDR2,configIP_ADDR3);
    		break;
    	}
    }


    PRINTF("\r\n************************************************\r\n");
    PRINTF(" SETUP UDP SOCKET\r\n");
    PRINTF("************************************************\r\n");
    udpecho_raw_init();



    PRINTF("\r\n************************************************\r\n");
    PRINTF(" ENTER MAIN LOOP\r\n");
    PRINTF("************************************************\r\n");
    while (1)
    {
        /* Poll the driver, get any outstanding frames */
        ethernetif_input(&netif);

        /* Handle all system timeouts for all core protocols */
        sys_check_timeouts();


        print_dhcp_state(&netif);


        /* READ CURRENT DATE FROM THE RTC */
        datetime_t cur_dt;
        if (PCF85063AT_time_get(&cur_dt)) {
        	breakTime(ntp_timestamp, &cur_dt);
        	if(ntp_timestamp >= 0){
        		PRINTF("%02d-%02d-%d %02d:%02d:%02d %ld\n", (1970 + cur_dt.year), cur_dt.month, cur_dt.day, cur_dt.hour, cur_dt.minute, cur_dt.second, ntp_timestamp);
        		ntp_timestamp_valid = true;
        	}else{
        		ntp_timestamp_valid = false;
        		PRINTF("INVALID TIME.\n");
        	}


        } else {
        	//PRINTF("Could not get time.\n");
        	ntp_timestamp_valid = false;
        	//SEND COMPILE TIMESTAMP
        	ntp_timestamp = UNIX_TIMESTAMP+ UTC_NTP_OFFSET;
        }
    }
}
#endif
