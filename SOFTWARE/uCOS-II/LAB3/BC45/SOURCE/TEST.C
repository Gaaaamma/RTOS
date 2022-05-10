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
INT32U zeroTime = 100;
char dbgBuf[10] = {0};
OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];
// Resource
INT8U errR1 ;
INT8U errR2 ;
OS_EVENT *R1 ;
OS_EVENT *R2 ;

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

        void Task1(void *data);                        /* Function prototypes of tasks                  */
        void Task2(void *data);                        /* Function prototypes of tasks                  */
        void Task3(void *data);                        /* Function prototypes of tasks                  */

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
    // Resource allocation

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
    R1 = OSMutexCreate(1, &errR1);
    R2 = OSMutexCreate(2, &errR2);

    // Create period task
    OSTaskCreate(Task1, (void *)0, &TaskStk[0][TASK_STK_SIZE - 1], 3);
    OSTaskCreate(Task2, (void *)0, &TaskStk[1][TASK_STK_SIZE - 1], 4);
    OSTaskCreate(Task3, (void *)0, &TaskStk[2][TASK_STK_SIZE - 1], 5);

    // Create printTask
    OSTaskCreate(printMsg, (void *)0, &TaskStk[3][TASK_STK_SIZE - 1], 6);        
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

// TODO
void  Task1 (void *data)
{
    int start ;
    int end ;
    int toDelay ;
    int onlyOnce_1 =1;
    int onlyOnce_2 =1;
    start = OSTimeGet();
    
    // Need to sleep to 108 sec
    OSTimeDly(zeroTime+8-start);
    start = zeroTime+8;

    while(1){
        onlyOnce_1 =1;
        onlyOnce_2 =1;
        while(OSTCBCur->compTime > 0){
            // C unit of time is't done
            if(OSTCBCur->compTime == (OSTCBCur->cycleTime-2) && onlyOnce_1 ==1){
                //strcat(outputMsg, "Task1 is going to allocate R1");
                strcat(outputMsg,"Task1 tries to allocate: R1\n");
                onlyOnce_1 =0;
                OSMutexPend(R1,99999,&errR1);
            }else if(OSTCBCur->compTime == (OSTCBCur->cycleTime-4) && onlyOnce_2 ==1){
                //strcat(outputMsg, "Task1 is going to allocate R2");
                strcat(outputMsg,"Task1 tries to allocate: R2\n");
                onlyOnce_2 =0;
                OSMutexPend(R2,99999,&errR2);
            }
        }
        // DONE -> release R2 R1
        OSMutexPost(R2);
        OSMutexPost(R1);

        end = OSTimeGet();
        toDelay = (OSTCBCur->period) - (end-start);
        start = start + OSTCBCur->period;
        OS_ENTER_CRITICAL();
        OSTCBCur->compTime = OSTCBCur->cycleTime;
        OS_EXIT_CRITICAL();
        OSTimeDly(toDelay);
    }
}

void  Task2 (void *data)
{
    int start ;
    int end ;
    int toDelay ;
    int onlyOnce_2 =1;
    start = OSTimeGet();

    // Need to sleep to 104 sec
    OSTimeDly(zeroTime+4-start);
    start = zeroTime+4;

    while(1){
        onlyOnce_2 =1;
        while(OSTCBCur->compTime > 0){
            // C unit of time is't done
            if(OSTCBCur->compTime == (OSTCBCur->cycleTime-2) && onlyOnce_2 ==1){
                //strcat(outputMsg, "Task2 is going to allocate R2");
                strcat(outputMsg,"Task2 tries to allocate: R2\n");
                onlyOnce_2 =0;
                OSMutexPend(R2,99999,&errR2);
            }
        }
        // DONE -> release R2
        OSMutexPost(R2);

        end = OSTimeGet();
        toDelay = (OSTCBCur->period) - (end-start);
        start = start + OSTCBCur->period;
        OS_ENTER_CRITICAL();
        OSTCBCur->compTime = OSTCBCur->cycleTime;
        OS_EXIT_CRITICAL();

        OSTimeDly(toDelay);
    }
}

void  Task3 (void *data)
{
    int start ;
    int end ;
    int toDelay ;
    int onlyOnce_1 =1;

    start = OSTimeGet();

    // Need to sleep to 100 sec
    OSTimeDly(zeroTime-start);
    start = zeroTime;

    while(1){
        onlyOnce_1 =1;
        while(OSTCBCur->compTime > 0){
            // C unit of time is't done
            if(OSTCBCur->compTime == (OSTCBCur->cycleTime-2) && onlyOnce_1 ==1){
                //strcat(outputMsg, "Task3 is going to allocate R1");
                strcat(outputMsg, "Task3 tries to allocate: R1\n");
                onlyOnce_1 =0;
                OSMutexPend(R1,99999,&errR1);
            }
        }
        // DONE -> release R1
        OSMutexPost(R1);

        end = OSTimeGet();
        toDelay = (OSTCBCur->period) - (end-start);
        start = start + OSTCBCur->period;
        OS_ENTER_CRITICAL();
        OSTCBCur->compTime = OSTCBCur->cycleTime;
        OS_EXIT_CRITICAL();

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
