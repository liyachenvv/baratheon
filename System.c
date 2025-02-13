
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

#define NO_LEVEL_SPEED  1

#define SECOND  100UL
#define MINUTE  SECOND*60

CROM U16 sLVL[ ] = 
  {
    0,      // 0  STOP
    2500,   // 1  FOLD
    3000,   // 2  MIN
    4000,   // 3  1
    6000,   // 4  2
    8000,   // 5  3
    9500,   // 6  4
    10500,  // 7  5
    12000,  // 8  6
    13500,  // 9  MAX
    13501   // 10 PULSE
  };

static bank2 U16 speedSet = 0;

void CAL_MtrLevel( void )
  {
    U8  ofs;
    U8  ad;
    ofs = SVC_GetTuneValue( );
    if ( ofs < DEFAULT_TUNE_VALUE )
      {
        ofs = DEFAULT_TUNE_VALUE - ofs;
        ad = adVrs + ofs;
        if ( ad < adVrs ) ad = 255;
      }
    else
      {
        ofs = ofs - DEFAULT_TUNE_VALUE;
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

    #if  NO_LEVEL_SPEED
    if ( ad < 12 )
      {
        speedSet = sLVL[ 10 ];
      }
    else if ( ad < 32 )
      {
        speedSet = sLVL[ 1 ];
      }
    else if ( ad < 68 )
      {
        speedSet = sLVL[ 1 ] + ( U16 )( MULU32( sLVL[ 2 ]-sLVL[ 1 ], ad-32 ) / ( 68-32 ) );
      }
    else if ( ad < 106 )
      {
        speedSet = sLVL[ 2 ] + ( U16 )( MULU32( sLVL[ 3 ]-sLVL[ 2 ], ad-68 ) / ( 106-68 ) );
      }
    else if ( ad < 127 )
      {
        speedSet = sLVL[ 3 ] + ( U16 )( MULU32( sLVL[ 4 ]-sLVL[ 3 ], ad-106 ) / ( 127-106 ) );
      }
    else if ( ad < 148 )
      {
        speedSet = sLVL[ 4 ] + ( U16 )( MULU32( sLVL[ 5 ]-sLVL[ 4 ], ad-127 ) / ( 148-127 ) );
      }
    else if ( ad < 171 )
      {
        speedSet = sLVL[ 5 ] + ( U16 )( MULU32( sLVL[ 6 ]-sLVL[ 5 ], ad-148 ) / ( 171-148 ) );
      }
    else if ( ad < 197 )
      {
        speedSet = sLVL[ 6 ] + ( U16 )( MULU32( sLVL[ 7 ]-sLVL[ 6 ], ad-171 ) / ( 197-171 ) );
      }
    else if ( ad < 225 )
      {
        speedSet = sLVL[ 7 ] + ( U16 )( MULU32( sLVL[ 8 ]-sLVL[ 7 ], ad-197 ) / ( 225-197 ) );
      }
    else if ( ad < 240 )
      {
        speedSet = sLVL[ 8 ] + ( U16 )( MULU32( sLVL[ 9 ]-sLVL[ 8 ], ad-225 ) / ( 240-225 ) );
      }
    else
      {
        speedSet = sLVL[ 9 ];
      }
    #else
    speedSet = sLVL[ mtrLevel ];
    #endif
    
  }

void CAL_TuneValue( void )
  {
    static XRAM U8  state = 0;
    static XRAM U16 time = 0;
    static XRAM U8  min = 0;
    static XRAM U8  max = 0;
    if ( time ) time--;
    switch ( state )
      {
        case 0:
            if ( acCycleHalf )
              {
                state++;
                time = 10;
              }
            break;
        case 1:
            if ( mtrLevel == 10 )
              {
                state++;
              }
            else if ( !time )
              {
                state = 255;
              }
            break;
        case 2:
            if ( mtrLevel == 1 )
              {
                state++;
                time = 300;
              }
            else if ( mtrLevel != 10 )
              {
                state = 255;
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
    CAL_MtrLevel( );
    CAL_TuneValue( );
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
                time = SECOND*5;
              }
            else if ( idrTest0 == 0xFF && idrTest1 == 0xFF && mtrTemp >= 0 )
              {
                sysStatus = E_SYS_RUN;
                time = MINUTE*30;
              }
            break;
        case E_SYS_RUN:
            if ( mtrError || !time )
              {
                mtrSpeedRef = 0;
                sysFault = mtrError;
                sysStatus = E_SYS_TURN_OFF;
                time = SECOND*5;
              }
            else switch ( mtrLevel )
              {
                default:    // Stop
                    sysLevel = E_LVL_STOP;
                    mtrSpeedRef = 0;
                    break;
                case 1:     // Fold
                    sysLevel = E_LVL_1;
                    mtrSpeedRef = speedSet;
                    break;
                case 2:     // Min
                    sysLevel = E_LVL_1;
                    mtrSpeedRef = speedSet;
                    break;
                case 3:     // 1
                    sysLevel = E_LVL_1;
                    mtrSpeedRef = speedSet;
                    break;
                case 4:     // 2
                    sysLevel = E_LVL_2;
                    mtrSpeedRef = speedSet;
                    break;
                case 5:     // 3
                    sysLevel = E_LVL_3;
                    mtrSpeedRef = speedSet;
                    break;
                case 6:     // 4
                    sysLevel = E_LVL_4;
                    mtrSpeedRef = speedSet;
                    break;
                case 7:     // 5
                    sysLevel = E_LVL_5;
                    mtrSpeedRef = speedSet;
                    break;
                case 8:     // 6
                    sysLevel = E_LVL_6;
                    mtrSpeedRef = speedSet;
                    break;
                case 9:     // Max
                    sysLevel = E_LVL_6;
                    mtrSpeedRef = speedSet;
                    break;
                case 10:    // Pulse
                    sysLevel = E_LVL_PULSE;
                    mtrSpeedRef = 13501;
                    break;
              }
            break;
        default:
            mtrSpeedRef = 0;
            if ( !time ) sysStatus = E_SYS_OFF;
            break;
      }
  }
  
