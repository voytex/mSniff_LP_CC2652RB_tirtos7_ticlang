/*
 * WebsocketServer.h
 *
 *  Created on: 1. 2. 2023
 *      Author: vojtechlukas
 */

#ifndef WEBSOCKET_WEBSOCKETSERVER_H_
#define WEBSOCKET_WEBSOCKETSERVER_H_

void WebsocketServer_begin(EthernetServer* eth, uint16_t port);
void WebsocketServer_write(EthernetServer* eth /*TODO: JSON? buffer?*/);
void WebsocketServer_end(EthernetServer* eth);
EthernetClient WebsocketServer_available(EthernetServer* eth);

#endif /* WEBSOCKET_WEBSOCKETSERVER_H_ */
