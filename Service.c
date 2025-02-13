
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
    PRODUCT_NUM_0,
    PRODUCT_NUM_1,
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
__EEPROM_DATA( 0,0,0,0,0,0,0,0 );

#define REC_TUNE_INFO         0X01
#define REC_USE_INFO          0X02
#define REC_KEY_INFO          0X04
#define REC_INTERLOCK_INFO    0X08

typedef struct { U8 s; U8 m; }                M1S1;
typedef struct { U8 s; U8 m; U8 h; }          H1M1S1;
typedef struct { U8 s; U8 m; U8 h; U8 h1; }   H2M1S1;
typedef struct { U8 m; U8 h; U8 h1; U8 h2; }  H3M1;
typedef struct { U8 s; H3M1 h3m1; }           H3M1S1;
typedef struct { U8 l; U8 h; }                COUNT;

#define LAST_USE_FAULT_CODE_ADDR        0x1C
static XRAM  U8     lastUseFaultCode[ 7 ]       @ 0x420;
#define LAST_USE_SET_SPEED_ADDR         0x23
static XRAM  U8     lastUseSetSpeed[ 7 ]        @ 0x428;
#define LAST_USE_MAX_MTR_POWER_ADDR     0x2A
static XRAM  U8     lastUseMaxMtrPower[ 7 ]     @ 0x430;
#define LAST_USE_AVE_MTR_POWER_ADDR     0x31
static XRAM  U8     lastUseAveMtrPower[ 7 ]     @ 0x438;
#define LAST_USE_WORKING_TIME_ADDR      0x38
static XRAM  H1M1S1 lastUseWorkingTime[ 7 ]     @ 0x440;
#define LAST_USE_MAX_MTR_TEMP_ADDR      0x4D
static XRAM  U8     lastUseMaxMtrTemp[ 7 ]      @ 0x458;
#define LAST_USE_WAITING_TIME_ADDR      0x54
static XRAM  M1S1   lastUseWaitingTime[ 7 ]     @ 0x4A0;
#define LAST_USE_TIME_STAMP_ADDR        0x62
static XRAM  H3M1   lastUseTimeStamp[ 7 ]       @ 0x4B0;
#define LAST_USE_MAX_MCU_TEMP_ADDR      0x7E
static XRAM  U8     lastUseMaxMcuTemp[ 7 ]      @ 0x4D0;
#define MAX_MOTOR_HISTORY_TEMP_ADDR     0x85
static XRAM  U8     maxMotorHistoryTemp         @ 0x520;
#define TOTAL_FAULT_COUNT_ADDR          0x86
static XRAM  U8     totalFaultCount[ 9 ]        @ 0x522;
#define TOTAL_SPEED_WORKING_TIME_ADDR   0x8F
static XRAM  H2M1S1 totalSpeedWorkingTime[ 7 ]  @ 0x530;
#define TOTAL_WORKING_TIME_ADDR         0xD3
static XRAM  H3M1   totalWorkingTime            @ 0x550;
#define TOTAL_POWER_ON_TIME_ADDR        0xD7
static XRAM  H3M1   totalPowerOnTime            @ 0x554;
#define TOTAL_STANDBY_TIME_ADDR         0xDF
static XRAM  H3M1   totalStandbyTime            @ 0x558;

static bank2 U8     thisUseFaultCode;
static bank2 U8     thisUseSetSpeed;
static bank2 U8     thisUseMaxMtrPower;
static bank2 U8     thisUseAveMtrPower;
static bank2 S8     thisUseMaxMtrTemp;
static bank2 H1M1S1 thisUseWorkingTime;
static bank2 M1S1   thisUseWaitingTime;
static bank2 H3M1   thisUseTimeStamp;
static bank2 S8     thisUseMaxMcuTemp;

#define AUTO_WORKING_TIME_ADDR          0xAB
static XRAM  H2M1S1 autoWorkingTime             @ 0x5A0;
#define KEY_WORKING_TIME_ADDR           0xAF
static XRAM  H2M1S1 keyWorkingTime[ 9 ]         @ 0x5A6;
#define TOTAL_AUTO_PRESSED_COUNT_ADDR   0xE5
static XRAM  COUNT  totalAutoPressedCount       @ 0x5D0;
#define TOTAL_KEY_PRESSED_COUNT_ADDR    0xE7
static XRAM  COUNT  totalKeyPressedCount[ 9 ]   @ 0x5D4;

