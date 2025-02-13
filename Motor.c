
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Motor.c                          //
//                                                       //
///////////////////////////////////////////////////////////

#define _MOTOR_SRC_
#include "Globals.h"
#include "Motor.h"
#include "MyMath.h"

#define TRIAC_TIME_SET( t )   do { __DI( ); tmTriac  = t;   __EI( ); } while(0)
#define MOTOR_ERROR_SET( e )  do { if ( !mtrError ) mtrError = e; } while(0)

CROM U8 ad2tpr[ 256 ] =
  {
    127, 127, 127, 127, 127, 127, 127, 127, 
    127, 127, 127, 127, 127, 127, 127, 127, 
    127, 127, 127, 127, 127, 127, 127, 127, 
    127, 127, 127, 127, 127, 127, 127, 127, 
    127, 127, 127, 127, 127, 127, 127, 127, 
    127, 127, 127, 127, 127, 127, 127, 127, 
    127, 127, 127, 127, 127, 127, 127, 127, 
    127, 127, 127, 127, 127, 127, 127, 127, 
    126, 125, 124, 123, 122, 122, 121, 120, 
    119, 119, 118, 117, 117, 116, 115, 115, 
    114, 113, 113, 112, 111, 111, 110, 109, 
    109, 108, 107, 107, 106, 106, 105, 104, 
    104, 103, 103, 102, 102, 101, 100, 100, 
    99,  99,  98,  98,  97,  97,  96,  96,  
    95,  94,  94,  93,  93,  92,  92,  91,  
    91,  90,  90,  89,  89,  88,  88,  87,  
    87,  86,  86,  85,  85,  84,  84,  83,  
    83,  82,  82,  81,  81,  80,  80,  79,  
    79,  78,  78,  77,  77,  76,  76,  75,  
    75,  74,  74,  73,  73,  72,  72,  71,  
    71,  70,  70,  69,  69,  68,  68,  67,  
    67,  66,  66,  65,  65,  64,  64,  63,  
    63,  62,  62,  61,  61,  60,  60,  59,  
    59,  58,  58,  57,  56,  56,  55,  55,  
    54,  54,  53,  53,  52,  51,  51,  50,  
    50,  49,  48,  48,  47,  46,  46,  45,  
    45,  44,  43,  42,  42,  41,  40,  40,  
    39,  38,  37,  37,  36,  35,  34,  33,  
    33,  32,  31,  30,  29,  28,  27,  26,  
    25,  24,  23,  21,  20,  19,  18,  16,  
    15,  13,  12,  10,  8,   6,   4,   2,   
    0,   0,   0,   0,   0,   0,   0,   0    
  };

void CAL_MtrTemp( void )
  {
    static XRAM U16 sum = 0;
    static XRAM U8  err = 0;
    static XRAM U8  er1 = 0;
    static XRAM U8  cnt = 0;
    if ( !acCycleHalf )
      {
        sum = 0;
        err = 0;
        er1 = 0;
        cnt = 0;
      }
    else if ( acCycleFlag )
      {
        sum += ( U16 )adNtc;
        if ( adNtc < 2 || adNtc >= 254 ) err++;
        if ( ++cnt >= 4 )
          {
            if ( !err )
              {
                er1 = 0;
                mtrTemp = ad2tpr[ sum >> 2 ];
                if ( mtrTemp >= 104 )
                  {
                    MOTOR_ERROR_SET( E_ERR_OVER_HEAT );
                  }
              }
            else if ( ++er1 >= 10 )
              {
                MOTOR_ERROR_SET( E_ERR_NTC_FAULT );
              }
            sum = 0;
            err = 0;
            cnt = 0;
          }
      }
  }

