
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   COM.h                            //
//                                                       //
///////////////////////////////////////////////////////////

#ifndef _COM_H_
#define _COM_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef _COM_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN void COM_Init( void );
EXTERN void COM_Ctrl( void );
EXTERN void COM_Ctrl2( void );
#endif

