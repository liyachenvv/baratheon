
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   COM.c                            //
//                                                       //
///////////////////////////////////////////////////////////

#define _COM_SRC_
#include "Globals.h"
#include "System.h"
#include "Motor.h"
#include "Uart.h"
#include "COM.h"

void COM_Print( U16 n )
  {
    U8  f = 0;
    if ( n >= 10000 )
      {
        SRL_Putc( '0' + ( U8 )( n / 10000 ) );
        n %= 10000;
        f = 1;
      }
    if ( f || n >= 1000 )
      {
        SRL_Putc( '0' + ( U8 )( n / 1000 ) );
        n %= 1000;
        f = 1;
      }
    if ( f || n >= 100 )
      {
        SRL_Putc( '0' + ( U8 )( n / 100 ) );
        n %= 100;
        f = 1;
      }
    if ( f || n >= 10 )
      {
        SRL_Putc( '0' + ( U8 )( n / 10 ) );
        n %= 10;
      }
    SRL_Putc( '0' + ( U8 )( n ) );
  }

void COM_Init( void )
  {
  }

void COM_Ctrl( void )
  {
    static XRAM U8 t = 0;
    if ( ++t >= 100 )
      {
        t = 0;
        COM_Print( ( U16 )mtrLevel );
        SRL_Putc( ',' );
        COM_Print( ( U16 )adVrs );
        SRL_Putc( ',' );
        COM_Print( ( U16 )adNtc );
        SRL_Putc( ',' );
        COM_Print( mtrSpeedRef );
        SRL_Putc( ',' );
        COM_Print( mtrSpeed );
        SRL_Putc( ',' );
        COM_Print( mtrCurrent );
        SRL_Putc( ',' );
        COM_Print( ( U16 )mtrTemp );
        SRL_Putc( ',' );
        COM_Print( ( U16 )E2P_ReadByte( 0xFE ) );
        SRL_Putc( '\r' );
        SRL_Putc( '\n' );
      }
  }

