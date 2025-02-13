
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Hardware.h                       //
//                                                       //
///////////////////////////////////////////////////////////

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef _HARDWARE_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#define CFG_OSCCON    0XF0
#define CFG_OSCTUNE   0X00

#define CFG_APFCON0   0X00
#define CFG_APFCON1   0X00

#define CFG_PORTA     0X04
#define CFG_TRISA     0X1B
#define CFG_ANSELA    0X11
#define CFG_WPUA      0X00
#define CFG_INLVLA    0X00

#define CFG_PORTB     0XE0
#define CFG_TRISB     0X30
#define CFG_ANSELB    0X00
#define CFG_WPUB      0X00
#define CFG_INLVLB    0X20

#define CFG_PORTC     0X00
#define CFG_TRISC     0XEF
#define CFG_ANSELC    0XCF
#define CFG_WPUC      0X00
#define CFG_INLVLC    0X00

#define CFG_FVRCON    0X00
#define CFG_DACCON0   0X00
#define CFG_DACCON1   0X00
#define CFG_CM1CON0   0X82
#define CFG_CM1CON1   0X03
#define CFG_CM2CON0   0XA2
#define CFG_CM2CON1   0X01

#define AIN_CUR       9
#define AIN_VRS       6
#define AIN_NTC       3
#define AIN_VAC       5

#define SYS_RELAY_ON( )   LATB6 = 1
#define SYS_RELAY_OFF( )  LATB6 = 0

EXTERN bank1 U8  sysTicks;

EXTERN bank1 U32 adCur2;
EXTERN bank1 U8  adCurN;

EXTERN bank1 U8  adVrs;
EXTERN bank1 U8  adNtc;
EXTERN bank1 U8  adVac;

EXTERN bank1 U8  idrTest0;
EXTERN bank1 U8  idrTest1;

EXTERN bank3 U8  txCNT;
EXTERN bank3 U8  txWRP;
EXTERN bank3 U8  txRDP;
EXTERN bank3 U8  txTIM;
EXTERN bank3 U8  txFIFO[ 32 ];
EXTERN bank3 U8  rxCNT;
EXTERN bank3 U8  rxWRP;
EXTERN bank3 U8  rxRDP;
EXTERN bank3 U8  rxTIM;
EXTERN bank3 U8  rxFIFO[ 32 ];

EXTERN bank2 U16 mtrHallTime[ 32 ];
EXTERN bank1 U8  mtrHallIndex;
EXTERN bank1 U8  mtrHallEvent;
EXTERN bank1 U8  mtrHallWDT;

EXTERN bank2 U16 acCrossTime[ 8 ];
EXTERN bank1 U8  acCrossIndex;
EXTERN bank1 U8  acCrossEvent;
EXTERN bank1 U8  acCrossWDT;

EXTERN bank1 U16 acCycle;
EXTERN bank1 U8  acCycleHalf;
EXTERN bank1 U8  acCycleFlag;

EXTERN bank1 U16 tmTriac;

EXTERN void MCU_Init( void );
EXTERN void MCU_Refresh( void );

#endif