#define TOTAL_NO_INTERLOCK_TIME_ADDR    0xDB
static XRAM  H3M1   totalNoInterlockTime        @ 0x620;
#define TOTAL_INTERLOCK_OPEN_COUNT_ADDR 0xE3
static XRAM  COUNT  totalInterlockOpenCount     @ 0x626;

#define TUNE_INFO_ADDR                  0xFE
static bank2 U8     tuneValue;
#define TUNE_COUNT_ADDR                 0x1B
static bank2 U8     tuneCount;

static bank2 H3M1S1 powerOnTime;

void E2P_RD( U8 addr, U8 * p, U8 n )
  {
    while ( n-- )
      {
        *p = E2P_ReadByte( addr );
        addr++;
        p++;
      }
  }

U8   E2P_WR( U8 addr, U8 * p, U8 n )
  {
    while ( n )
      {
        if ( *p != E2P_ReadByte( addr ) )
          {
            E2P_WriteByte( addr, *p );
            return 0;
          }
        addr++;
        p++;
        n--;
      }
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
    
    E2P_RD_EX( TUNE_INFO_ADDR,                  tuneValue                );
    E2P_RD_EX( TUNE_COUNT_ADDR,                 tuneCount                );
    
    E2P_RD_EX( LAST_USE_FAULT_CODE_ADDR,        lastUseFaultCode         );
    E2P_RD_EX( LAST_USE_SET_SPEED_ADDR,         lastUseSetSpeed          );
    E2P_RD_EX( LAST_USE_MAX_MTR_POWER_ADDR,     lastUseMaxMtrPower       );
    E2P_RD_EX( LAST_USE_AVE_MTR_POWER_ADDR,     lastUseAveMtrPower       );
    E2P_RD_EX( LAST_USE_WORKING_TIME_ADDR,      lastUseWorkingTime       );
    E2P_RD_EX( LAST_USE_MAX_MTR_TEMP_ADDR,      lastUseMaxMtrTemp        );
    E2P_RD_EX( LAST_USE_WAITING_TIME_ADDR,      lastUseWaitingTime       );
    E2P_RD_EX( LAST_USE_TIME_STAMP_ADDR,        lastUseTimeStamp         );
    E2P_RD_EX( LAST_USE_MAX_MCU_TEMP_ADDR,      lastUseMaxMcuTemp        );
    E2P_RD_EX( MAX_MOTOR_HISTORY_TEMP_ADDR,     maxMotorHistoryTemp      );
    E2P_RD_EX( TOTAL_FAULT_COUNT_ADDR,          totalFaultCount          );
    E2P_RD_EX( TOTAL_SPEED_WORKING_TIME_ADDR,   totalSpeedWorkingTime    );
    E2P_RD_EX( TOTAL_WORKING_TIME_ADDR,         totalWorkingTime         );
    E2P_RD_EX( TOTAL_POWER_ON_TIME_ADDR,        totalPowerOnTime         );
    E2P_RD_EX( TOTAL_STANDBY_TIME_ADDR,         totalStandbyTime         );
    thisUseFaultCode   = 0;
    thisUseSetSpeed    = 0;
    thisUseMaxMtrPower = 0;
    thisUseAveMtrPower = 0;
    thisUseMaxMtrTemp  = 0;
    thisUseMaxMcuTemp  = 0;
    MEM_Clr( ( U8* )&thisUseWorkingTime,
             sizeof( thisUseWorkingTime ) );
    MEM_Clr( ( U8* )&thisUseWaitingTime,
             sizeof( thisUseWaitingTime ) );
    MEM_Clr( ( U8* )&thisUseTimeStamp,
             sizeof( thisUseTimeStamp ) );
    MEM_Clr( ( U8* )&powerOnTime,
             sizeof( powerOnTime ) );
    
    E2P_RD_EX( AUTO_WORKING_TIME_ADDR,          autoWorkingTime          );
    E2P_RD_EX( KEY_WORKING_TIME_ADDR,           keyWorkingTime           );
    E2P_RD_EX( TOTAL_AUTO_PRESSED_COUNT_ADDR,   totalAutoPressedCount    );
    E2P_RD_EX( TOTAL_KEY_PRESSED_COUNT_ADDR,    totalKeyPressedCount     );
    
    E2P_RD_EX( TOTAL_NO_INTERLOCK_TIME_ADDR,    totalNoInterlockTime     );
    E2P_RD_EX( TOTAL_INTERLOCK_OPEN_COUNT_ADDR, totalInterlockOpenCount  );
    
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
            e2pWrFlags |= REC_USE_INFO;
          }
        return 1;
      }
    return 0;
  }

