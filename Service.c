
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Service.c                        //
//                                                       //
///////////////////////////////////////////////////////////

#define  _SERVICE_SRC_
#include "Globals.h"
#include "Service.h"
#include "System.h"
#include "Motor.h"

__EEPROM_DATA( 
    PRODUCT_NAME_0,
    PRODUCT_NAME_1,
    PRODUCT_NAME_2,
    PRODUCT_NUM>>8,
    PRODUCT_NUM&255,
    PCBA_NUM_0,
    PCBA_NUM_1,
    PCBA_NUM_2);
__EEPROM_DATA(
    PCBA_NUM_3,
    PCBA_NUM_4,
    PCBA_NUM_5,
    PCBA_NUM_6,
    SOFTWARE_NUM_0,
    SOFTWARE_NUM_1,
    SOFTWARE_NUM_2,
    SOFTWARE_NUM_3);
__EEPROM_DATA(
    SOFTWARE_NUM_4,
    SOFTWARE_NUM_5,
    SOFTWARE_NUM_6,
    SOFTWARE_VER_0,
    '.',
    SOFTWARE_VER_1,
    '.',
    SOFTWARE_VER_2);
__EEPROM_DATA(
    SERIAL_NUM_0,
    SERIAL_NUM_1,
    SERIAL_NUM_2,
    0,
    0,
    0,
    0,
    0);
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );
__EEPROM_DATA( 
    0,
    0,
    0,
    0,
    0,
    0,
    TUNE_VALUE_DEFAULT,
    0 );

#define REC_SPEED_WORKING_INFO    0X01
#define REC_LAST_USE_INFO         0X02
#define REC_FAULT_INFO            0X04
#define REC_TEMP_INFO             0X08
#define REC_WORKING_INFO          0X10
#define REC_KEY_WORKING_INFO      0X20
#define REC_INTERLOCK_INFO        0X40
#define REC_TUNE_INFO             0X80

typedef struct { U8 s; U8 m; }                M1S1;
typedef struct { U8 s; U8 m; U8 h; }          H1M1S1;
typedef struct { U8 s; U8 m; U8 h; U8 h1; }   H2M1S1;
typedef struct { U8 m; U8 h; U8 h1; U8 h2; }  H3M1;
typedef struct { U8 s; H3M1 h3m1; }           H3M1S1;
typedef struct { U8 l; U8 h; }                COUNT;

#define LAST_SPEED_SETTING_ADDR         0X23
static XRAM U8     lastSpeedSetting[ 7 ]        @ 0X420;
#define LAST_SPEED_WORKING_TIME_ADDR    0X38
static XRAM H1M1S1 lastSpeedWorkingTime[ 7 ]    @ 0X428;
#define TOTAL_SPEED_WORKING_TIME_ADDR   0X8F
static XRAM H2M1S1 totalSpeedWorkingTime[ 7 ]   @ 0X440;

#define LAST_USE_MAX_POWER_ADDR         0X2A
static XRAM U8     lastUseMaxPower[ 7 ]         @ 0X4A0;
#define LAST_USE_AVE_POWER_ADDR         0X31
static XRAM U8     lastUseAvePower[ 7 ]         @ 0X4A8;
#define LAST_USE_READY_TIME_ADDR        0X54
static XRAM M1S1   lastUseReadyTime[ 7 ]        @ 0X4B0;

#define FAULT_CODE_ADDR                 0X1C
static XRAM U8     faultCode[ 7 ]               @ 0X520;
#define FAULT_TOTAL_COUNT_ADDR          0X86
static XRAM U8     faultTotalCount[ 9 ]         @ 0X527;
#define FAULT_TIME_STAMP_ADDR           0X62
static XRAM H3M1   faultTimeStamp[ 7 ]          @ 0X530;

#define MAX_MCU_TEMP_ADDR               0X7E
static XRAM U8     maxMcuTemp[ 7 ]              @ 0X550;
#define MAX_MOTOR_HISTORY_TEMP_ADDR     0X85
static XRAM U8     maxMotorHistoryTemp          @ 0X557;
#define LAST_USE_MAX_MOTOR_TEMP_ADDR    0X4D
static XRAM U8     lastUseMaxMotorTemp[ 7 ]     @ 0X558;

