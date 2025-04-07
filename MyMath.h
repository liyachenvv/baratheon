
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   MyMath.h                         //
//                                                       //
///////////////////////////////////////////////////////////

#ifndef _MY_MATH_H_
#define _MY_MATH_H_

#ifdef  EXTERN
#undef  EXTERN
#endif
#ifdef  _MY_MATH_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

typedef union
  {
    S32  D;
    struct { S16 L; S16 H; } W;
  } SDW;

typedef struct
  {
    S16  kp;
    S16  ki;
    S16  kd;
    S16  e1;
    SDW  ri;
    SDW  ro;
  } PID_t;

#define MULU32( a, b )  ( ( U32 )a * ( U32 )b )
#define MULS32( a, b )  ( ( S32 )a * ( S32 )b )
  
EXTERN U16  SQRT32( U32 a );
EXTERN S16  PID_CAL( PID_t * p, S16 err );
EXTERN S16  PID_LIM( PID_t * p, S16 min, S16 max );

bank3 U32 pidKp;
bank3 U32 pidKi;
bank3 U32 pidKd;

#endif

