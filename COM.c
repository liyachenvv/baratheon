
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   COM.c                            //
//                                                       //
///////////////////////////////////////////////////////////

#define _COM_SRC_
#include "Globals.h"
#include "Service.h"
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

void COM_PrintH8( U8 n )
  {
    U8  c;
    c = n >> 4;
    if ( c < 10 ) c += '0'; else c += ( 'A' - 10 );
    SRL_Putc( c );
    c = n & 15;
    if ( c < 10 ) c += '0'; else c += ( 'A' - 10 );
    SRL_Putc( c );
    SRL_Putc( 'H' );
  }

void COM_PrintD8( U8 n )
  {
    U8  f = 0;
    if ( n >= 100 )
      {
        SRL_Putc( n / 100 + '0' );
        n %= 100;
        f = 1;
      }
    if ( f || n >= 10 )
      {
        SRL_Putc( n / 10 + '0' );
        n %= 10;
      }
    SRL_Putc( n + '0' );
  }

void COM_Init( void )
  {
  }

void COM_Ctrl( void )
  {
    static XRAM U8 t = 0;
    if ( ++t >= 50 )
      {
        t = 0;
        COM_PrintD8( mtrLevel );
        SRL_Putc( ',' );        COM_PrintD8( adVrs );
        SRL_Putc( ',' );        COM_PrintD8( adNtc );
        SRL_Putc( ',' );        COM_PrintD8( mtrPower );
        //SRL_Putc( ',' );        COM_PrintD8( mtrPowerAve );
        SRL_Putc( ',' );        COM_PrintD8( mtrTemp );
        SRL_Putc( ',' );        COM_PrintH8( idrTest0 );
        SRL_Putc( ',' );        COM_PrintH8( idrTest1 );
        //SRL_Putc( ',' );        COM_PrintH8( E2P_ReadByte( 0xFE ) );
        //SRL_Putc( ',' );        COM_PrintD8( E2P_ReadByte( 0x1B ) );
        SRL_Putc( '\r' );
        SRL_Putc( '\n' );
      }
  }

