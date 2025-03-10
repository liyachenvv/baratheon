
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   PicTypes.h                       //
//                                                       //
///////////////////////////////////////////////////////////

#ifndef _PIC_TYPES_H_
#define _PIC_TYPES_H_

#define SRAM
#define XRAM
#define CROM  const

typedef unsigned char   U8;
typedef unsigned short  U16;
typedef unsigned long   U32;
typedef signed char     S8;
typedef signed short    S16;
typedef signed long     S32;

typedef union
  {
    U8 B;
    struct
      {
        unsigned char b0 : 1;
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;
      } bits;
  } BYTE;

typedef union
  {
    U16 W;
    struct
      {
        U8  L;
        U8  H;
      } B;
  } WORD;

typedef union
  {
    U32 D;
    struct
      {
        U16 L;
        U16 H;
      } W;
    struct
      {
        U8  L;
        U8  H;
        U8  M;
        U8  T;
      } B;
  } DWORD;

#define BIT7  0X80
#define BIT6  0X40
#define BIT5  0X20
#define BIT4  0X10
#define BIT3  0X08
#define BIT2  0X04
#define BIT1  0X02
#define BIT0  0X01
  
#define BMSK(N)       (1U<<(N))
#define BSET(X,N)     (X)|=BMSK(N)
#define BCLR(X,N)     (X)&=(BMSK(N)^-1)
#define BCPL(X,N)     (X)^=BMSK(N)   //converse position logical，not.
#define BTST(X,N)     ((X)&BMSK(N))
#define BSETEX(X,N)   X[(N)>>3]|=((U8)BMSK((N)&7))
#define BCLREX(X,N)   X[(N)>>3]&=(((U8)BMSK((N)&7))^-1)
#define BCPLEX(X,N)   X[(N)>>3]^=((U8)BMSK((N)&7))
#define BTSTEX(X,N)   (X[(N)>>3]&((U8)BMSK((N)&7)))
#define LENOF(ARR)    ((U8)(sizeof(ARR)/sizeof(ARR[0])))

#define __DI( )       GIE = 0
#define __EI( )       GIE = 1
#define __NOP( )      asm(" NOP           ")
#define __WDTC( )     asm(" CLRWDT        ")

#endif

