
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Motor.c                          //
//                                                       //
///////////////////////////////////////////////////////////

#define _MOTOR_SRC_
#include "Globals.h"
#include "Motor.h"
#include "MyMath.h"

#define TRIAC_TIME_SET( t ) do { __DI( ); tmTriac  = t;   __EI( ); } while(0)

void CAL_AcPeriod( void )
  {
    U8  i;
    U16 t;
    if ( ++acCrossWDT >= 5 )
      {
        acCrossWDT = 5;
        acCrossEvent = 0;
        acCycleHalf = 0;
        acCycle = 0;
      }
    else if ( acCrossEvent >= 5 )
      {
        acCrossEvent = 5;
        i = acCrossIndex - 1;
        t = ( acCrossTime[ i & 7 ] - acCrossTime[ i - 4 & 7 ] ) >> 3;
        if ( 300 < t && t < 500 )
          {
            acCycle = t;
            acCycleHalf = t >> 3;
          }
      }
  }

void CAL_MtrSpeed( void )
  {
    U8  n, i, j;
    U16 t;
    if ( ++mtrHallWDT >= 50 )
      {
        mtrHallWDT = 50;
        mtrHallEvent = 0;
        mtrSpeed = 0;
      }
    else if ( mtrHallEvent >= 3 )
      {
        if ( mtrHallEvent >= 30 ) mtrHallEvent = 30;
        n = mtrHallEvent - 2;
        i = mtrHallIndex - 1;
        j = 0;
        if ( n < 4 )
          {
            j = n;
            t = mtrHallTime[ i & 31 ] - mtrHallTime[ i - j & 31 ];
          }
        else while ( 1 )
          {
            n -= 4;
            j += 4;
            t = mtrHallTime[ i & 31 ] - mtrHallTime[ i - j & 31 ];
            if ( n < 4 ) break;
            if ( t >= 1600 ) break;
          }
        mtrSpeed = ( U16 )( 600000UL * ( U32 )j / ( U32 )t );
      }
  }

void CAL_MtrCurrent( void )
  {
    U32 s, n;
    if ( acCrossEvent < 5 )
      {
        mtrCurrent = 0;
      }
    else if ( acCycleFlag && adCurN != 0 )
      {
        s = adCur2 << 1;
        n = ( U32 )adCurN;
        adCurN = 0;
        s = ( U32 )SQRT32( s );
        n = ( U32 )SQRT32( n << 16 );
        mtrCurrent = ( U16 )( ( 25600UL*4800UL/50UL/1023UL ) * s / n );
      }
  }

void CAL_MtrError( void )
  {
    static U8  tmErr1 = 0;
    static U16 tmErr2 = 0;
    static U16 tmErr3 = 0;
    static U8  tmErr4 = 0;
    if ( acCycleHalf != 0 )
      {
        tmErr1 = 0;
      }
    else if ( ++tmErr1 >= 100 )
      {
        mtrError = E_ERR_NO_CROSS;
      }
    if ( mtrSpeedRef == 0 || mtrSpeed >= 500 )
      {
        tmErr2 = 0;
      }
    else if ( ++tmErr2 >= 500 )
      {
        mtrError = E_ERR_ZERO_SPEED;
      }
    if ( mtrCurrent < 760 )
      {
        tmErr3 = 0;
      }
    else if ( ++tmErr3 >= 1500 )
      {
        mtrError = E_ERR_OVER_CURRENT;
      }
    if ( adNtc >= 95 && adNtc < 254 )
      {
        tmErr4 = 0;
      }
    else if ( ++tmErr4 >= 200 )
      {
        mtrError = E_ERR_TPMR_FAULT;
      }
  }

void MTR_Driver( void )
  {
    static U8   state = 0;
    static PI_t IREG;
    static PI_t SREG;
    S16 max;
    if ( mtrSpeedRef == 0 || !acCycleHalf )
      {
        state = 0;
        TRIAC_TIME_SET( 0 );
      }
    else switch ( state )
      {
        case 0:
            TRIAC_TIME_SET( 0 );
            if ( acCycle )
              {
                SREG.kp = SREG_KP;
                SREG.ki = SREG_KI;
                SREG.ri.W.H = 40;
                SREG.ri.W.L = 0;
                SREG.ro.W.H = 40;
                SREG.ro.W.L = 40;
                IREG.kp = IREG_KP;
                IREG.ki = IREG_KI;
                IREG.ri.W.H = acCycle >> 1;
                IREG.ri.W.L = 0;
                IREG.ro.W.H = SREG.ri.W.H;
                IREG.ro.W.L = SREG.ri.W.H;
                state++;
              }
            break;
        case 1:
            if ( acCycleFlag )
              {
                max = ( S16 )( acCycle*56>>6 );
                PI_CAL( &SREG, mtrSpeedRef - mtrSpeed );
                PI_LIM( &SREG, 40, max );
                PI_CAL( &IREG, 800 - mtrCurrent );
                PI_LIM( &IREG, 40, max );
                if ( SREG.ro.W.H < IREG.ro.W.H )
                  {
                    IREG.ro.W.H = SREG.ro.W.H;
                  }
                else
                  {
                    SREG.ro.W.H = IREG.ro.W.H;
                  }
                TRIAC_TIME_SET( SREG.ro.W.H );
              }
            break;
      }
  }

void MTR_Init( void )
  {
    mtrSpeedRef = 0;
    mtrSpeed = 0;
    mtrCurrent = 0;
    mtrError = 0;
  }

void MTR_Ctrl( void )
  {
    CAL_AcPeriod( );
    CAL_MtrSpeed( );
    CAL_MtrCurrent( );
    CAL_MtrError( );
    MTR_Driver( );
    acCycleFlag = 0;
  }

