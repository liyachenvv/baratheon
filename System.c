
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   System.c                         //
//                                                       //
///////////////////////////////////////////////////////////

#define _SYSTEM_SRC_
#include "Globals.h"
#include "System.h"
#include "Motor.h"

void SYS_Init( void )
  {
    sysStatus = E_SYS_INIT;
    sysLevel = 0;
    sysError = 0;
  }

void SYS_Ctrl( void )
  {
    static U8 time = 0;
    if ( time ) time--;
    switch ( sysStatus )
      {
        default:
            sysStatus = E_SYS_INIT;
        case E_SYS_INIT:
            SYS_RELAY_ON( );
            sysLevel = 0;
            sysStatus = E_SYS_PREPARE;
            time = 200;
            break;
        case E_SYS_PREPARE:
            SYS_RELAY_ON( );
            sysLevel = 0;
            if ( idrTest0 == 0xFF
              && idrTest1 == 0xFF
              && adNtc < 254
              && adNtc >= 2 )
              {
                sysStatus = E_SYS_RUN;
              }
            else if ( !time )
              {
                sysStatus = E_SYS_TURN_OFF;
              }
            break;
        case E_SYS_RUN:
            SYS_RELAY_ON( );
            if ( mtrError )
              {
                sysLevel = 0;
                sysStatus = E_SYS_TURN_OFF;
                time = 20;
              }
            else if ( adVrs < 10 )
              {
                sysLevel = 10;
              }
            else if ( adVrs < 40 - 1 )
              {
                sysLevel = 1;
              }
            else if ( adVrs < 40 + 1 )
              {
                sysLevel = sysLevel < 2 ? 1 : 2;
              }
            else if ( adVrs < 92 - 1 )
              {
                sysLevel = 2;
              }
            else if ( adVrs < 92 + 1 )
              {
                sysLevel = sysLevel < 3 ? 2 : 3;
              }
            else if ( adVrs < 117 - 1 )
              {
                sysLevel = 3;
              }
            else if ( adVrs < 117 + 1 )
              {
                sysLevel = sysLevel < 4 ? 3 : 4;
              }
            else if ( adVrs < 139 - 1 )
              {
                sysLevel = 4;
              }
            else if ( adVrs < 139 + 1 )
              {
                sysLevel = sysLevel < 5 ? 4 : 5;
              }
            else if ( adVrs < 162 - 1 )
              {
                sysLevel = 5;
              }
            else if ( adVrs < 162 + 1 )
              {
                sysLevel = sysLevel < 6 ? 5 : 6;
              }
            else if ( adVrs < 187 - 1 )
              {
                sysLevel = 6;
              }
            else if ( adVrs < 187 + 1 )
              {
                sysLevel = sysLevel < 7 ? 6 : 7;
              }
            else if ( adVrs < 214 - 1 )
              {
                sysLevel = 7;
              }
            else if ( adVrs < 214 + 1 )
              {
                sysLevel = sysLevel < 8 ? 7 : 8;
              }
            else if ( adVrs < 241 - 1 )
              {
                sysLevel = 8;
              }
            else if ( adVrs < 241 + 1 )
              {
                sysLevel = sysLevel < 9 ? 8 : 9;
              }
            else
              {
                sysLevel = 9;
              }
            break;
        case E_SYS_TURN_OFF:
            sysLevel = 0;
            if ( !time )
              {
                SYS_RELAY_OFF( );
              }
            break;
      }
    switch ( sysLevel )
      {
        default:  mtrSpeedRef = 0;       break;
        case 1:   mtrSpeedRef = 2500;    break;  // Fold
        case 2:   mtrSpeedRef = 3000;    break;  // Min
        case 3:   mtrSpeedRef = 5000;    break;  // 1
        case 4:   mtrSpeedRef = 6500;    break;  // 2
        case 5:   mtrSpeedRef = 8000;    break;  // 3
        case 6:   mtrSpeedRef = 9500;    break;  // 4
        case 7:   mtrSpeedRef = 10500;   break;  // 5
        case 8:   mtrSpeedRef = 12000;   break;  // 6
        case 9:   mtrSpeedRef = 13500;   break;  // Max
        case 10:  mtrSpeedRef = 13501;   break;  // Pulse
      }
  }
  
