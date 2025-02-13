
#define _MAIN_SRC_
#include "Globals.h"
#include "COM.h"
#include "Motor.h"
#include "System.h"

void main( void )
  {
    __DI( );
    MCU_Init( );
    COM_Init( );
    MTR_Init( );
    SYS_Init( );
    while ( 1 )
      {
        __EI( );
        __WDTC( );
        if ( sysTicks >= 100 )
          {
            sysTicks -= 100;
            MCU_Refresh( );
            COM_Ctrl( );
            MTR_Ctrl( );
            SYS_Ctrl( );
          }
      }
  }

