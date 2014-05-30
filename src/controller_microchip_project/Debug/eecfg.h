#ifndef RTDH_EECFG_H
#define RTDH_EECFG_H


#define RTDRUID_CONFIGURATOR_NUMBER 1277



/***************************************************************************
 *
 * Common defines ( CPU 0 )
 *
 **************************************************************************/

    /* TASK definition */
    #define EE_MAX_TASK 9
    #define TaskLCD 0
    #define TaskSend 1
    #define TaskA 2
    #define TaskB 3
    #define TaskC 4
    #define TaskD 5
    #define TaskE 6
    #define TaskF 7
    #define Task_TimeOut 8

    /* MUTEX definition */
    #define EE_MAX_RESOURCE 1U
    #define RES_SCHEDULER 0U

    /* ALARM definition */
    #define EE_MAX_ALARM 9
    #define AlarmLCD 0
    #define AlarmSend 1
    #define AlarmA 2
    #define AlarmB 3
    #define AlarmC 4
    #define AlarmD 5
    #define AlarmE 6
    #define AlarmF 7
    #define AlarmTimeOut 8

    /* COUNTER definition */
    #define EE_MAX_COUNTER 1
    #define myCounter 0

    /* APPMODE definition */
    #define EE_MAX_APPMODE 0U

    /* CPUs */
    #define EE_MAX_CPU 1
    #define EE_CURRENTCPU 0

    /* Number of isr 2 */
    #define EE_MAX_ISR2 0

#ifndef __DISABLE_EEOPT_DEFINES__


/***************************************************************************
 *
 * User options
 *
 **************************************************************************/


/***************************************************************************
 *
 * Automatic options
 *
 **************************************************************************/
#define __RTD_CYGWIN__
#define __PIC30__
#define __33FJ256GP710__
#define __PIC33FJ256GP710__
#define __MICROCHIP_EXPLORER16__
#define __USE_LEDS__
#define __USE_BUTTONS__
#define __USE_LCD__
#define __USE_ANALOG__
#define __MICROCHIP_DSPIC30__
#define __PIC30_ICD2__
#define __PIC30_SPLIM__
#define __FP__
#define __MONO__
#define __ALARMS__
#define __ALLOW_NESTED_IRQ__

#endif



#endif

