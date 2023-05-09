/*
 * radio.c
 *
 *  Created on: 17. 11. 2022
 *      Author: vojtechlukas
 *      Made by editing Texas Instruments' code
 */





#include <unistd.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART2.h>
#include <xdc/runtime/System.h>
#include <ti/drivers/rf/RF.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>
#include DeviceFamily_constructPath(driverlib/rf_ble_mailbox.h)

/* Driver configuration */
#include "ti_drivers_config.h"
#include "ti_radio_config.h"
#include "radio_queue.h"
#include "radio_task.h"

extern Semaphore_Handle sem_radioInit;

// Current RF protocol in use (BLE5 or IEEE 802.15.4)
static Protocol currentProtocol = IEEE_802_15_4;

// Radio Object
RF_Object rfObject;
// Radio Handle
RF_Handle rfHandle;
// RF Parameters struct
RF_Params rfParams;
// Handle for managing commands
RF_CmdHandle cmdHandle;
// Struct with Radio statistics
//TODO: rename stats to bleStats
static rfc_bleGenericRxOutput_t stats;
rfc_ieeeRxOutput_t ieeeStats;
Radio_stats globalStats;

static uint16_t nRxBufFull = 0;

UART2_Handle uart;
UART2_Params uartParams;

#define RF_DEBUG



/*
 * Main task for Radio, inits objects and queue
 * then falls into endless loop and periodically prints stats.
 */
