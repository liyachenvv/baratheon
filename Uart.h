
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Uart.h                           //
//                                                       //
///////////////////////////////////////////////////////////

#ifndef _UART_H_
#define _UART_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef _UART_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN U8   SRL_Getc( U8 *c );
EXTERN U8   SRL_Gets( U8 * s );
EXTERN U8   SRL_Putc( U8 c );
EXTERN U8   SRL_Puts( U8 * s, U8 n );

#endif

