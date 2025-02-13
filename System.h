
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
EXTERN U8  sysStatus;

EXTERN U8  sysLevel;
EXTERN U8  sysError;

EXTERN void SYS_Init( void );
EXTERN void SYS_Ctrl( void );

#endif

