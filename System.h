
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   System.h                         //
//                                                       //
///////////////////////////////////////////////////////////

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef _SYSTEM_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#define E_SYS_INIT      0
#define E_SYS_PREPARE   1
#define E_SYS_RUN       2
#define E_SYS_TURN_OFF  3
#define E_SYS_OFF       4
EXTERN XRAM U8  sysStatus;
//EXTERN XRAM U8  preAdvalue;
EXTERN XRAM U8  sysFault;
EXTERN XRAM U8  preStatus;
#define E_LVL_STOP      0
#define E_LVL_PULSE     1
#define E_LVL_1         2
#define E_LVL_2         3
#define E_LVL_3         4
#define E_LVL_4         5
#define E_LVL_5         6
#define E_LVL_6         7
EXTERN XRAM U8  sysLevel;

EXTERN XRAM U8  mtrLevel;

EXTERN void SYS_Init( void );
EXTERN void SYS_Ctrl( void );

#endif

