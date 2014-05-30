#include "ee.h"





/***************************************************************************
 *
 * Kernel ( CPU 0 )
 *
 **************************************************************************/
    /* Definition of task's body */
    DeclareTask(TaskLCD);
    DeclareTask(TaskSend);
    DeclareTask(TaskA);
    DeclareTask(TaskB);
    DeclareTask(TaskC);
    DeclareTask(TaskD);
    DeclareTask(TaskE);
    DeclareTask(TaskF);
    DeclareTask(Task_TimeOut);

    const EE_THREAD_PTR EE_hal_thread_body[EE_MAX_TASK] = {
        &FuncTaskLCD,		/* thread TaskLCD */
        &FuncTaskSend,		/* thread TaskSend */
        &FuncTaskA,		/* thread TaskA */
        &FuncTaskB,		/* thread TaskB */
        &FuncTaskC,		/* thread TaskC */
        &FuncTaskD,		/* thread TaskD */
        &FuncTaskE,		/* thread TaskE */
        &FuncTaskF,		/* thread TaskF */
        &FuncTask_TimeOut 		/* thread Task_TimeOut */

    };

    /* ready priority */
    const EE_TYPEPRIO EE_th_ready_prio[EE_MAX_TASK] = {
        0x1U,		/* thread TaskLCD */
        0x1U,		/* thread TaskSend */
        0x1U,		/* thread TaskA */
        0x1U,		/* thread TaskB */
        0x1U,		/* thread TaskC */
        0x1U,		/* thread TaskD */
        0x1U,		/* thread TaskE */
        0x1U,		/* thread TaskF */
        0x2U 		/* thread Task_TimeOut */
    };

    /* dispatch priority */
    const EE_TYPEPRIO EE_th_dispatch_prio[EE_MAX_TASK] = {
        0x1U,		/* thread TaskLCD */
        0x1U,		/* thread TaskSend */
        0x1U,		/* thread TaskA */
        0x1U,		/* thread TaskB */
        0x1U,		/* thread TaskC */
        0x1U,		/* thread TaskD */
        0x1U,		/* thread TaskE */
        0x1U,		/* thread TaskF */
        0x2U 		/* thread Task_TimeOut */
    };

    /* thread status */
    #if defined(__MULTI__) || defined(__WITH_STATUS__)
        EE_TYPESTATUS EE_th_status[EE_MAX_TASK] = {
            EE_READY,
            EE_READY,
            EE_READY,
            EE_READY,
            EE_READY,
            EE_READY,
            EE_READY,
            EE_READY,
            EE_READY
        };
    #endif

    /* next thread */
    EE_TID EE_th_next[EE_MAX_TASK] = {
        EE_NIL,
        EE_NIL,
        EE_NIL,
        EE_NIL,
        EE_NIL,
        EE_NIL,
        EE_NIL,
        EE_NIL,
        EE_NIL
    };

    EE_TYPEPRIO EE_th_nact[EE_MAX_TASK];
    /* The first stacked task */
    EE_TID EE_stkfirst = EE_NIL;

    /* The first task into the ready queue */
    EE_TID EE_rqfirst  = EE_NIL;

    /* system ceiling */
    EE_TYPEPRIO EE_sys_ceiling= 0x0000U;



/***************************************************************************
 *
 * Mutex
 *
 **************************************************************************/
    const EE_TYPEPRIO EE_resource_ceiling[EE_MAX_RESOURCE]= {
        0x2U 		/* resource RES_SCHEDULER */
    };

    EE_TYPEPRIO EE_resource_oldceiling[EE_MAX_RESOURCE];



/***************************************************************************
 *
 * Counters
 *
 **************************************************************************/
    EE_counter_RAM_type       EE_counter_RAM[EE_MAX_COUNTER] = {
        {0, -1}         /* myCounter */
    };



/***************************************************************************
 *
 * Alarms
 *
 **************************************************************************/
    const EE_alarm_ROM_type   EE_alarm_ROM[] = {
        {0, EE_ALARM_ACTION_TASK    , TaskLCD, NULL},
        {0, EE_ALARM_ACTION_TASK    , TaskSend, NULL},
        {0, EE_ALARM_ACTION_TASK    , TaskA, NULL},
        {0, EE_ALARM_ACTION_TASK    , TaskB, NULL},
        {0, EE_ALARM_ACTION_TASK    , TaskC, NULL},
        {0, EE_ALARM_ACTION_TASK    , TaskD, NULL},
        {0, EE_ALARM_ACTION_TASK    , TaskE, NULL},
        {0, EE_ALARM_ACTION_TASK    , TaskF, NULL},
        {0, EE_ALARM_ACTION_TASK    , Task_TimeOut, NULL}
    };

    EE_alarm_RAM_type         EE_alarm_RAM[EE_MAX_ALARM];

