
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Motor.h                          //
//                                                       //
///////////////////////////////////////////////////////////

#ifndef _MOTOR_H_
#define _MOTOR_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef _MOTOR_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#if     VER_110VAC
#define SREG_KP   3000
#define SREG_KP_M 800
#define SREG_KI   80
#define SREG_KD   800
#define CUR_THR   1500
#endif

#if     VER_220VAC
#define SREG_KP   2000
#define SREG_KP_M 500
#define SREG_KI   50
#define SREG_KD   500
#define CUR_THR   1000
#endif

EXTERN XRAM U16 mtrSpeedRef;
EXTERN XRAM U16 mtrSpeed;
EXTERN XRAM U16 mtrCurrent;
EXTERN XRAM S8  mtrTemp;

#define E_ERR_NTC_FAULT     2
#define E_ERR_OVER_HEAT     2
#define E_ERR_BAD_TRIAC     3
#define E_ERR_OVER_TIME     4
#define E_ERR_ZERO_SPEED    7
#define E_ERR_OVER_CURRENT  8
#define E_ERR_NO_CROSS      10
EXTERN XRAM U8  mtrError;

EXTERN void MTR_Init( void );
EXTERN void MTR_Ctrl( void );

#endif

