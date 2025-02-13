
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Uart.c                           //
//                                                       //
///////////////////////////////////////////////////////////

#define _UART_SRC_
#include "Globals.h"
#include "Uart.h"

U8   SRL_Getc( U8 * c )
  {
    if ( !rxCNT ) return 0;
    *c = rxFIFO[ rxRDP & 31 ];
    rxRDP++;
    rxCNT--;
    return 0;
  }
  
U8   SRL_Gets( U8 * s )
  {
    U8 n = 0;
    if ( rxTIM >= 18 )
      {
        while ( rxCNT )
          {
            *s = rxFIFO[ rxRDP & 31 ];
            rxRDP++;
            rxCNT--;
            s++;
            n++;
          }
      }
    return n;
  }
  
U8   SRL_Putc( U8 c )
  {
    if ( txCNT >= 32 ) return 0;
    txFIFO[ txWRP & 31 ] = c;
    txWRP++;
    txCNT++;
    return 1;
  }
  
U8   SRL_Puts( U8 * s, U8 n )
  {
    if ( txTIM >= 18 )
      {
        while ( txCNT < 32 && n != 0 )
          {
            txFIFO[ txWRP & 31 ] = *s;
            txWRP++;
            txCNT++;
            s++;
            n--;
          }
      }
    if ( n != 0 ) return 0;
    return 1;
  }

