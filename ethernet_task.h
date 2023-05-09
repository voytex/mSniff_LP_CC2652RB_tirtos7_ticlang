/*
 * ethernet_task.h
 *
 *  Created on: 21. 11. 2022
 *      Author: vojtechlukas
 */

#ifndef ETHERNET_TASK_H_
#define ETHERNET_TASK_H_

void* Ethernet_mainTask(UArg a0, UArg a1);

void  Ethernet_handleRFpacket();

void  Ethernet_setTargetIp(char*);
char* Ethernet_getTargetIp(void);
void  Ethernet_setThisIp(char*);
char* Ethernet_getThisIp(void);
char* Ethernet_getThisMac(void);
void  Ethernet_setGatewayIp(char*);
char* Ethernet_getGatewayIp(void);
void  Ethernet_setMask(char*);
char* Ethernet_getMask(void);
void  Ethernet_startSniffing(void);
void  Ethernet_stopSniffing(void);
bool  Ethernet_getSniffingStatus(void);
void  Ethernet_startDhcp(void);
bool  Ethernet_getDhcpBool(void);



#endif /* ETHERNET_TASK_H_ */
