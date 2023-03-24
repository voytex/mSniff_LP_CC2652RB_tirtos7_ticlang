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

#define PORT 2014

uint8_t my_mac[] = {0x00, 0x00, 0x00, 0xA1, 0xB2, 0xC3};
uint8_t response_mac[6];
const char ip_bytes[] = "192.168.5.23\0";
const char remote_ip_bytes[] = "192.168.5.12\0";

volatile IPAddress thisIp;
volatile IPAddress remoteIp;

EthernetUDP eth;

extern Semaphore_Handle sem_radioInit;
extern Semaphore_Handle sem_ethernetInit;
extern Semaphore_Handle sem_ready2send;
volatile bool ready2send;

void intToStr (uint8_t* str, uint8_t len, uint8_t val);


void* Ethernet_mainTask(UArg a0, UArg a1)
{
    Semaphore_pend(sem_radioInit, BIOS_WAIT_FOREVER);


    System_printf("ETH START\n");
    IPAddress_fromString((IPAddress*)&thisIp, ip_bytes);
    IPAddress_fromString((IPAddress*)&remoteIp, remote_ip_bytes);
    Ethernet_begin_mac_ip(my_mac, thisIp);

    Semaphore_post(sem_ethernetInit);


    EthernetUDP_begin_init(&eth);
    EthernetUDP_begin(&eth, PORT);

    ready2send = true;

    for (;;)
    {
        if (ready2send) Ethernet_handleRFpacket();
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
        EthernetUDP_beginPacket_ip(&eth, remoteIp, PORT);
        EthernetUDP_write(&eth, (uint8_t*)buffer, packetlen);
        EthernetUDP_endPacket(&eth);
    }
}

void Ethernet_setRemoteIp(char* newRemoteIp) {
    ready2send = false;
    EthernetUDP_stop(&eth);
    IPAddress_fromString((IPAddress*)&remoteIp, newRemoteIp);
    EthernetUDP_begin(&eth, PORT);
    ready2send = true;
}

void Ethernet_getRemoteIp(char* rIp) {
    uint8_t temp_ip[4][3];
    uint8_t i;
    for (i = 0; i < 4; ++i) {
        intToStr(temp_ip[i], 3, remoteIp.bytes[i]);
    }
    //asprintf(&rIp, "%d.%d.%d.%d", remoteIp.bytes[0], remoteIp.bytes[1], remoteIp.bytes[2], remoteIp.bytes[3]);
    //itoa(remoteIp.bytes[0], rIp, 3);
}

void intToStr (uint8_t* str, uint8_t len, uint8_t val) {
  uint8_t i;
  for(i=1; i<=len; i++)
  {
    str[len-i] = (uint8_t) ((val % 10UL) + '0');
    val /= 10;
  }

  str[i-1] = '\0';
}