void* Radio_mainTask(UArg a0, UArg a1)
{
//
//    RF_Object lcl_rfObject;
//    RF_Params lcl_rfParams;
//    RF_Params_init(&lcl_rfParams);
//    RF_Handle lcl_rfHandle = RF_open(&lcl_rfObject, (RF_Mode*)&RF_prop_bt5le1m_0, (RF_RadioSetup*)&RF_cmdBle5RadioSetup_bt5le1m_0, &lcl_rfParams);
//
//
//
//    RadioQueue_init();
//
//    RF_cmdBle5GenericRx_bt5le1m_0.pParams->pRxQ = RadioQueue_getDQpointer();
//    RF_cmdBle5GenericRx_bt5le1m_0.whitening.init = 0x65;
//    RF_cmdBle5GenericRx_bt5le1m_0.pOutput = &stats;
//    RF_cmdBle5GenericRx_bt5le1m_0.channel = 0x66;
//
//    System_printf("FS status:   %x\n", RF_cmdFs_bt5le1m_0.status);
//    System_printf("Run FS:      %d\n", RF_runCmd(lcl_rfHandle, (RF_Op*)&RF_cmdFs_bt5le1m_0, RF_PriorityNormal, NULL, 0));
//    System_printf("RX status: 0x%x\n", RF_cmdBle5GenericRx_bt5le1m_0.status);
//    RF_CmdHandle lcl_cmdHandle =  RF_postCmd(lcl_rfHandle, (RF_Op*)&RF_cmdBle5GenericRx_bt5le1m_0, RF_PriorityNormal, &Radio_callback, RF_EventRxEntryDone);
//    System_printf("RX status: 0x%x\n", RF_cmdBle5GenericRx_bt5le1m_0.status);
//    Task_sleep(200000);
//    //RF_runDirectCmd(rfHandle, CMD_ABORT);
//    System_printf("Flush all:   %d\n", RF_flushCmd(lcl_rfHandle, RF_CMDHANDLE_FLUSH_ALL, 0));
//    System_printf("RX status: 0x%x\n", RF_cmdBle5GenericRx_bt5le1m_0.status);
//
//    Task_sleep(200000);
//    RF_close(lcl_rfHandle);
//    System_printf("==========================\n");
//
//    RF_yield(lcl_rfHandle);
//    lcl_rfHandle = RF_open(&lcl_rfObject, (RF_Mode*)&RF_prop_ieee154_1, (RF_RadioSetup*)&RF_cmdRadioSetup_ieee154_1, &lcl_rfParams);
//
//    RadioQueue_init();
//
//    RF_cmdIeeeRx_ieee154_1.pRxQ = RadioQueue_getDQpointer();
//    RF_cmdIeeeRx_ieee154_1.rxConfig.bAutoFlushCrc = 0;
//    RF_cmdIeeeRx_ieee154_1.rxConfig.bAutoFlushIgn = 0;
//    RF_cmdIeeeRx_ieee154_1.rxConfig.bIncludePhyHdr = 1;
//    RF_cmdIeeeRx_ieee154_1.rxConfig.bIncludeCrc = 1;
//    RF_cmdIeeeRx_ieee154_1.rxConfig.bAppendRssi = 1;
//    RF_cmdIeeeRx_ieee154_1.rxConfig.bAppendCorrCrc = 1;
//    RF_cmdIeeeRx_ieee154_1.rxConfig.bAppendSrcInd = 1;
//    RF_cmdIeeeRx_ieee154_1.rxConfig.bAppendTimestamp = 1;
//    RF_cmdIeeeRx_ieee154_1.pOutput = &ieeeStats;
//    RF_cmdIeeeRx_ieee154_1.channel = 11;
//    RF_cmdIeeeRx_ieee154_1.condition.rule = COND_NEVER;
//
//    RF_cmdFs_ieee154_1.frequency = 2405;
//    RF_cmdFs_ieee154_1.fractFreq = 0;
//
//    System_printf("Run FS:      %d\n", RF_runCmd(lcl_rfHandle, (RF_Op*)&RF_cmdFs_ieee154_1, RF_PriorityNormal, NULL, 0));
//
//    System_printf("RX status: 0x%x\n", RF_cmdIeeeRx_ieee154_1.status);
//    lcl_cmdHandle =  RF_postCmd(lcl_rfHandle, (RF_Op*)&RF_cmdIeeeRx_ieee154_1, RF_PriorityNormal, &Radio_callback, RF_EventRxEntryDone);
//    System_printf("RX status: 0x%x\n", RF_cmdIeeeRx_ieee154_1.status);
//    Task_sleep(200000);
//    RF_runDirectCmd(rfHandle, CMD_ABORT);
////    System_printf("Flush all:   %d\n", RF_flushCmd(lcl_rfHandle, RF_CMDHANDLE_FLUSH_ALL, 0));
//    System_printf("RX status: 0x%x\n", RF_cmdIeeeRx_ieee154_1.status);
//
//
//    System_printf("==========================\n");
//    RF_close(lcl_rfHandle);
//    RF_yield(lcl_rfHandle);
//    RF_cmdFs_bt5le1m_0.status = 0;
//    Task_sleep(200000);
//
//    lcl_rfHandle = RF_open(&lcl_rfObject, (RF_Mode*)&RF_prop_bt5le1m_0, (RF_RadioSetup*)&RF_cmdBle5RadioSetup_bt5le1m_0, &lcl_rfParams);
//    RadioQueue_init();
//
//    RF_cmdBle5GenericRx_bt5le1m_0.pParams->pRxQ = RadioQueue_getDQpointer();
//
//    System_printf("FS status:   %x\n", RF_cmdFs_bt5le1m_0.status);
//    System_printf("Run FS:      %d\n", RF_runCmd(lcl_rfHandle, (RF_Op*)&RF_cmdFs_bt5le1m_0, RF_PriorityNormal, NULL, 0));
//    System_printf("RX status: 0x%x\n", RF_cmdBle5GenericRx_bt5le1m_0.status);
//    lcl_cmdHandle =  RF_postCmd(lcl_rfHandle, (RF_Op*)&RF_cmdBle5GenericRx_bt5le1m_0, RF_PriorityNormal, &Radio_callback, RF_EventRxEntryDone);
//    System_printf("RX status: 0x%x\n", RF_cmdBle5GenericRx_bt5le1m_0.status);
//    Task_sleep(200000);
//    System_printf("Flush all:   %d\n", RF_flushCmd(lcl_rfHandle, RF_CMDHANDLE_FLUSH_ALL, 1));
//    System_printf("RX status: 0x%x\n", RF_cmdBle5GenericRx_bt5le1m_0.status);
//
//







    RadioQueue_init();
    System_printf("queue initialized\n");

    RadioQueue_reset();
    System_printf("queue reset\n");

    Radio_initObjects();
    System_printf("objects initialized\n");


    Radio_openRF();
    Radio_setFS();

    Radio_beginRX();

    System_printf("RX START\n");
    System_printf("BLE RX code: %x\n", RF_cmdBle5GenericRx_bt5le1m_0.status);
    System_printf("IEEE RX code: %x\n", RF_cmdIeeeRx_ieee154_1.status);

    Semaphore_post(sem_radioInit);

    for (;;)
    {
#ifdef RF_DEBUG
        Radio_getStats();
        System_printf("=============\n");
        System_printf("RX STATUS:  %x\n", Radio_getRXcmd()->status);
        System_printf("RXieee OK:  %d\n", globalStats.nRxOkIeee);
        System_printf("RXble  OK:  %d\n", globalStats.nRxOkBle);
        System_printf("RXieee NOK: %d\n", globalStats.nRxNokIeee);
        System_printf("RXble  NOK: %d\n", globalStats.nRxNokBle);
        System_printf("RXBFUL:     %d\n", nRxBufFull);
        System_printf("LRSSI:      %d\n", ieeeStats.lastRssi);
        System_printf("CH:         %d\n", ((rfc_CMD_FS_t*)Radio_getFScmd())->frequency);
#endif
        Task_sleep(100000);
    }
}