void CAL_MtrCurrent( void )
  {
    static XRAM U16 tmOVC = 0;
    U32 s, n;
    if ( !acCycleHalf )
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
    if ( mtrCurrent < CUR_THR /*|| ( mtrSpeedRef & 1 )*/ )
      {
        tmOVC = 0;
      }
    else if ( ++tmOVC >= 600 )
      {
        MOTOR_ERROR_SET( E_ERR_OVER_CURRENT );
      }
  }

void CAL_MtrSpeed( void )
  {
    static XRAM U16 tmZSP = 0;
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
    if ( tmTriac <= ( U16 )( acCycleHalf ) || mtrSpeed >= 300 )
      {
        tmZSP = 0;
      }
    else if ( ++tmZSP >= 300 )
      {
        MOTOR_ERROR_SET( mtrError = E_ERR_ZERO_SPEED );
      }
  }

void CAL_AcPeriod( void )
  {
    static U16 tmCRS = 0;
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
    if ( acCycleHalf != 0 )
      {
        tmCRS = 0;
      }
    else if ( ++tmCRS >= 300 )
      {
        MOTOR_ERROR_SET( mtrError = E_ERR_NO_CROSS );
      }
  }

void CAL_MtrError( void )
  {
  }

void MTR_Driver( void )
  {
    static XRAM U8    state = 0;
    static XRAM U16   sref = 0;
    static XRAM U8    time = 0;
    static XRAM SDW   OLIM;
    static XRAM PID_t SREG;
    S16 max;
    S16 err;
    if ( time ) time--;
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
                sref = mtrSpeedRef;
                OLIM.W.H = 80;
                OLIM.W.L = 0;
                SREG.kp = SREG_KP;
                SREG.ki = SREG_KI;
                SREG.kd = SREG_KD;
                SREG.e1 = 0;
                SREG.ri.W.H = 60;
                SREG.ri.W.L = 0;
                SREG.ro.W.H = SREG.ri.W.H;
                SREG.ro.W.L = SREG.ri.W.H;
                state++;
                time = 200;
              }
            break;
        case 1:
            if ( acCycleFlag )
              {
                if ( sref != mtrSpeedRef )
                  {
                    if ( sref < mtrSpeedRef ) time = 200;
                    sref = mtrSpeedRef;
                    OLIM.W.H = SREG.ro.W.H;
                  }
                if ( mtrSpeedRef >= 13000 )
                  {
                    if ( time >= 25 ) time = 25;
                    if ( mtrSpeedRef & 1 ) time = 0;
                  }
                max = ( S16 )( acCycle*54>>6 );
                OLIM.D += (400UL<<16)/25;
                if ( OLIM.W.H >= max ) OLIM.W.H = max; 
                err = ( S16 )( mtrSpeedRef - mtrSpeed );
                if ( time || err < 0 )
                  {
                    SREG.kp = SREG_KP_M;
                  }
                else
                  {
                    SREG.kp = SREG_KP;
                  }
                PID_CAL( &SREG, err );
                if ( mtrSpeedRef & 1 )
                  {
                    PID_LIM( &SREG, 40, max );
                  }
                else
                  {
                    PID_LIM( &SREG, 40, OLIM.W.H );
                  }
                TRIAC_TIME_SET( SREG.ro.W.H );
              }
            break;
      }
  }

void MTR_Init( void )
  {
    acCrossIndex = 0;
    acCrossEvent = 0;
    acCrossWDT = 0;
    acCycle = 0;
    acCycleHalf = 0;
    acCycleFlag = 0;
    mtrHallIndex = 0;
    mtrHallEvent = 0;
    mtrHallWDT = 0;
    tmTriac = 0;
    mtrSpeedRef = 0;
    mtrSpeed = 0;
    mtrCurrent = 0;
    mtrError = 0;
    mtrTemp = -1;
  }

void MTR_Ctrl( void )
  {
    CAL_MtrTemp( );
    CAL_MtrCurrent( );
    CAL_MtrSpeed( );
    CAL_AcPeriod( );
    CAL_MtrError( );
    MTR_Driver( );
    acCycleFlag = 0;
  }