#define TOTAL_POWER_ON_TIME_ADDR        0XD7
static XRAM H3M1   totalPowerOnTime             @ 0X5A0;
#define TOTAL_STANDBY_TIME_ADDR         0XDF
static XRAM H3M1   totalStandbyTime             @ 0X5A4;
#define TOTAL_WORKING_TIME_ADDR         0XD3
static XRAM H3M1   totalWorkingTime             @ 0X5A8;

#define AUTO_WORKING_TIME_ADDR          0XAB
static XRAM H2M1S1 autoWorkingTime              @ 0X620;
#define KEY_WORKING_TIME_ADDR           0XAF
static XRAM H2M1S1 keyWorkingTime[ 9 ]          @ 0X624;
#define TOTAL_AUTO_PRESSED_COUNT_ADDR   0XE5
static XRAM COUNT  totalAutoPressedCount        @ 0X650;
#define TOTAL_KEY_PRESSED_COUNT_ADDR    0XE7
static XRAM COUNT  totalKeyPressedCount[ 9 ]    @ 0X654;

#define TOTAL_NO_INTERLOCK_TIME_ADDR    0XDB
static XRAM H3M1   totalNoInterlockTime         @ 0X658;
#define TOTAL_INTERLOCK_OPEN_COUNT_ADDR 0XE3
static XRAM COUNT  totalInterlockOpenCount      @ 0X65C;

#define TUNE_INFO_ADDR                  0XFE
static XRAM U8     tuneValue;

static XRAM H3M1S1 powerOnTime;

void E2P_RD( U8 addr, U8 * p, U8 n )
  {
    while ( n-- )
      {
        *p = E2P_ReadByte( addr++ );
        p++;
      }
  }

U8   E2P_WR( U8 addr, U8 * p, U8 n )
  {
    while ( n )
      {
        n--;
        if ( *p == E2P_ReadByte( addr ) )
          {
            addr++;
            p++;
          }
        else
          {
            E2P_WriteByte( addr, *p );
            break;
          }
      }
    if ( n ) return 0;
    return 1;
  }

void MEM_Clr( U8 * p, U8 n )
  {
    while ( n-- )
      {
        *p = 0;
        p++;
      }
  }

void MEM_Cpy( U8 * p1, U8 * p2, U8 n )
  {
    while ( n-- )
      {
        *p1 = *p2;
        p1++;
        p2++;
      }
  }

S8   MEM_Cmp( U8 * p1, U8 * p2, U8 n )
  {
    while ( n )
      {
        n--;
        if ( p1[ n ] < p2[ n ] ) return -1;
        if ( p2[ n ] < p1[ n ] ) return 1;
      }
    return 0;
  }

void M1S1_ADD( M1S1 * p, U8 s )
  {
    p->s += s;
    if ( p->s >= 60 )
      {
        p->s -= 60;
        p->m += 1;
      }
  }

void M1S1_SUB( M1S1 * p, U8 s )
  {
    p->s -= s;
    if ( p->s >= 60 )
      {
        p->s += 60;
        p->m -= 1;
      }
  }

void H1M1S1_ADD( H1M1S1 * p, U8 s )
  {
    p->s += s;
    if ( p->s >= 60 )
      {
        p->s -= 60;
        p->m += 1;
        if ( p->m >= 60 )
          {
            p->m -= 60;
            p->h += 1;
          }
      }
  }

void H1M1S1_SUB( H1M1S1 * p, U8 s )
  {
    p->s -= s;
    if ( p->s >= 60 )
      {
        p->s += 60;
        p->m -= 1;
        if ( p->m >= 60 )
          {
            p->m += 60;
            p->h -= 1;
          }
      }
  }

void H2M1S1_ADD( H2M1S1 * p, U8 s )
  {
    p->s += s;
    if ( p->s >= 60 )
      {
        p->s -= 60;
        p->m += 1;
        if ( p->m >= 60 )
          {
            p->m -= 60;
            p->h += 1;
            if ( p->h == 0 ) p->h1 += 1;
          }
      }
  }
  
void H2M1S1_SUB( H2M1S1 * p, U8 s )
  {
    p->s -= s;
    if ( p->s >= 60 )
      {
        p->s += 60;
        p->m -= 1;
        if ( p->m >= 60 )
          {
            p->m += 60;
            p->h -= 1;
            if ( p->h == 255 ) p->h1 -= 1;
          }
      }
  }

