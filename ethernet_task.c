/*
 * ethernet_task.c
 *
 *  Created on: 21. 11. 2022
 *      Author: vojtechlukas
 */

#include <stdio.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/runtime/System.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include <ti/sysbios/BIOS.h>

#include "ti_radio_config.h"
#include "radio_task.h"
#include "ethernet_task.h"
#include "radio_queue.h"
#include "data_packet.h"
#include "ethernet/Ethernet.h"
#include "ethernet/EthernetUdp.h"


#define IP_MAX_SIZE  17
#define MAC_MAX_SIZE 18
#define PORT         2014

uint8_t thisMac_byte[]    = {0x00, 0x00, 0x00, 0xA1, 0xB2, 0xC3};
char    thisMac_char[MAC_MAX_SIZE];

char    pThisIp[IP_MAX_SIZE]   = "192.168.5.23";
char    pTargetIp[IP_MAX_SIZE] = "192.168.5.12";
char    pGatewayIp[IP_MAX_SIZE];
char    pMask[IP_MAX_SIZE];

uint8_t response_mac[6];

IPAddress   hThisIp;
IPAddress   hTargetIp;

EthernetUDP hEthUdp;

extern Semaphore_Handle sem_radioInit;
extern Semaphore_Handle sem_ethernetInit;
extern Semaphore_Handle sem_ready2send;
volatile bool bReadyToSend;
static bool bUsingDhcp;



void* Ethernet_mainTask(UArg a0, UArg a1)
{
    Semaphore_pend(sem_radioInit, BIOS_WAIT_FOREVER);


    System_printf("ETH START\n");
    //IPAddress_fromString((IPAddress*)&hThisIp, pThisIp);
    //IPAddress_fromString((IPAddress*)&hTargetIp, pTargetIp);
    bUsingDhcp = true;
    Ethernet_begin_mac(thisMac_byte);
    //Ethernet_begin_mac_ip(thisMac_byte, hThisIp);
    Semaphore_post(sem_ethernetInit);


    EthernetUDP_begin_init(&hEthUdp);
    EthernetUDP_begin(&hEthUdp, PORT);

    bReadyToSend = false;

    for (;;)
    {
        if (bReadyToSend) Ethernet_handleRFpacket();
        Task_sleep(1);
    }
}

void Ethernet_handleRFpacket()
{
    const uint16_t maxlen = 2047;
    uint8_t buffer[maxlen];
    const uint8_t accesAddress[] = {0xD6, 0xBE, 0x89, 0x8E};
    uint8_t proto = Radio_getCurrentProtocol();
    uint16_t packetlen = RadioQueue_takePacket(buffer, maxlen);
    if (packetlen && (hTargetIp.dword != 0))
    {
        EthernetUDP_beginPacket_ip(&hEthUdp, hTargetIp, PORT);
        EthernetUDP_write(&hEthUdp, &proto, 1);
        if (proto == BLE_1M)
            EthernetUDP_write(&hEthUdp, (uint8_t*)accesAddress, 4);
        EthernetUDP_write(&hEthUdp, (uint8_t*)buffer, packetlen);
        EthernetUDP_endPacket(&hEthUdp);
    }
}

/*
 * Sets target IP address in a following process:
 * 1. Disables sniffing and UDP sender
 * 2. Copies new target IP address to its global var
 * 3. Starts UDP sender and sniffing
 */
void Ethernet_setTargetIp(char* newTargetIp)
{
    bReadyToSend = false;
    EthernetUDP_stop(&hEthUdp);
    IPAddress_fromString((IPAddress*)&hTargetIp, newTargetIp);
    memset(pTargetIp, 0, IP_MAX_SIZE);
    strcpy(pTargetIp, newTargetIp);
    EthernetUDP_begin(&hEthUdp, PORT);
    bReadyToSend = true;
}


/*
 * Returns pointer to global variable
 * storing target IP address as a string
 */
char* Ethernet_getTargetIp()
{
    return pTargetIp;
}


/*
 * Returns pointer to global variable
 * storing this IP address as a string
 */
char* Ethernet_getThisIp()
{
    hThisIp = Ethernet_localIP();
    IPAddress_toString(hThisIp, pThisIp);
    return pThisIp;
}

/*
 * Sets this IP address
 *
 * @param pIp char*
 *     pointer to a string containing new IP
 */
void Ethernet_setThisIp(char* pIp)
{
    bUsingDhcp = false;

    IPAddress tmpIp;
    IPAddress_fromString(&tmpIp, pIp);
    W5500_setIPAddress(tmpIp.bytes);
}

/*
 * Returns pointer to global variable
 * storing this MAC address.
 */
char* Ethernet_getThisMac()
{
    sprintf(thisMac_char, "%x-%x-%x-%x-%x-%x",
            thisMac_byte[0], thisMac_byte[1], thisMac_byte[2],
            thisMac_byte[3], thisMac_byte[4], thisMac_byte[5]);
    return thisMac_char;
}


/*
 * Returns pointer to Gateway IP string
 */
char* Ethernet_getGatewayIp()
{
    IPAddress tmpIp;
    W5500_getGatewayIp(tmpIp.bytes);
    IPAddress_toString(tmpIp, pGatewayIp);
    return pGatewayIp;
}

/*
 * Sets Gateway IP address
 * (using W5500 driver directly)
 *
 * @param pIp char*
 *    pointer to IP string
 */
void Ethernet_setGatewayIp(char* pIp)
{
    IPAddress tmpIp;
    IPAddress_fromString(&tmpIp, pIp);
    W5500_setGatewayIp(tmpIp.bytes);
}

/*
 * Returns pointer to Subnet Mask string
 */
char* Ethernet_getMask()
{
    IPAddress tmpMsk;
    W5500_getSubnetMask(tmpMsk.bytes);
    IPAddress_toString(tmpMsk, pMask);
    return pMask;
}

/*
 * Sets Subnet Mask
 * (using W5500 driver directly)
 *
 * @param pMsk char*
 *     pointer to Mask string
 */
void Ethernet_setMask(char* pMsk)
{
    IPAddress tmpMsk;
    IPAddress_fromString(&tmpMsk, pMsk);
    W5500_setSubnetMask(tmpMsk.bytes);
}

/*
 * Command to start sniffing process
 * (Does not alter RF Core, just starts
 * sending frames from queue to target)
 */
void Ethernet_startSniffing()
{
    bReadyToSend = true;
}

/*
 * Command to stop sniffing process
 * (Does not alter RF Core, just stops
 * sending frames from queue to target)
 */
void Ethernet_stopSniffing()
{
    bReadyToSend = false;
}

/*
 * Returns TRUE if sniffing is enabled,
 * FALSE if otherwise.
 *
 */
bool Ethernet_getSniffingStatus()
{
    return bReadyToSend;
}

/*
 * Starts Ethernet session over
 * using DHCP. Also initializesW5500.
 */
void Ethernet_startDhcp()
{
    bUsingDhcp = true;
    Ethernet_begin_mac(thisMac_byte);
}

/*
 * Return true if using DHCP, false
 * otherwise
 */
bool Ethernet_getDhcpBool()
{
    return bUsingDhcp;
}





