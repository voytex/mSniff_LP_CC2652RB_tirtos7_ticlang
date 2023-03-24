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
void handleHttp(EthernetClient* ethClient, char* read_buffer);
void handleParameterChange(char* parameter, char* value);

const char buffer[] = "Hello world!\0";
extern const char Html_head[];
extern const char Html_preamble[];
extern const char Html_style[];
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
                        handleHttp(&ethClient, incoming_buffer);
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


void handleHttp(EthernetClient* ethClient, char* read_buffer) {
    // HOMEPAGE -----------------------------
    // pocitam POUZE s metodou GET, pak hodnoty vracene formularem
    System_printf("%s\n", read_buffer);

    //Send HTML
    EthernetClient_println(ethClient, Html_preamble);
    EthernetClient_println(ethClient, Html_head);
    EthernetClient_println(ethClient, Html_style);
    //EthernetClient_println(ethClient, Html_body);
    char *ip;
    Ethernet_getRemoteIp(ip);
    EthernetClient_println(ethClient, Html_remote_target_start);
    EthernetClient_println(ethClient, Html_remote_target_ending);
    EthernetClient_stop(ethClient);

    //cut "HTTP/1.1" substring from the first header line
    *(strstr(read_buffer, "HTTP")) = '\0';
    strtok(read_buffer, "?");
    char* parameter = strtok(NULL, "=");
    char* value = strtok(NULL, "&");
    while (parameter != NULL){
        handleParameterChange(parameter, value);
        parameter = strtok(NULL, "=");
        value = strtok(NULL, "&");
    }
}

void handleParameterChange(char* parameter, char* value) {
    System_printf("%s | %s\n", parameter, value);
    if (strcmp(parameter, "target_ip") == 0) {
        Ethernet_setRemoteIp(value);
    }
}




