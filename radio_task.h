/*
 * radio.h
 *
 *  Created on: 17. 11. 2022
 *      Author: vojtechlukas
 */

#ifndef RADIO_TASK_H_
#define RADIO_TASK_H_

typedef enum Protocol_t
{
    BLE_1M = 0,
    IEEE_802_15_4 = 1
} Protocol;

typedef struct Radio_stats {
    uint16_t nRxOkBle;
    uint16_t nRxNokBle;
    uint16_t nRxOkIeee;
    uint16_t nRxNokIeee;
    int8_t lastRssi;
} Radio_stats;

void            Radio_openRF(void);
void            Radio_setFS(void);
void            Radio_beginRX(void);
void            Radio_stopRX(void);
void            Radio_initObjects(void);

RF_Mode*        Radio_getRFmode(void);
RF_RadioSetup*  Radio_getRadioSetup(void);
RF_Op*          Radio_getFScmd(void);
RF_Op*          Radio_getRXcmd(void);

Radio_stats*    Radio_getStats(void);

Protocol        Radio_getCurrentProtocol(void);
void            Radio_setCurrentProtocol(Protocol);

void            Radio_setRXChannel(uint8_t);
uint8_t         Radio_getRXChannel(void);

void            Radio_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e);






#endif /* RADIO_TASK_H_ */