void H3M1_ADD( H3M1 * p, U8 m )
  {
    p->m += m;
    if ( p->m >= 60 )
      {
        p->m -= 60;
        p->h += 1;
        if ( p->h == 0 )
          {
            p->h1 += 1;
            if ( p->h1 == 0 ) p->h2 += 1;
          }
      }
  }

void H3M1_SUB( H3M1 * p, U8 m )
  {
    p->m -= m;
    if ( p->m >= 60 )
      {
        p->m += 60;
        p->h -= 1;
        if ( p->h == 255 )
          {
            p->h1 -= 1;
            if ( p->h1 == 255 ) p->h2 -= 1;
          }
      }
  }
  
#define E2P_RD_EX( ADDR, M )  E2P_RD( ADDR, ( U8* )&M, sizeof( M ) )
#define E2P_WR_EX( ADDR, M )  E2P_WR( ADDR, ( U8* )&M, sizeof( M ) )

void SVC_Init( void )
  {
    S8  cmp;
    e2pWrFlags = 0x00;
    powerOnTime.s = 30;
    powerOnTime.h3m1.m = 0;
    powerOnTime.h3m1.h = 0;
    powerOnTime.h3m1.h1 = 0;
    powerOnTime.h3m1.h2 = 0;
    E2P_RD_EX( LAST_SPEED_SETTING_ADDR,         lastSpeedSetting         );
    E2P_RD_EX( LAST_SPEED_WORKING_TIME_ADDR,    lastSpeedWorkingTime     );
    E2P_RD_EX( TOTAL_SPEED_WORKING_TIME_ADDR,   totalSpeedWorkingTime    );
    E2P_RD_EX( LAST_USE_MAX_POWER_ADDR,         lastUseMaxPower          );
    E2P_RD_EX( LAST_USE_AVE_POWER_ADDR,         lastUseAvePower          );
    E2P_RD_EX( LAST_USE_READY_TIME_ADDR,        lastUseReadyTime         );
    E2P_RD_EX( FAULT_CODE_ADDR,                 faultCode                );
    E2P_RD_EX( FAULT_TOTAL_COUNT_ADDR,          faultTotalCount          );
    E2P_RD_EX( FAULT_TIME_STAMP_ADDR,           faultTimeStamp           );
    E2P_RD_EX( MAX_MCU_TEMP_ADDR,               maxMcuTemp               );
    E2P_RD_EX( MAX_MOTOR_HISTORY_TEMP_ADDR,     maxMotorHistoryTemp      );
    E2P_RD_EX( LAST_USE_MAX_MOTOR_TEMP_ADDR,    lastUseMaxMotorTemp      );
    E2P_RD_EX( TOTAL_POWER_ON_TIME_ADDR,        totalPowerOnTime         );
    E2P_RD_EX( TOTAL_STANDBY_TIME_ADDR,         totalStandbyTime         );
    E2P_RD_EX( TOTAL_WORKING_TIME_ADDR,         totalWorkingTime         );
    E2P_RD_EX( AUTO_WORKING_TIME_ADDR,          autoWorkingTime          );
    E2P_RD_EX( KEY_WORKING_TIME_ADDR,           keyWorkingTime           );
    E2P_RD_EX( TOTAL_AUTO_PRESSED_COUNT_ADDR,   totalAutoPressedCount    );
    E2P_RD_EX( TOTAL_KEY_PRESSED_COUNT_ADDR,    totalKeyPressedCount     );
    E2P_RD_EX( TOTAL_NO_INTERLOCK_TIME_ADDR,    totalNoInterlockTime     );
    E2P_RD_EX( TOTAL_INTERLOCK_OPEN_COUNT_ADDR, totalInterlockOpenCount  );
    E2P_RD_EX( TUNE_INFO_ADDR,                  tuneValue                );
    cmp = MEM_Cmp( ( U8* )&totalPowerOnTime, ( U8* )&totalWorkingTime, sizeof( H3M1 ) );
    if ( cmp < 0 )
      {
        totalPowerOnTime.h2 = totalWorkingTime.h2;
        totalPowerOnTime.h1 = totalWorkingTime.h1;
        totalPowerOnTime.h = totalWorkingTime.h;
        totalPowerOnTime.m = totalWorkingTime.m;
        e2pWrFlags |= REC_WORKING_INFO;
      }
    else if ( cmp > 0 )
      {
        totalWorkingTime.h2 = totalPowerOnTime.h2;
        totalWorkingTime.h1 = totalPowerOnTime.h1;
        totalWorkingTime.h = totalPowerOnTime.h;
        totalWorkingTime.m = totalPowerOnTime.m;
        e2pWrFlags |= REC_WORKING_INFO;
      }
  }

