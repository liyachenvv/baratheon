
#define _MAIN_SRC_
#include "Globals.h"
#include "COM.h"
#include "Motor.h"
#include "System.h"
#include "Service.h"

void main( void )
  {
    static U8 offTime = 0;
    __DI( );
    MCU_Init( );
    COM_Init( );
    MTR_Init( );
    SYS_Init( );
    SVC_Init( );
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
            SVC_Ctrl( );
            if ( sysStatus != E_SYS_OFF || e2pWrFlags )
              {
                offTime = 5;
                SYS_RELAY_ON( );
              }
            else if ( --offTime == 0 )
              {
                offTime = 1;
                SYS_RELAY_OFF( );
              }
          }
      }
  }

