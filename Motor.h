
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

#define IREG_KP   8000
#define IREG_KI   2000
#define SREG_KP   1000
#define SREG_KI   50

EXTERN U16 mtrSpeedRef;
EXTERN U16 mtrSpeed;
EXTERN U16 mtrCurrent;

#define E_ERR_NO_CROSS      1
#define E_ERR_ZERO_SPEED    2
#define E_ERR_OVER_CURRENT  3
#define E_ERR_TPMR_FAULT    4
EXTERN U8  mtrError;

EXTERN void MTR_Init( void );
EXTERN void MTR_Ctrl( void );

#endif