/*
 * Opens Radio Core
 * Returns nothing
 */
void Radio_openRF(void)
{
    RF_Params_init(&rfParams);
    rfHandle = RF_open(&rfObject,
                       Radio_getRFmode(),
                       Radio_getRadioSetup(),
                       &rfParams);
    // Handle could be NULL!
    // TODO NULL Handle

}

/*
 * Sets Radio Frequency Sythesizer according
 * to given Protocol and its channel (frequency).
 * Returns nothing
 */
void Radio_setFS(void)
{
     RF_runCmd(rfHandle,
               Radio_getFScmd(),
               RF_PriorityNormal,
               NULL, 0);

}

/*
 * Begins Sniffing Packets
 */
void Radio_beginRX()
{
    cmdHandle = RF_postCmd(rfHandle,
                           Radio_getRXcmd(),
                           RF_PriorityNormal,
                           &Radio_callback, RF_EventRxBufFull);
}

/*
 * Stops Sniffing Packets
 */
void Radio_stopRX(void)
{
    System_printf("%d\n", RF_flushCmd(rfHandle, RF_CMDHANDLE_FLUSH_ALL, 1));

    // Disable RAT timer channels. This is done to avoid callbacks
    // after RF driver is closed.
    RF_ratDisableChannel(rfHandle, RF_RatChannel0);
    RF_ratDisableChannel(rfHandle, RF_RatChannel1);
    RF_ratDisableChannel(rfHandle, RF_RatChannel2);

    // Close RF driver
    RF_close(rfHandle);
}

/*
 * Returns RF mode according to
 * current RF protocol (BLE or IEEE)
 */
RF_Mode* Radio_getRFmode(void)
{
    switch (currentProtocol)
    {
    case BLE_1M:
        return (RF_Mode*)&RF_prop_bt5le1m_0;
        break;

    case IEEE_802_15_4:
        return (RF_Mode*)&RF_prop_ieee154_1;
        break;

    default:
        return 0;
        break;
    }
}

/*
 * Returns Radio Setup Command according
 * to current RF protocol (BLE or IEEE)
 */
RF_RadioSetup* Radio_getRadioSetup(void)
{
    switch (currentProtocol)
    {
    case BLE_1M:
        return (RF_RadioSetup*)&RF_cmdBle5RadioSetup_bt5le1m_0;
        break;

    case IEEE_802_15_4:
        return (RF_RadioSetup*)&RF_cmdRadioSetup_ieee154_1;
        break;

    default:
        return 0;
        break;
    }
}

/*
 * Returns command for frequency synthesizer control
 * according to current RF protocol (BLE or IEEE)
 */
RF_Op* Radio_getFScmd(void)
{
    switch (currentProtocol)
    {
    case BLE_1M:
        return (RF_Op*)&RF_cmdFs_bt5le1m_0;
        break;

    case IEEE_802_15_4:
        return (RF_Op*)&RF_cmdFs_ieee154_1;
        break;

    default:
        return 0;
        break;
    }
}


/*
 * Returns CMD_GENERIC_RX command
 * according to current RF protocol (BLE or IEEE)
 */
RF_Op* Radio_getRXcmd(void)
{
    switch (currentProtocol)
    {
    case BLE_1M:
        return (RF_Op*)&RF_cmdBle5GenericRx_bt5le1m_0;
        break;

    case IEEE_802_15_4:
        return (RF_Op*)&RF_cmdIeeeRx_ieee154_1;
        break;

    default:
        return 0;
        break;
    }

}

/*
 * Initializes and sets GPIO and commands
 */