U8   SVC_Timer( void )
  {
    static XRAM U8  tmSEC = 0;
    if ( ++tmSEC >= 100 )
      {
        tmSEC = 0;
        if ( ++powerOnTime.s >= 60 )
          {
            powerOnTime.s = 0;
            H3M1_ADD( &powerOnTime.h3m1, 1 );
            H3M1_ADD( &totalPowerOnTime, 1 );
            MEM_Cpy( ( U8* )&totalWorkingTime, ( U8* )&totalPowerOnTime, sizeof( H3M1 ) );
            e2pWrFlags |= REC_WORKING_INFO;
          }
        return 1;
      }
    return 0;
  }

void REC_SpeedWorking( void )
  {
    static XRAM U8     level = 0;
    static XRAM U8     lsLvl = 0;
    static XRAM U8     cnLvl = 0;
    static XRAM H1M1S1 tmLvl = { 0, 0, 0 };
    H1M1S1_ADD( &tmLvl, 1 );
    if ( sysLevel != lsLvl )
      {
        lsLvl = sysLevel;
        cnLvl = 0;
      }
    else if ( cnLvl < 2 )
      {
        cnLvl++;
      }
    else if ( sysLevel != level )
      {
        H1M1S1_SUB( &tmLvl, 3 );
        if ( level && level < 8 )
          {
            MEM_Cpy( ( U8* )&lastSpeedWorkingTime[ 0 ], ( U8* )&tmLvl, sizeof( H1M1S1 ) );
            if ( tmLvl.s < 30 )
              {
                H2M1S1_ADD( &totalSpeedWorkingTime[ level - 1 ], tmLvl.s );
              }
            else
              {
                H2M1S1_ADD( &totalSpeedWorkingTime[ level - 1 ], tmLvl.s - 30 );
              }
            e2pWrFlags |= REC_SPEED_WORKING_INFO;
          }
        level = sysLevel;
        tmLvl.s = 3;
        tmLvl.m = 0;
        tmLvl.h = 0;
        if ( level && level < 8 )
          {
            lastSpeedSetting[ 6 ] = lastSpeedSetting[ 5 ];
            lastSpeedSetting[ 5 ] = lastSpeedSetting[ 4 ];
            lastSpeedSetting[ 4 ] = lastSpeedSetting[ 3 ];
            lastSpeedSetting[ 3 ] = lastSpeedSetting[ 2 ];
            lastSpeedSetting[ 2 ] = lastSpeedSetting[ 1 ];
            lastSpeedSetting[ 1 ] = lastSpeedSetting[ 0 ];
            lastSpeedSetting[ 0 ] = 0XF0 + level;
            MEM_Cpy( ( U8* )&lastSpeedWorkingTime[ 6 ], ( U8* )&lastSpeedWorkingTime[ 5 ], sizeof( H1M1S1 ) );
            MEM_Cpy( ( U8* )&lastSpeedWorkingTime[ 5 ], ( U8* )&lastSpeedWorkingTime[ 4 ], sizeof( H1M1S1 ) );
            MEM_Cpy( ( U8* )&lastSpeedWorkingTime[ 4 ], ( U8* )&lastSpeedWorkingTime[ 3 ], sizeof( H1M1S1 ) );
            MEM_Cpy( ( U8* )&lastSpeedWorkingTime[ 3 ], ( U8* )&lastSpeedWorkingTime[ 2 ], sizeof( H1M1S1 ) );
            MEM_Cpy( ( U8* )&lastSpeedWorkingTime[ 2 ], ( U8* )&lastSpeedWorkingTime[ 1 ], sizeof( H1M1S1 ) );
            MEM_Cpy( ( U8* )&lastSpeedWorkingTime[ 1 ], ( U8* )&lastSpeedWorkingTime[ 0 ], sizeof( H1M1S1 ) );
            MEM_Cpy( ( U8* )&lastSpeedWorkingTime[ 0 ], ( U8* )&tmLvl, sizeof( H1M1S1 ) );
            H2M1S1_ADD( &totalSpeedWorkingTime[ level - 1 ], 3 );
            e2pWrFlags |= REC_SPEED_WORKING_INFO;
          }
      }
    else
      {
        if ( level && level < 8 && ( tmLvl.s == 0 || tmLvl.s == 30 ) )
          {
            MEM_Cpy( ( U8* )&lastSpeedWorkingTime[ 0 ], ( U8* )&tmLvl, sizeof( H1M1S1 ) );
            if ( tmLvl.h == 0 && tmLvl.m == 0 && tmLvl.s == 30 )
              {
                H2M1S1_ADD( &totalSpeedWorkingTime[ level - 1 ], 27 );
              }
            else
              {
                H2M1S1_ADD( &totalSpeedWorkingTime[ level - 1 ], 30 );
              }
            e2pWrFlags |= REC_SPEED_WORKING_INFO;
          }
      }
  }

