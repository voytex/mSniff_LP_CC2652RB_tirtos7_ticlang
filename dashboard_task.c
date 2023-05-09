/*
 * dashboard_task.c
 *
 *  Created on: 25. 2. 2023
 *      Author: vojtechlukas
 */
#include <ti/drivers/rf/RF.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <stdlib.h>
#include <string.h>

#include "ethernet/Ethernet.h"
#include "ethernet/EthernetServer.h"
#include "ethernet_task.h"
#include "radio_task.h"
#include "dashboard_task.h"
#include "html.h"


#define PORT 80
#define INPUT_BUFFER_SIZE 128
#define READ_BUFFER_SIZE  32

//uint8_t my_mac[] = {0x00, 0x00, 0x00, 0xA1, 0xB2, 0xC3};
//uint8_t response_mac[6];
//const char ip_bytes[] = "192.168.5.23\0";
//
//IPAddress my_ip;



EthernetServer ethServer;

extern Semaphore_Handle sem_ethernetInit;

char* readPointer;
bool readUntil(char *buffer, char delimiter);
char* moveUntil(char* buffer, char delimiter);
void Dashboard_handleHttp(EthernetClient* hEthClient, char* read_buffer);
void Dashboard_handleParameterChange(char* parameter, char* value);

const char buffer[] = "Hello world!\0";
//extern const char Html_head[];
//extern const char Html_preamble[];
//extern const char Html_style[];
bool readDestination;



void* Dashboard_mainTask(UArg a0, UArg a1)
{
    Semaphore_pend(sem_ethernetInit, BIOS_WAIT_FOREVER);

//    IPAddress_fromString(&my_ip, ip_bytes);
//    //IPAddress_fromString(&remote_ip, remote_ip_bytes);
//    Ethernet_begin_mac_ip(my_mac, my_ip);
    EthernetServer_begin(&ethServer, PORT);
    System_printf("%d.%d.%d.%d\n",  Ethernet_localIP().bytes[0], Ethernet_localIP().bytes[1], Ethernet_localIP().bytes[2], Ethernet_localIP().bytes[3]);



    for(;;)
    {
        readDestination = false;
        EthernetClient ethClient = EthernetServer_available(&ethServer);
        if (ethClient._sock < 8)                       // if client connected to server
        {
            char incoming_buffer[INPUT_BUFFER_SIZE];
            System_printf("DASH CON\n");
            bool currentLineIsBlank = true;
            while (EthernetClient_connected(&ethClient))
            {
                if (EthernetClient_available(&ethClient))
                {

                    if (!readDestination) {
                        EthernetClient_readBytesUntil(&ethClient, '\n', incoming_buffer, INPUT_BUFFER_SIZE);
                        readDestination = true;
                    }

                    uint8_t c = EthernetClient_read(&ethClient);
                    if (c == '\n' && currentLineIsBlank)
                    {
                        Dashboard_handleHttp(&ethClient, incoming_buffer);
                    }
                    if (c == '\n')      currentLineIsBlank = true;
                    else if (c != '\r') currentLineIsBlank = false;
                }
                Task_sleep(100);
            }


        }
        Task_sleep(100);
    }
}


void Dashboard_handleHttp(EthernetClient* hEthClient, char* read_buffer) {
    // HOMEPAGE -----------------------------
    // pocitam POUZE s metodou GET, pak hodnoty vracene formularem
    // System_printf("%s\n", read_buffer);
    *(strstr(read_buffer, " HTTP")) = '\0';
    strtok(read_buffer, "?");
    char* parameter = strtok(NULL, "=");
    char* value = strtok(NULL, "&");
    while (parameter != NULL)
    {
        Dashboard_handleParameterChange(parameter, value);
        parameter = strtok(NULL, "=");
        value = strtok(NULL, "&");
    }

    //Send HTML...
    //...preamble
    EthernetClient_println(hEthClient, Html_preamble());

    //...head
    EthernetClient_println(hEthClient, Html_head());

    //...CSS
    EthernetClient_println(hEthClient, Html_style());

    //...<section> Network Info
    EthernetClient_println(hEthClient,
                           Html_sectionNetworkInfo(Ethernet_getThisMac(),
                                                   Ethernet_getThisIp(),
                                                   Ethernet_getGatewayIp(),
                                                   Ethernet_getMask(),
                                                   Ethernet_getDhcpBool() ? "true" : "false"));
    //...<section> Remote Target
    EthernetClient_println(hEthClient,
                           Html_sectionRemoteTarget((unsigned int) Radio_getCurrentProtocol(),
                                                    (const char*)Ethernet_getTargetIp(),
                                                    Ethernet_getSniffingStatus() ? "true" : "false"));
    //...<section> Statistics
    Radio_stats* radioStats = Radio_getStats();
    EthernetClient_println(hEthClient,
                           Html_sectionStatistics(radioStats->nRxOkBle,
                                                  radioStats->nRxNokBle,
                                                  radioStats->nRxOkIeee,
                                                  radioStats->nRxNokIeee,
                                                  radioStats->lastRssi));

    //...<script>
    EthernetClient_println(hEthClient, Html_script());

    // TCP FIN ACK
    EthernetClient_stop(hEthClient);


}

void Dashboard_handleParameterChange(char* parameter, char* value) {
#ifdef ETH_DEBUG
    System_printf("%s | %s\n", parameter, value);
#endif
    /* Change target IP address */
    if (strcmp(parameter, "tgt") == 0)
    {
        Ethernet_setTargetIp(value);
    }

    /* Change Protocol */
    if (strcmp(parameter, "pro") == 0)
    {
        Protocol p = (*value - '0');
        //Radio_setCurrentProtocol(p);
    }

    /* Start/stop sniffing */
    if (strcmp(parameter, "run") == 0)
    {
        if (*value == '1') Ethernet_startSniffing();
        if (*value == '0') Ethernet_stopSniffing();
    }

    /* Change gateway IP address */
    if (strcmp(parameter, "gip") == 0)
    {
        Ethernet_setGatewayIp(value);
    }

    /* Change Subnet Mask */
    if (strcmp(parameter, "msk") == 0)
    {
        Ethernet_setMask(value);
    }

    /* Change this IP address */
    if (strcmp(parameter, "tip") == 0)
    {
        Ethernet_setThisIp(value);
    }

    /* Do or Dont use DHCP */
    if (strcmp(parameter, "dhcp") == 0)
    {
        if (*value == '1') Ethernet_startDhcp();
    }

    /* Change BLE or IEEE 802.15.4 RX channel */
    if (strcmp(parameter, "chn") == 0)
    {
        uint8_t ch = atoi(value);
        Radio_setRXChannel(ch);
    }
}