void REC_LastUseInfo( void )
  {
    static XRAM U8  lsLvl = 0;
    static XRAM U8  cnLvl = 0;
    static XRAM U8  fault = 0;
    U8  lvl = 0;
    U8  fau = 0;
    U8  rec = 0;
    U8  sec = 0;
    /////////////////////////////////////////////////////////////////////////////////
    if ( thisUseSetSpeed != 0 )
      {
        if ( thisUseMaxMtrPower < mtrPower )
          {
            thisUseMaxMtrPower = mtrPower;
          }
        if ( mtrPowerAve != 0 )
          {
            thisUseAveMtrPower = mtrPowerAve;
            if ( thisUseAveMtrPower > thisUseMaxMtrPower )
              {
                thisUseAveMtrPower = thisUseMaxMtrPower;
              }
          }
        if ( thisUseMaxMtrTemp < mtrTemp )
          {
            thisUseMaxMtrTemp = mtrTemp;
          }
        H1M1S1_ADD( &thisUseWorkingTime, 1 );
        thisUseMaxMcuTemp = 0;
      }
    else
      {
        M1S1_ADD( &thisUseWaitingTime, 1 );
        if ( thisUseWaitingTime.s == 0 )
          {
            H3M1_ADD( &totalStandbyTime, 1 );
            e2pWrFlags |= REC_USE_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    if ( sysFault != 0 && sysFault < 10 )
      {
        fau = sysFault;
        if ( fault != fau )
          {
            fault = fau;
            totalFaultCount[ fault - 1 ]++;
            e2pWrFlags |= REC_USE_INFO;
            lvl = 0;
            if ( thisUseSetSpeed == 0 )
              {
                rec = 1;
              }
            else
              {
                rec = 4;
              }
          }
      }
    else
      {
        fau = 0;
        fault = 0;
      }
    /////////////////////////////////////////////////////////////////////////////////
    if ( fault == 0 )
      {
        if ( sysLevel < 8 )
          {
            lvl = sysLevel;
          }
        else
          {
            lvl = 0;
          }
        if ( lsLvl != lvl )
          {
            lsLvl = lvl;
            cnLvl = 0;
          }
        else if ( cnLvl < 1 )
          {
            cnLvl++;
          }
        else if ( thisUseSetSpeed != lvl )
          {
            if ( lvl != 0 )
              {
                if ( thisUseSetSpeed == 0 )
                  {
                    rec = 2;
                  }
                else
                  {
                    rec = 3;
                  }
                if ( thisUseWaitingTime.m == 0
                  && thisUseWaitingTime.s <= 2 )
                  {
                    thisUseWaitingTime.s = 0;
                  }
                else
                  {
                    M1S1_SUB( &thisUseWaitingTime, 2 );
                  }
              }
            else
              {
                rec = 4;
              }
          }
        if ( rec == 0
          && thisUseSetSpeed != 0
          && thisUseWorkingTime.s % 15 == 0 )
          {
            rec = 5;
          }
      }
    else
      {
        lsLvl = 0;
        cnLvl = 0;
      }
    /////////////////////////////////////////////////////////////////////////////////
    if ( rec >= 1 && rec <= 3 )
      {
        lastUseFaultCode  [ 6 ] = lastUseFaultCode  [ 5 ];
        lastUseSetSpeed   [ 6 ] = lastUseSetSpeed   [ 5 ];
        lastUseMaxMtrPower[ 6 ] = lastUseMaxMtrPower[ 5 ];
        lastUseAveMtrPower[ 6 ] = lastUseAveMtrPower[ 5 ];
        lastUseMaxMtrTemp [ 6 ] = lastUseMaxMtrTemp [ 5 ];
        lastUseMaxMcuTemp [ 6 ] = lastUseMaxMcuTemp [ 5 ];
        MEM_Cpy( ( U8* )&lastUseWorkingTime[ 6 ],
                 ( U8* )&lastUseWorkingTime[ 5 ],
                 sizeof( H1M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseWaitingTime[ 6 ],
                 ( U8* )&lastUseWaitingTime[ 5 ],
                 sizeof( M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseTimeStamp  [ 6 ],
                 ( U8* )&lastUseTimeStamp  [ 5 ],
                 sizeof( H3M1 ) );
        
        lastUseFaultCode  [ 5 ] = lastUseFaultCode  [ 4 ];
        lastUseSetSpeed   [ 5 ] = lastUseSetSpeed   [ 4 ];
        lastUseMaxMtrPower[ 5 ] = lastUseMaxMtrPower[ 4 ];
        lastUseAveMtrPower[ 5 ] = lastUseAveMtrPower[ 4 ];
        lastUseMaxMtrTemp [ 5 ] = lastUseMaxMtrTemp [ 4 ];
        lastUseMaxMcuTemp [ 5 ] = lastUseMaxMcuTemp [ 4 ];
        MEM_Cpy( ( U8* )&lastUseWorkingTime[ 5 ],
                 ( U8* )&lastUseWorkingTime[ 4 ],
                 sizeof( H1M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseWaitingTime[ 5 ],
                 ( U8* )&lastUseWaitingTime[ 4 ],
                 sizeof( M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseTimeStamp  [ 5 ],
                 ( U8* )&lastUseTimeStamp  [ 4 ],
                 sizeof( H3M1 ) );
        
        lastUseFaultCode  [ 4 ] = lastUseFaultCode  [ 3 ];
        lastUseSetSpeed   [ 4 ] = lastUseSetSpeed   [ 3 ];
        lastUseMaxMtrPower[ 4 ] = lastUseMaxMtrPower[ 3 ];
        lastUseAveMtrPower[ 4 ] = lastUseAveMtrPower[ 3 ];
        lastUseMaxMtrTemp [ 4 ] = lastUseMaxMtrTemp [ 3 ];
        lastUseMaxMcuTemp [ 4 ] = lastUseMaxMcuTemp [ 3 ];
        MEM_Cpy( ( U8* )&lastUseWorkingTime[ 4 ],
                 ( U8* )&lastUseWorkingTime[ 3 ],
                 sizeof( H1M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseWaitingTime[ 4 ],
                 ( U8* )&lastUseWaitingTime[ 3 ],
                 sizeof( M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseTimeStamp  [ 4 ],
                 ( U8* )&lastUseTimeStamp  [ 3 ],
                 sizeof( H3M1 ) );
        
        lastUseFaultCode  [ 3 ] = lastUseFaultCode  [ 2 ];
        lastUseSetSpeed   [ 3 ] = lastUseSetSpeed   [ 2 ];
        lastUseMaxMtrPower[ 3 ] = lastUseMaxMtrPower[ 2 ];
        lastUseAveMtrPower[ 3 ] = lastUseAveMtrPower[ 2 ];
        lastUseMaxMtrTemp [ 3 ] = lastUseMaxMtrTemp [ 2 ];
        lastUseMaxMcuTemp [ 3 ] = lastUseMaxMcuTemp [ 2 ];
        MEM_Cpy( ( U8* )&lastUseWorkingTime[ 3 ],
                 ( U8* )&lastUseWorkingTime[ 2 ],
                 sizeof( H1M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseWaitingTime[ 3 ],
                 ( U8* )&lastUseWaitingTime[ 2 ],
                 sizeof( M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseTimeStamp  [ 3 ],
                 ( U8* )&lastUseTimeStamp  [ 2 ],
                 sizeof( H3M1 ) );
        
        lastUseFaultCode  [ 2 ] = lastUseFaultCode  [ 1 ];
        lastUseSetSpeed   [ 2 ] = lastUseSetSpeed   [ 1 ];
        lastUseMaxMtrPower[ 2 ] = lastUseMaxMtrPower[ 1 ];
        lastUseAveMtrPower[ 2 ] = lastUseAveMtrPower[ 1 ];
        lastUseMaxMtrTemp [ 2 ] = lastUseMaxMtrTemp [ 1 ];
        lastUseMaxMcuTemp [ 2 ] = lastUseMaxMcuTemp [ 1 ];
        MEM_Cpy( ( U8* )&lastUseWorkingTime[ 2 ],
                 ( U8* )&lastUseWorkingTime[ 1 ],
                 sizeof( H1M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseWaitingTime[ 2 ],
                 ( U8* )&lastUseWaitingTime[ 1 ],
                 sizeof( M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseTimeStamp  [ 2 ],
                 ( U8* )&lastUseTimeStamp  [ 1 ],
                 sizeof( H3M1 ) );

        if ( rec <= 2 )
          {
            lastUseFaultCode  [ 1 ] = lastUseFaultCode  [ 0 ];
            lastUseSetSpeed   [ 1 ] = lastUseSetSpeed   [ 0 ];
            lastUseMaxMtrPower[ 1 ] = lastUseMaxMtrPower[ 0 ];
            lastUseAveMtrPower[ 1 ] = lastUseAveMtrPower[ 0 ];
            lastUseMaxMtrTemp [ 1 ] = lastUseMaxMtrTemp [ 0 ];
            lastUseMaxMcuTemp [ 1 ] = lastUseMaxMcuTemp [ 0 ];
            lastUseMaxMcuTemp [ 1 ] = lastUseMaxMcuTemp [ 0 ];
            MEM_Cpy( ( U8* )&lastUseWorkingTime[ 1 ],
                     ( U8* )&lastUseWorkingTime[ 0 ],
                     sizeof( H1M1S1 ) );
            MEM_Cpy( ( U8* )&lastUseWaitingTime[ 1 ],
                     ( U8* )&lastUseWaitingTime[ 0 ],
                     sizeof( M1S1 ) );
            MEM_Cpy( ( U8* )&lastUseTimeStamp  [ 1 ],
                     ( U8* )&lastUseTimeStamp  [ 0 ],
                     sizeof( H3M1 ) );
          }
        else
          {
            lastUseFaultCode  [ 1 ] = 0;
            lastUseSetSpeed   [ 1 ] = thisUseSetSpeed;
            lastUseMaxMtrPower[ 1 ] = thisUseMaxMtrPower;
            lastUseAveMtrPower[ 1 ] = thisUseAveMtrPower;
            lastUseMaxMtrTemp [ 1 ] = thisUseMaxMtrTemp;
            lastUseMaxMcuTemp [ 1 ] = thisUseMaxMcuTemp;
            if ( lastUseFaultCode[ 1 ] ) lastUseFaultCode[ 1 ] += 0xE0;
            if ( lastUseSetSpeed [ 1 ] ) lastUseSetSpeed [ 1 ] += 0xF0;
            MEM_Cpy( ( U8* )&lastUseWorkingTime[ 1 ],
                     ( U8* )&thisUseWorkingTime,
                     sizeof( H1M1S1 ) );
            MEM_Cpy( ( U8* )&lastUseWaitingTime[ 1 ],
                     ( U8* )&thisUseWaitingTime,
                     sizeof( M1S1 ) );
            MEM_Cpy( ( U8* )&lastUseTimeStamp  [ 1 ],
                     ( U8* )&thisUseTimeStamp,
                     sizeof( H3M1 ) );
          }

        if ( rec == 1 )
          {
            thisUseFaultCode   = fault;
            thisUseSetSpeed    = 0;
            thisUseMaxMtrPower = 0;
            thisUseAveMtrPower = 0;
            thisUseMaxMtrTemp  = mtrTemp;
            thisUseMaxMcuTemp  = 0;
            MEM_Clr( ( U8* )&thisUseWorkingTime,
                     sizeof( thisUseWorkingTime ) );
            MEM_Cpy( ( U8* )&thisUseTimeStamp,
                     ( U8* )&totalWorkingTime,
                     sizeof( H3M1 ) );
          }
        else
          {
            thisUseFaultCode   = 0;
            thisUseSetSpeed    = lvl;
            thisUseMaxMtrPower = mtrPower;
            thisUseAveMtrPower = mtrPower;
            thisUseMaxMtrTemp  = mtrTemp;
            thisUseMaxMcuTemp  = 0;
            MEM_Clr( ( U8* )&thisUseWorkingTime,
                     sizeof( thisUseWorkingTime ) );
            MEM_Cpy( ( U8* )&thisUseTimeStamp,
                     ( U8* )&totalWorkingTime,
                     sizeof( H3M1 ) );
          }

        lastUseFaultCode  [ 0 ] = thisUseFaultCode;
        lastUseSetSpeed   [ 0 ] = thisUseSetSpeed;
        lastUseMaxMtrPower[ 0 ] = thisUseMaxMtrPower;
        lastUseAveMtrPower[ 0 ] = thisUseAveMtrPower;
        lastUseMaxMtrTemp [ 0 ] = thisUseMaxMtrTemp;
        lastUseMaxMcuTemp [ 0 ] = thisUseMaxMcuTemp;
        if ( lastUseFaultCode[ 0 ] ) lastUseFaultCode[ 0 ] += 0xE0;
        if ( lastUseSetSpeed [ 0 ] ) lastUseSetSpeed [ 0 ] += 0xF0;
        MEM_Cpy( ( U8* )&lastUseWorkingTime[ 0 ],
                 ( U8* )&thisUseWorkingTime,
                 sizeof( H1M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseWaitingTime[ 0 ],
                 ( U8* )&thisUseWaitingTime,
                 sizeof( M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseTimeStamp[ 0 ],
                 ( U8* )&thisUseTimeStamp,
                 sizeof( H3M1 ) );

        e2pWrFlags |= REC_USE_INFO;
          
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( rec == 4 )
      {
        sec = thisUseWorkingTime.s % 15;
        if ( sec == 0 ) sec = 15;
        H2M1S1_ADD( &totalSpeedWorkingTime[ thisUseSetSpeed - 1 ], sec );
        if ( thisUseWorkingTime.s == 0 ) H3M1_ADD( &totalWorkingTime, 1 );
        e2pWrFlags |= REC_USE_INFO;
        
        thisUseFaultCode = fault;
        lastUseFaultCode  [ 0 ] = thisUseFaultCode;
        lastUseSetSpeed   [ 0 ] = thisUseSetSpeed;
        lastUseMaxMtrPower[ 0 ] = thisUseMaxMtrPower;
        lastUseAveMtrPower[ 0 ] = thisUseAveMtrPower;
        lastUseMaxMtrTemp [ 0 ] = thisUseMaxMtrTemp;
        lastUseMaxMcuTemp [ 0 ] = thisUseMaxMcuTemp;
        if ( lastUseFaultCode[ 0 ] ) lastUseFaultCode[ 0 ] += 0xE0;
        if ( lastUseSetSpeed [ 0 ] ) lastUseSetSpeed [ 0 ] += 0xF0;
        MEM_Cpy( ( U8* )&lastUseWorkingTime[ 0 ],
                 ( U8* )&thisUseWorkingTime,
                 sizeof( H1M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseTimeStamp[ 0 ],
                 ( U8* )&totalWorkingTime,
                 sizeof( H3M1 ) );
        e2pWrFlags |= REC_USE_INFO;
        
        thisUseFaultCode   = 0;
        thisUseSetSpeed    = 0;
        thisUseMaxMtrPower = 0;
        thisUseAveMtrPower = 0;
        thisUseMaxMtrTemp  = 0;
        thisUseMaxMcuTemp  = 0;
        MEM_Clr( ( U8* )&thisUseWorkingTime,
                 sizeof( thisUseWorkingTime ) );
        MEM_Clr( ( U8* )&thisUseWaitingTime,
                 sizeof( thisUseWaitingTime ) );
        MEM_Clr( ( U8* )&thisUseTimeStamp,
                 sizeof( thisUseTimeStamp ) );
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( rec == 5 )
      {
        H2M1S1_ADD( &totalSpeedWorkingTime[ thisUseSetSpeed - 1 ], 15 );
        if ( thisUseWorkingTime.s == 0 ) H3M1_ADD( &totalWorkingTime, 1 );
        e2pWrFlags |= REC_USE_INFO;
        
        thisUseFaultCode = fault;
        lastUseFaultCode  [ 0 ] = thisUseFaultCode;
        lastUseSetSpeed   [ 0 ] = thisUseSetSpeed;
        lastUseMaxMtrPower[ 0 ] = thisUseMaxMtrPower;
        lastUseAveMtrPower[ 0 ] = thisUseAveMtrPower;
        lastUseMaxMtrTemp [ 0 ] = thisUseMaxMtrTemp;
        lastUseMaxMcuTemp [ 0 ] = thisUseMaxMcuTemp;
        if ( lastUseFaultCode[ 0 ] ) lastUseFaultCode[ 0 ] += 0xE0;
        if ( lastUseSetSpeed [ 0 ] ) lastUseSetSpeed [ 0 ] += 0xF0;
        MEM_Cpy( ( U8* )&lastUseWorkingTime[ 0 ],
                 ( U8* )&thisUseWorkingTime,
                 sizeof( H1M1S1 ) );
        MEM_Cpy( ( U8* )&lastUseTimeStamp[ 0 ],
                 ( U8* )&thisUseTimeStamp,
                 sizeof( H3M1 ) );
        e2pWrFlags |= REC_USE_INFO;
      }
  }

void REC_WorkingTemp( void )
  {
    if ( maxMotorHistoryTemp < mtrTemp )
      {
        maxMotorHistoryTemp = mtrTemp;
        e2pWrFlags |= REC_USE_INFO;
      }
  }
  
void SVC_Ctrl( void )
  {
    /////////////////////////////////////////////////////////////////////////////////
    if ( SVC_Timer( ) )
      {
        REC_LastUseInfo( );
        REC_WorkingTemp( );
      }
    /////////////////////////////////////////////////////////////////////////////////
    if ( e2pWrFlags & REC_TUNE_INFO )
      {
        if ( E2P_WR_EX( TUNE_INFO_ADDR,                  tuneValue                ) )
        if ( E2P_WR_EX( TUNE_COUNT_ADDR,                 tuneCount                ) )
          {
            e2pWrFlags &= ~REC_TUNE_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( e2pWrFlags & REC_USE_INFO )
      {
        if ( E2P_WR_EX( LAST_USE_FAULT_CODE_ADDR,        lastUseFaultCode         ) )
        if ( E2P_WR_EX( LAST_USE_SET_SPEED_ADDR,         lastUseSetSpeed          ) )
        if ( E2P_WR_EX( LAST_USE_MAX_MTR_POWER_ADDR,     lastUseMaxMtrPower       ) )
        if ( E2P_WR_EX( LAST_USE_AVE_MTR_POWER_ADDR,     lastUseAveMtrPower       ) )
        if ( E2P_WR_EX( LAST_USE_WORKING_TIME_ADDR,      lastUseWorkingTime       ) )
        if ( E2P_WR_EX( LAST_USE_MAX_MTR_TEMP_ADDR,      lastUseMaxMtrTemp        ) )
        if ( E2P_WR_EX( LAST_USE_WAITING_TIME_ADDR,      lastUseWaitingTime       ) )
        if ( E2P_WR_EX( LAST_USE_TIME_STAMP_ADDR,        lastUseTimeStamp         ) )
        if ( E2P_WR_EX( LAST_USE_MAX_MCU_TEMP_ADDR,      lastUseMaxMcuTemp        ) )
        if ( E2P_WR_EX( MAX_MOTOR_HISTORY_TEMP_ADDR,     maxMotorHistoryTemp      ) )
        if ( E2P_WR_EX( TOTAL_FAULT_COUNT_ADDR,          totalFaultCount          ) )
        if ( E2P_WR_EX( TOTAL_SPEED_WORKING_TIME_ADDR,   totalSpeedWorkingTime    ) )
        if ( E2P_WR_EX( TOTAL_WORKING_TIME_ADDR,         totalWorkingTime         ) )
        if ( E2P_WR_EX( TOTAL_POWER_ON_TIME_ADDR,        totalPowerOnTime         ) )
        if ( E2P_WR_EX( TOTAL_STANDBY_TIME_ADDR,         totalStandbyTime         ) )
          {
            e2pWrFlags &= ~REC_USE_INFO;
          }
      }
    /////////////////////////////////////////////////////////////////////////////////
    else if ( e2pWrFlags & REC_KEY_INFO )
      {
        if ( E2P_WR_EX( AUTO_WORKING_TIME_ADDR,          autoWorkingTime          ) )
        if ( E2P_WR_EX( KEY_WORKING_TIME_ADDR,           keyWorkingTime           ) )
        if ( E2P_WR_EX( TOTAL_AUTO_PRESSED_COUNT_ADDR,   totalAutoPressedCount    ) )
        if ( E2P_WR_EX( TOTAL_KEY_PRESSED_COUNT_ADDR,    totalKeyPressedCount     ) )
          {
            e2pWrFlags &= ~REC_KEY_INFO;
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
    else
      {
        e2pWrFlags = 0x00;
      }
  }
  
U8   SVC_GetTuneValue( void )
  {
    if ( tuneValue == 0 )
      {
        return DEFAULT_TUNE_VALUE;
      }
    return tuneValue;
  }
  
void SVC_SetTuneValue( U8 value )
  {
    tuneCount++;
    tuneValue = value;
    e2pWrFlags |= REC_TUNE_INFO;
  }

