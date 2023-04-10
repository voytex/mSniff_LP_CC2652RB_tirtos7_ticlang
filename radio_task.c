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
static Protocol currentProtocol = BLE_1M;

// Radio Object
static RF_Object rjObject;
// Radio Handle
static RF_Handle rfHandle;
// RF Parameters struct
static RF_Params rfParams;
// Handle for managing commands
static RF_CmdHandle cmdHandle;
// Struct with Radio statistics
//TODO: rename stats to bleStats
static rfc_bleGenericRxOutput_t stats;
static rfc_ieeeRxOutput_t ieeeStats;
static Radio_stats globalStats;

static uint16_t nRxBufFull = 0;

UART2_Handle uart;
UART2_Params uartParams;



/*
 * Main task for Radio, inits objects and queue
 * then falls into endless loop and periodically prints stats.
 */
void* Radio_mainTask(UArg a0, UArg a1)
{
    Radio_initObjects();
    RadioQueue_init();
    RadioQueue_reset();

    Radio_openRF();
    Radio_setFS();

    Radio_beginRX();

    System_printf("RX START\n");

    Semaphore_post(sem_radioInit);
    for (;;)
    {
#ifdef RF_DEBUG
        System_printf("=============\n");
        System_printf("RX STATUS: %x\n", Radio_getRXcmd()->status);
        System_printf("RX OK:  %d\n", stats.nRxOk);
        System_printf("RX NOK: %d\n", stats.nRxNok);
        System_printf("RXBFUL: %d\n", nRxBufFull);
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
    rfHandle = RF_open(&rjObject,
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
    RF_postCmd(rfHandle,
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
    RF_cancelCmd(rfHandle, cmdHandle, 0);
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
    GPIO_init();
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    switch (currentProtocol)
    {
    case BLE_1M:
        RF_cmdBle5GenericRx_bt5le1m_0.pParams->pRxQ = RadioQueue_getDQpointer();
        RF_cmdBle5GenericRx_bt5le1m_0.whitening.init = 0x65;
        RF_cmdBle5GenericRx_bt5le1m_0.pOutput = &stats;
        RF_cmdBle5GenericRx_bt5le1m_0.channel = 0x66;
        break;

    case IEEE_802_15_4:
        RF_cmdIeeeRx_ieee154_1.pOutput = &ieeeStats;
        break;

    default:
        break;
    }

}

/*
 * Handles RX queue overflow for now
 */
void Radio_callback(RF_Handle h, RF_CmdHandle ch, RF_EventMask e)
{
    nRxBufFull++;
    //Radio_stopRX();
    RadioQueue_reset();
    Radio_beginRX();
}

Radio_stats* Radio_getStats()
{
    switch(currentProtocol)
    {
    case BLE_1M:
        globalStats.nRxOk = stats.nRxOk;
        globalStats.nRxNok = stats.nRxNok;
        globalStats.lastRssi = stats.lastRssi;
        break;

    case IEEE_802_15_4:
        globalStats.nRxOk = ieeeStats.nRxData + ieeeStats.nRxBeacon + ieeeStats.nRxAck; //TODO: is it alright?
        globalStats.nRxNok = ieeeStats.nRxNok;
        globalStats.lastRssi = ieeeStats.lastRssi;
        break;
    }
    return &globalStats;
}








