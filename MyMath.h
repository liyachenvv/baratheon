
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
    SDW  ri;
    SDW  ro;
  } PI_t;
  
EXTERN U16  SQRT32( U32 a );
EXTERN S16  PI_CAL( PI_t * p, S16 err );
EXTERN S16  PI_LIM( PI_t * p, S16 min, S16 max );

#endif

