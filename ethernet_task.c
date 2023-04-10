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
char    thisIp_char[IP_MAX_SIZE]   = "192.168.5.23";
char    targetIp_char[IP_MAX_SIZE] = "192.168.5.12";

uint8_t response_mac[6];
IPAddress   hThisIp;
IPAddress   hTargetIp;

EthernetUDP hEthUdp;

extern Semaphore_Handle sem_radioInit;
extern Semaphore_Handle sem_ethernetInit;
extern Semaphore_Handle sem_ready2send;
volatile bool bReadyToSend;

void intToStr (uint8_t* str, uint8_t len, uint8_t val);


void* Ethernet_mainTask(UArg a0, UArg a1)
{
    Semaphore_pend(sem_radioInit, BIOS_WAIT_FOREVER);


    System_printf("ETH START\n");
    IPAddress_fromString((IPAddress*)&hThisIp, thisIp_char);
    IPAddress_fromString((IPAddress*)&hTargetIp, targetIp_char);
    Ethernet_begin_mac_ip(thisMac_byte, hThisIp);
    Semaphore_post(sem_ethernetInit);


    EthernetUDP_begin_init(&hEthUdp);
    EthernetUDP_begin(&hEthUdp, PORT);

    bReadyToSend = true;

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
    uint16_t packetlen = RadioQueue_takePacket(buffer, maxlen);
    if (packetlen)
    {
        EthernetUDP_beginPacket_ip(&hEthUdp, hTargetIp, PORT);
        EthernetUDP_write(&hEthUdp, (uint8_t*)buffer, packetlen);
        EthernetUDP_endPacket(&hEthUdp);
    }
}

void Ethernet_setTargetIp(char* newTargetIp)
{
    bReadyToSend = false;
    EthernetUDP_stop(&hEthUdp);
    IPAddress_fromString((IPAddress*)&hTargetIp, newTargetIp);
    memset(targetIp_char, 0, IP_MAX_SIZE);
    // TODO: copies a portion of "HTTP" as well!
    strcpy(targetIp_char, newTargetIp);
    EthernetUDP_begin(&hEthUdp, PORT);
    bReadyToSend = true;
}

char* Ethernet_getTargetIp()
{
    return targetIp_char;
}

char* Ethernet_getThisIp()
{
    return thisIp_char;
}

char* Ethernet_getThisMac()
{
    sprintf(thisMac_char, "%x-%x-%x-%x-%x-%x",
            thisMac_byte[0], thisMac_byte[1], thisMac_byte[2],
            thisMac_byte[3], thisMac_byte[4], thisMac_byte[5]);
    return thisMac_char;
}




