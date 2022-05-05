/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        5       /* Number of identical tasks                          */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

        void  Task(void *data);                       /* Function prototypes of tasks                  */
        void  printMsg(void *data);
        void  TaskStart(void *data);                  /* Function prototypes of Startup task           */
static  void  TaskStartCreateTasks(void);

/*$PAGE*/
/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/
char intBuf[10]={0};
void  main (void)
{
    PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */

    PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */
    PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */
    OSTimeSet(0);
    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 10);
    OSStart();                                             /* Start multitasking                       */
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void  TaskStart (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    char       s[100];
    INT16S     key;


    pdata = pdata;                                         /* Prevent compiler warning                 */

    OS_ENTER_CRITICAL();
    PC_VectSet(0x08, OSTickISR);                           /* Install uC/OS-II's clock tick ISR        */
    PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
    OS_EXIT_CRITICAL();

    //OSStatInit();                                          /* Initialize uC/OS-II's statistics         */

    TaskStartCreateTasks();                                /* Create all the application tasks         */

    for (;;) {
        if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
                PC_DOSReturn();                            /* Return to DOS                            */
            }
        }

        OSCtxSwCtr = 0;                                    /* Clear context switch counter             */
        OSTimeDlyHMSM(0, 0, 10, 0);                         /* Wait one second                          */
    }
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             CREATE TASKS
*********************************************************************************************************
*/

// TODO
static  void  TaskStartCreateTasks (void)
{
    // Create period task
    OSTaskCreate(Task, (void *)0, &TaskStk[0][TASK_STK_SIZE - 1], 1);
    OSTaskCreate(Task, (void *)0, &TaskStk[1][TASK_STK_SIZE - 1], 2);

    // Create printTask
    OSTaskCreate(printMsg, (void *)0, &TaskStk[2][TASK_STK_SIZE - 1], 3);        
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

// TODO
void  Task (void *data)
{
    int start ;
    int end ;
    int toDelay ;
    start = OSTimeGet();
    if(OSTCBCur->OSTCBPrio ==2){
        start -=1 ;
    }
    itoa(OSTCBCur->OSTCBPrio,intBuf,10);
    strcat(outputMsg,"Task");
    strcat(outputMsg,intBuf);
    strcat(outputMsg," startTime=");
    memset(intBuf, 0, 10);
    itoa(start, intBuf, 10);
    strcat(outputMsg,intBuf);
    memset(intBuf, 0, 10);
    strcat(outputMsg,"\n");

    while(1){
        while(OSTCBCur->compTime > 0){
            // C unit of time is't done
            // DO NOTHING
        }
        end = OSTimeGet();
        toDelay = (OSTCBCur->period) - (end-start);
        start = start + OSTCBCur->period;
        OS_ENTER_CRITICAL();
        OSTCBCur->compTime = OSTCBCur->cycleTime;
        OS_EXIT_CRITICAL();

        itoa(OSTCBCur->OSTCBPrio, intBuf, 10);
        strcat(outputMsg, "Task");
        strcat(outputMsg, intBuf);
        strcat(outputMsg, " newStartTime=");
        memset(intBuf, 0, 10);
        itoa(start, intBuf, 10);
        strcat(outputMsg,intBuf);
        memset(intBuf, 0, 10);
        strcat(outputMsg, "\n");

        OSTimeDly(toDelay);
    }
}
void  printMsg(void *data){
    while(1){
        printf("%s", outputMsg);
        memset(outputMsg, 0, 1000);
        OSTimeDly(1);
    }
}
