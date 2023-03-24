/*
 * WebsocketServer.c
 *
 *  Created on: 1. 2. 2023
 *      Author: vojtechlukas
 */



#include "ethernet/Ethernet.h"
#include "ethernet/EthernetServer.h"
#include "websocket/WebsocketServer.h"

void WebsocketServer_begin(EthernetServer* eth, uint16_t port)
{
    EthernetServer_begin(eth, port);
}