void Radio_initObjects(void)
{

    RF_cmdBle5GenericRx_bt5le1m_0.pParams->pRxQ = RadioQueue_getDQpointer();
    RF_cmdBle5GenericRx_bt5le1m_0.pParams->rxConfig.bAutoFlushCrcErr = 1;
    RF_cmdBle5GenericRx_bt5le1m_0.pParams->rxConfig.bIncludeLenByte = 1;
    RF_cmdBle5GenericRx_bt5le1m_0.pParams->rxConfig.bIncludeCrc = 1;
    RF_cmdBle5GenericRx_bt5le1m_0.pParams->rxConfig.bAppendRssi = 0;
    RF_cmdBle5GenericRx_bt5le1m_0.pParams->rxConfig.bAppendStatus = 0;
    RF_cmdBle5GenericRx_bt5le1m_0.whitening.init = 0x65;
    RF_cmdBle5GenericRx_bt5le1m_0.pOutput = &stats;
    RF_cmdBle5GenericRx_bt5le1m_0.channel = 0x66;

    RF_cmdIeeeRx_ieee154_1.pRxQ = RadioQueue_getDQpointer();
    RF_cmdIeeeRx_ieee154_1.rxConfig.bAutoFlushCrc = 1;
    RF_cmdIeeeRx_ieee154_1.rxConfig.bAutoFlushIgn = 0;
    RF_cmdIeeeRx_ieee154_1.rxConfig.bIncludePhyHdr = 0;
    RF_cmdIeeeRx_ieee154_1.rxConfig.bIncludeCrc = 1;
    RF_cmdIeeeRx_ieee154_1.rxConfig.bAppendRssi = 0;
    RF_cmdIeeeRx_ieee154_1.rxConfig.bAppendCorrCrc = 0;
    RF_cmdIeeeRx_ieee154_1.rxConfig.bAppendSrcInd = 0;
    RF_cmdIeeeRx_ieee154_1.rxConfig.bAppendTimestamp = 0;
    RF_cmdIeeeRx_ieee154_1.pOutput = &ieeeStats;
    RF_cmdIeeeRx_ieee154_1.channel = 0;

    RF_cmdFs_ieee154_1.synthConf.bTxMode = 1;
    RF_cmdFs_ieee154_1.frequency = 0x0965;
    RF_cmdFs_ieee154_1.fractFreq = 0;

}

/*
 * Handles RX queue overflow for now
 */
void Radio_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    //uint8_t buffer[64];
    //RadioQueue_takePacket(buffer, 64);
    nRxBufFull++;

    RadioQueue_reset();
    Radio_beginRX();
}

Radio_stats* Radio_getStats()
{
    switch(currentProtocol)
    {
    case BLE_1M:
        globalStats.nRxOkBle = stats.nRxOk;
        globalStats.nRxNokBle = stats.nRxNok;
        globalStats.lastRssi = stats.lastRssi;
        break;

    case IEEE_802_15_4:
        globalStats.nRxOkIeee = ieeeStats.nRxData + ieeeStats.nRxBeacon + ieeeStats.nRxMacCmd; //TODO: is it alright?
        globalStats.nRxNokIeee = ieeeStats.nRxNok;
        globalStats.lastRssi = ieeeStats.lastRssi;
        break;
    }
    return &globalStats;
}

Protocol Radio_getCurrentProtocol()
{
    return currentProtocol;
}

void Radio_setCurrentProtocol(Protocol p)
{
    Radio_stopRX();
    currentProtocol = p;
    RadioQueue_reset();
    Radio_openRF();
    Radio_setFS();
    Radio_beginRX();
    System_printf("BLE status:  %d\n", RF_cmdBle5GenericRx_bt5le1m_0.status);
    System_printf("IEEE status: %d\n", RF_cmdIeeeRx_ieee154_1.status);
//    Radio_stopRX();
//    while (Radio_getRXcmd()->status != 0)
//    {
//        System_printf("waiting");
//        Task_sleep(100);
//    }
//    currentProtocol = p;
//
//    /*RadioQueue_init();
//    System_printf("queue initialized\n");*/
//
//    RadioQueue_reset();
//    System_printf("queue reset\n");
//
//    Radio_initObjects();
//    System_printf("objects initialized\n");
//
//
//    Radio_openRF();
//    Radio_setFS();
//    Radio_beginRX();
}

uint8_t Radio_getRXChannel(void)
{
    switch (currentProtocol)
    {
    case BLE_1M:
        return RF_cmdBle5GenericRx_bt5le1m_0.channel;

    case IEEE_802_15_4:
        return RF_cmdIeeeRx_ieee154_1.channel;

    default:
        return 0;
    }
}

void Radio_setRXChannel(uint8_t ch)
{
    switch (currentProtocol)
    {
    case BLE_1M:
        if ((ch >= 37) && (ch <= 39))
        {
            switch (ch)
            {
            case 37:
                RF_cmdFs_bt5le1m_0.frequency = 2402;
                break;
            case 38:
                RF_cmdFs_bt5le1m_0.frequency = 2426;
                break;
            case 39:
                RF_cmdFs_bt5le1m_0.frequency = 2480;
            }
            RF_cmdBle5GenericRx_bt5le1m_0.channel = ch;
            RF_cmdBle5GenericRx_bt5le1m_0.whitening.bOverride = 0;
        }
        else return;
        break;

    case IEEE_802_15_4:
        if ((ch >= 11) && (ch <= 26))
        {
            RF_cmdFs_ieee154_1.frequency = (2405 + 5*(ch - 11));
        }
        return;
        break;
    }
}