void REC_WorkingPower( void )
  {
  }

void REC_WorkingTemp( void )
  {
    static XRAM S8  temp = 0;
    static XRAM U8  next = 0;
    if ( sysLevel == 0 )
      {
        next = 0;
        temp = mtrTemp;
      }
    else
      {
        if ( maxMotorHistoryTemp < mtrTemp )
          {
            maxMotorHistoryTemp = mtrTemp;
            e2pWrFlags |= REC_TEMP_INFO;
          }
        if ( temp < mtrTemp )
          {
            temp = mtrTemp;
          }
        if ( powerOnTime.s == 0 )
          {
            if ( next == 0 )
              {
                next = 1;
                lastUseMaxMotorTemp[ 6 ] = lastUseMaxMotorTemp[ 5 ];
                lastUseMaxMotorTemp[ 5 ] = lastUseMaxMotorTemp[ 4 ];
                lastUseMaxMotorTemp[ 4 ] = lastUseMaxMotorTemp[ 3 ];
                lastUseMaxMotorTemp[ 3 ] = lastUseMaxMotorTemp[ 2 ];
                lastUseMaxMotorTemp[ 2 ] = lastUseMaxMotorTemp[ 1 ];
                lastUseMaxMotorTemp[ 1 ] = lastUseMaxMotorTemp[ 0 ];
              }
            lastUseMaxMotorTemp[ 0 ] = temp;
            e2pWrFlags |= REC_TEMP_INFO;
          }
      }
  }

void REC_Faults( void )
  {
    static XRAM U8  fault = 0;
    if ( !sysFault || sysFault >= 10 )
      {
        fault = 0;
      }
    else if ( fault != sysFault )
      {
        fault = sysFault;
        faultCode[ 6 ] = faultCode[ 5 ];
        faultCode[ 5 ] = faultCode[ 4 ];
        faultCode[ 4 ] = faultCode[ 3 ];
        faultCode[ 3 ] = faultCode[ 2 ];
        faultCode[ 2 ] = faultCode[ 1 ];
        faultCode[ 1 ] = faultCode[ 0 ];
        faultCode[ 0 ] = 0xE0 + sysFault;
        faultTotalCount[ sysFault - 1 ]++;
        MEM_Cpy( ( U8* )&faultTimeStamp[ 6 ], ( U8* )&faultTimeStamp[ 5 ], sizeof( H3M1 ) );
        MEM_Cpy( ( U8* )&faultTimeStamp[ 5 ], ( U8* )&faultTimeStamp[ 4 ], sizeof( H3M1 ) );
        MEM_Cpy( ( U8* )&faultTimeStamp[ 4 ], ( U8* )&faultTimeStamp[ 3 ], sizeof( H3M1 ) );
        MEM_Cpy( ( U8* )&faultTimeStamp[ 3 ], ( U8* )&faultTimeStamp[ 2 ], sizeof( H3M1 ) );
        MEM_Cpy( ( U8* )&faultTimeStamp[ 2 ], ( U8* )&faultTimeStamp[ 1 ], sizeof( H3M1 ) );
        MEM_Cpy( ( U8* )&faultTimeStamp[ 1 ], ( U8* )&faultTimeStamp[ 0 ], sizeof( H3M1 ) );
        MEM_Cpy( ( U8* )&faultTimeStamp[ 0 ], ( U8* )&powerOnTime.h3m1,    sizeof( H3M1 ) );
        e2pWrFlags |= REC_FAULT_INFO;
      }
  }
  
