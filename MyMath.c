
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   MyMath.c                         //
//                                                       //
///////////////////////////////////////////////////////////

#define _MY_MATH_SRC_
#include "Globals.h"
#include "MyMath.h"

#define MULU( a, b )  ( ( U32 )a * ( U32 )b )
#define MULS( a, b )  ( ( S32 )a * ( S32 )b )

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

S16  PI_CAL( PI_t * p, S16 err )
  {
    if ( !( p->ro.W.H < p->ro.W.L && err >= 0 )
      && !( p->ro.W.L < p->ro.W.H && err < 0 ) )
      {
        p->ri.D += MULS( p->ki, err );
      }
    p->ro.D = MULS( p->kp, err ) + p->ri.D;
    p->ro.W.L = p->ro.W.H;
    return p->ro.W.H;
  }

S16  PI_LIM( PI_t * p, S16 min, S16 max )
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

