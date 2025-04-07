
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   MyMath.c                         //
//                                                       //
///////////////////////////////////////////////////////////

#define _MY_MATH_SRC_
#include "Globals.h"
#include "MyMath.h"

U16  SQRT32( U32 a )
  {
    U32 root = 0;
    U32 divisor = 0;
    U32 remainder = 0;
    U8  i = 16;
    do{
        root <<= 1;
        remainder <<= 1;
        if ( a & 0x80000000 ) remainder |= 1;
        a <<= 1;
        remainder <<= 1;
        if ( a & 0x80000000 ) remainder |= 1;
        a <<= 1;
        divisor = ( root << 1 ) | 1;
        if ( remainder >= divisor )
          {
            remainder -= divisor;
            root |= 1;
          }
      } while( --i );
    return ( ( U16 )root );
  }

S16  PID_CAL( PID_t * p, S16 err )
  {
    S32 tempp,tempi,tempd;
    if ( !( p->ro.W.H < p->ro.W.L && err >= 0 )
      && !( p->ro.W.L < p->ro.W.H && err < 0 ) )
      {
        p->ri.D += MULS32( p->ki, err );
      }

    tempp=MULS32( p->kp, err );
    tempi=p->ri.D;
    tempd=MULS32( p->kd, err - p->e1 );
    //p->ro.D=tempp+tempi+MULS32( p->kd, err - p->e1 ); //tempd;
    p->ro.D = tempp + tempi + tempd;    
    if(tempp<0)
    {    pidKp=-tempp;    }
    else
    {    pidKp=tempp;     }
    if(tempi<0)
    {    pidKi=-tempi;    }
    else
    {    pidKi=tempi;     }
    if(tempd<0)
    {    pidKd=-tempd;    }
    else
    {    pidKd=tempd;     }
    
    //p->ro.D = MULS32( p->kp, err ) + MULS32( p->kd, err - p->e1 ) + p->ri.D;
    p->ro.W.L = p->ro.W.H;
    p->e1 = err;
    return p->ro.W.H;
  }

S16  PID_LIM( PID_t * p, S16 min, S16 max )
  {
    if ( min < max )
      {
        if ( p->ro.W.H > max )
          {
            p->ro.W.H = max;
          }
        else if ( p->ro.W.H < min )
          {
            p->ro.W.H = min;
          }
      }
    return p->ro.W.H;
  }

