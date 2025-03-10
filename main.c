
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
        if ( sysTicks >= 100 )    //10ms  
          {
            sysTicks -= 100;  //sysTicks every 100us inc 1.  100us*100=10ms
            MCU_Refresh( );
            COM_Ctrl( );
            MTR_Ctrl( );
            SYS_Ctrl( );
            SVC_Ctrl( );
            if ( sysStatus != E_SYS_OFF || e2pWrFlags )  // normal status
              {
                offTime = 5;  //every 10ms refresh offtime. 
                SYS_RELAY_ON( );
              }
            else if ( --offTime == 0 )  //E_SYS_off longer than 50ms
              {
                offTime = 1;
                SYS_RELAY_OFF( );
              }
          }
      }
  }

