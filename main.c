/*
 * mSniff | Master's project | BUT FEEC 2022/23
 * Vojtech Lukas | 211572
 *
 */

/*
 *  ======== main.c ========
 */
#include <stdint.h>

/* RTOS header files */
#include <ti/sysbios/BIOS.h>

#include <ti/drivers/Board.h>

#include <ti/sysbios/knl/Semaphore.h>


// Include for System_printf();
#include <xdc/runtime/System.h>

// Include for Tasks
#include <ti/sysbios/knl/Task.h>


extern void *Radio_mainTask(UArg a0, UArg a1);
extern void *Ethernet_mainTask(UArg a0, UArg a1);
extern void *Dashboard_mainTask(UArg a0, UArg a1);

Task_Params Radio_taskParams;
Task_Handle Radio_taskHandle;

Task_Params Ethernet_taskParams;
Task_Handle Ethernet_taskHandle;

Task_Params Dashboard_taskParams;
Task_Handle Dashboard_taskHandle;

Semaphore_Handle sem_radioInit;
Semaphore_Params sem_riParams;

Semaphore_Handle sem_ethernetInit;
Semaphore_Params sem_eiParams;

Semaphore_Handle sem_ready2send;
Semaphore_Params sem_r2sParams;

/* Stack size in bytes */
#define THREADSTACKSIZE 1024

/*
 *  ======== main ========
 */
int main(void)
{

    Board_init();

    Semaphore_Params_init(&sem_riParams);
    sem_riParams.mode = 0x1;
    sem_radioInit = Semaphore_create(0, &sem_riParams, NULL);

    Semaphore_Params_init(&sem_eiParams);
    sem_eiParams.mode = 0x1;
    sem_ethernetInit = Semaphore_create(0, &sem_eiParams, NULL);

    Semaphore_Params_init(&sem_r2sParams);
    sem_r2sParams.mode = 0x1;
    sem_ready2send = Semaphore_create(0, &sem_r2sParams, NULL);

    // RF TASK =======
    Task_Params_init(&Radio_taskParams);
    Radio_taskParams.priority = 2;
    Radio_taskHandle = Task_create((Task_FuncPtr)Radio_mainTask, &Radio_taskParams, NULL);

    // ETH TASK ======
    Task_Params_init(&Ethernet_taskParams);
    Ethernet_taskParams.priority = 2;
    Ethernet_taskHandle = Task_create((Task_FuncPtr)Ethernet_mainTask, &Ethernet_taskParams, NULL);

    // DASH TASK =====
    Task_Params_init(&Dashboard_taskParams);
    Dashboard_taskParams.priority = 2;
    Dashboard_taskParams.stackSize = 2048;
    Dashboard_taskHandle = Task_create((Task_FuncPtr)Dashboard_mainTask, &Dashboard_taskParams, NULL);


    BIOS_start();

    return (0);
}
