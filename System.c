
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   System.c                         //
//                                                       //
///////////////////////////////////////////////////////////

#define _SYSTEM_SRC_
#include "Globals.h"
#include "System.h"
#include "Motor.h"
#include "Service.h"

#define SECOND  100UL
#define MINUTE  SECOND*60

void CAL_MtrLevel( void )
  {
    U8  ofs;
    U8  ad;
    ofs = SVC_GetTuneValue( );
    if ( ofs < 20 )
      {
        ofs = 20 - ofs;
        ad = adVrs + ofs;
        if ( ad < adVrs ) ad = 255;
      }
    else
      {
        ofs = ofs - 20;
        ad = adVrs - ofs;
        if ( ad > adVrs ) ad = 0;
      }
    if      ( !acCycleHalf )  mtrLevel = 0;
    else if ( ad <  12     )  mtrLevel = 10;
    else if ( ad <  42 - 1 )  mtrLevel = 1;
    else if ( ad <  42 + 1 )  mtrLevel = mtrLevel < 2 ? 1 : 2;
    else if ( ad <  94 - 1 )  mtrLevel = 2;
    else if ( ad <  94 + 1 )  mtrLevel = mtrLevel < 3 ? 2 : 3;
    else if ( ad < 117 - 1 )  mtrLevel = 3;
    else if ( ad < 117 + 1 )  mtrLevel = mtrLevel < 4 ? 3 : 4;
    else if ( ad < 137 - 1 )  mtrLevel = 4;
    else if ( ad < 137 + 1 )  mtrLevel = mtrLevel < 5 ? 4 : 5;
    else if ( ad < 158 - 1 )  mtrLevel = 5;
    else if ( ad < 158 + 1 )  mtrLevel = mtrLevel < 6 ? 5 : 6;
    else if ( ad < 184 - 1 )  mtrLevel = 6;
    else if ( ad < 184 + 1 )  mtrLevel = mtrLevel < 7 ? 6 : 7;
    else if ( ad < 209 - 1 )  mtrLevel = 7;
    else if ( ad < 209 + 1 )  mtrLevel = mtrLevel < 8 ? 7 : 8;
    else if ( ad < 240 - 1 )  mtrLevel = 8;
    else if ( ad < 240 + 1 )  mtrLevel = mtrLevel < 9 ? 8 : 9;
    else                      mtrLevel = 9;
  }

void CAL_TuneValue( void )
  {
    static XRAM U8 state = 0;
    static XRAM U16 time = 0;
    static XRAM U8 min = 0;
    static XRAM U8 max = 0;
    if ( time ) time--;
    switch ( state )
      {
        case 0:
            if ( acCycleHalf )
              {
                state++;
                time = 6;
              }
            break;
        case 1:
            if ( !time )
              {
                if ( mtrLevel == 10 )
                  {
                    state++;
                  }
                else
                  {
                    state = 255;
                  }
              }
            break;
        case 2:
            if ( mtrLevel == 1 )
              {
                state++;
                time = 300;
              }
            break;
        case 3:
            if ( mtrLevel != 1 )
              {
                state = 255;
              }
            else if ( time >= 100 )
              {
                min = adVrs;
                max = adVrs;
              }
            else
              {
                if ( adVrs < min ) min = adVrs;
                if ( adVrs > max ) max = adVrs;
                if ( !time )
                  {
                    if ( min < 10 || max >= 30 )
                      {
                        state = 255;
                      }
                    else
                      {
                        state++;
                      }
                  }
              }
            break;
        case 4:
            SVC_SetTuneValue( ( min + max ) >> 1 );
            state = 255;
            break;
        default:
            break;
      }
  }

void SYS_Init( void )
  {
    sysStatus = E_SYS_INIT;
    sysLevel  = E_LVL_STOP;
    sysFault  = 0;
    mtrLevel  = 0;
  }

void SYS_Ctrl( void )
  {
    static XRAM U32 time = 0;
    if(!mtrError) 
	{
	    CAL_MtrLevel( );
	    CAL_TuneValue( );
    }
    if ( time ) time--;
    switch ( sysStatus )
      {
        case E_SYS_INIT:
            mtrSpeedRef = 0;
            sysStatus = E_SYS_PREPARE;
            time = SECOND*5;
            break;
        case E_SYS_PREPARE:
            mtrSpeedRef = 0;
            if ( mtrError || !time )
              {
                sysFault = mtrError;
                sysStatus = E_SYS_TURN_OFF;
                time = SECOND/2;
              }
            else if ( idrTest0 == 0xFF && idrTest1 == 0xFF && mtrTemp >= 0 )
              {
                sysStatus = E_SYS_RUN;
                time = MINUTE*45;
              }
            break;
        case E_SYS_RUN:
            if ( mtrError || !time )
              {
                if(!time) {
                  if ( !mtrError ) 
                    mtrError = E_ERR_OVER_TIME;
                }
                sysFault = mtrError;
                sysStatus = E_SYS_TURN_OFF;
                //time = SECOND/2;
                time = 5;
              }
            else switch ( mtrLevel )
              {
                default:    // Stop
                    sysLevel = E_LVL_STOP;
                    mtrSpeedRef = 0;
                    break;
                case 1:     // Fold->Off 
                    sysLevel = E_LVL_STOP;
                    mtrSpeedRef = 0;
                    break;
                case 2:     // Min
                    sysLevel = E_LVL_1;
                    mtrSpeedRef = 2500;
                    break;
                case 3:     // 1
                    sysLevel = E_LVL_1;
                    mtrSpeedRef = 5000;
                    break;
                case 4:     // 2
                    sysLevel = E_LVL_2;
                    mtrSpeedRef = 6000;
                    break;
                case 5:     // 3
                    sysLevel = E_LVL_3;
                    mtrSpeedRef = 8000;
                    break;
                case 6:     // 4
                    sysLevel = E_LVL_4;
                    mtrSpeedRef = 9500;
                    break;
                case 7:     // 5
                    sysLevel = E_LVL_5;
                    mtrSpeedRef = 10500;
                    break;
                case 8:     // 6
                    sysLevel = E_LVL_6;
                    mtrSpeedRef = 12000;
                    break;
                case 9:     // Max
                    sysLevel = E_LVL_6;
                    mtrSpeedRef = 13500;
                    break;
                case 10:    // Pulse
                    sysLevel = E_LVL_PULSE;
                    mtrSpeedRef = 13501;
                    break;
              }
            break;
        default:
            if ( !time ) sysStatus = E_SYS_OFF;
            break;
      }
  }
  
