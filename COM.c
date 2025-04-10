
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
#include "MyMath.h"
#include "Hardware.h"

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
        //COM_Print( acCrossEvent );
        //SRL_Putc( ',' );  
        //COM_Print( acCrossIndex );
        //SRL_Putc( ',' );  
        //COM_Print( acCycleHalf );
        //SRL_Putc( ',' );          
        //COM_Print( mtrHallIndex );
        //SRL_Putc( ',' );  
        //COM_Print( mtrHallEvent );
        //SRL_Putc( ',' );      
        //COM_Print( mtrHallWDT );
        //SRL_Putc( ',' );              
        //COM_Print( ( U16 )tmTriac );
        //SRL_Putc( ',' );          
        //COM_Print( mtrSpeed );
        //SRL_Putc( ',' );          
        //COM_Print( sysStatus );     
        //SRL_Putc( ',' );      
        //COM_Print( statetemp );
        //SRL_Putc( ',' );           
        //COM_Print( ( U16 )timetemp );
        //COM_Print( sysTimer );
        //SRL_Putc( ',' );  
        //COM_Print( idrTest0 );
        //SRL_Putc( ',' );  
        //COM_Print( idrTest1 );
        //SRL_Putc( ',' );  
        //COM_Print( scnHall );
        //SRL_Putc( ',' );  
        //COM_Print( scnZero );
        //SRL_Putc( ',' );  
        //COM_Print( scnTest );
        //SRL_Putc( ',' );  
        //COM_Print( tmTriacOn );
        //SRL_Putc( ',' );  
        //COM_Print( tmTriacOff );

        SRL_Putc( '\r' );
        SRL_Putc( '\n' );
      }
  }
  void COM_Ctrl2( void )
  {
    /*
    static XRAM U8 t2 = 0;
    if ( ++t2 >= 100 )
      {
        t2 = 0;
        COM_Print( ( U16 )mtrLevel );
        //SRL_Putc( ',' );
        SRL_Putc( 'P' );
        //SRL_Putc( ',' );
        COM_Print( ( U16 )( pidKp&0x0000FFFF) );
        SRL_Putc( ',' );
        COM_Print( ( U16 )( pidKp>>16 ));
        //SRL_Putc( ',' );
        SRL_Putc( 'I' );
        //SRL_Putc( ',' );
        COM_Print( ( U16 )( pidKi&0x0000FFFF ) );
        SRL_Putc( ',' );
        COM_Print( ( U16 )( pidKi>>16 ));
        //SRL_Putc( ',' );
        SRL_Putc( 'D' );
        //SRL_Putc( ',' );
        COM_Print( ( U16 )( pidKd&0x0000FFFF ) );
        SRL_Putc( ',' );
        COM_Print( ( U16 )( pidKd>>16 ));
        SRL_Putc( ',' );    
      }
    */
  }