void SVC_Ctrl( void )
  {
    /////////////////////////////////////////////////////////////////////////////////
    if ( SVC_Timer( ) )
      {
        REC_SpeedWorking( );
        REC_WorkingPower( );
        REC_WorkingTemp( );
      }
    REC_Faults( );
    /////////////////////////////////////////////////////////////////////////////////
    if ( e2pWrFlags & REC_SPEED_WORKING_INFO )
      {
        if ( E2P_WR_EX( LAST_SPEED_SETTING_ADDR,         lastSpeedSetting         ) )
        if ( E2P_WR_EX( LAST_SPEED_WORKING_TIME_ADDR,    lastSpeedWorkingTime     ) )
        if ( E2P_WR_EX( TOTAL_SPEED_WORKING_TIME_ADDR,   totalSpeedWorkingTime    ) )
          {
            e2pWrFlags &= ~REC_SPEED_WORKING_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( e2pWrFlags & REC_LAST_USE_INFO )
      {
        if ( E2P_WR_EX( LAST_USE_MAX_POWER_ADDR,         lastUseMaxPower          ) )
        if ( E2P_WR_EX( LAST_USE_AVE_POWER_ADDR,         lastUseAvePower          ) )
        if ( E2P_WR_EX( LAST_USE_READY_TIME_ADDR,        lastUseReadyTime         ) )
          {
            e2pWrFlags &= ~REC_LAST_USE_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( e2pWrFlags & REC_FAULT_INFO )
      {
        if ( E2P_WR_EX( FAULT_CODE_ADDR,                 faultCode                ) )
        if ( E2P_WR_EX( FAULT_TOTAL_COUNT_ADDR,          faultTotalCount          ) )
        if ( E2P_WR_EX( FAULT_TIME_STAMP_ADDR,           faultTimeStamp           ) )
          {
            e2pWrFlags &= ~REC_FAULT_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( e2pWrFlags & REC_TEMP_INFO )
      {
        if ( E2P_WR_EX( MAX_MCU_TEMP_ADDR,               maxMcuTemp               ) )
        if ( E2P_WR_EX( MAX_MOTOR_HISTORY_TEMP_ADDR,     maxMotorHistoryTemp      ) )
        if ( E2P_WR_EX( LAST_USE_MAX_MOTOR_TEMP_ADDR,    lastUseMaxMotorTemp      ) )
          {
            e2pWrFlags &= ~REC_TEMP_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( e2pWrFlags & REC_WORKING_INFO )
      {
        if ( E2P_WR_EX( TOTAL_POWER_ON_TIME_ADDR,        totalPowerOnTime         ) )
        if ( E2P_WR_EX( TOTAL_STANDBY_TIME_ADDR,         totalStandbyTime         ) )
        if ( E2P_WR_EX( TOTAL_WORKING_TIME_ADDR,         totalWorkingTime         ) )
          {
            e2pWrFlags &= ~REC_WORKING_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( e2pWrFlags & REC_KEY_WORKING_INFO )
      {
        if ( E2P_WR_EX( AUTO_WORKING_TIME_ADDR,          autoWorkingTime          ) )
        if ( E2P_WR_EX( KEY_WORKING_TIME_ADDR,           keyWorkingTime           ) )
        if ( E2P_WR_EX( TOTAL_AUTO_PRESSED_COUNT_ADDR,   totalAutoPressedCount    ) )
        if ( E2P_WR_EX( TOTAL_KEY_PRESSED_COUNT_ADDR,    totalKeyPressedCount     ) )
          {
            e2pWrFlags &= ~REC_KEY_WORKING_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( e2pWrFlags & REC_INTERLOCK_INFO )
      {
        if ( E2P_WR_EX( TOTAL_NO_INTERLOCK_TIME_ADDR,    totalNoInterlockTime     ) )
        if ( E2P_WR_EX( TOTAL_INTERLOCK_OPEN_COUNT_ADDR, totalInterlockOpenCount  ) )
          {
            e2pWrFlags &= ~REC_INTERLOCK_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( e2pWrFlags & REC_TUNE_INFO )
      {
        if ( E2P_WR_EX( TUNE_INFO_ADDR,                  tuneValue                ) )
          {
            e2pWrFlags &= ~REC_TUNE_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    else
      {
        e2pWrFlags = 0x00;
      }
  }
  
U8   SVC_GetTuneValue( void )
  {
    return tuneValue;
  }
  
void SVC_SetTuneValue( U8 value )
  {
    if ( tuneValue != value )
      {
        tuneValue = value;
        e2pWrFlags |= REC_TUNE_INFO;
      }
  }

