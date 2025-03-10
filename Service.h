
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Service.h                        //
//                                                       //
///////////////////////////////////////////////////////////

#ifndef _SERVICE_H_
#define _SERVICE_H_

#ifdef  EXTERN
#undef  EXTERN
#endif
#ifdef  _SERVICE_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#if     VER_110VAC
#define PRODUCT_NAME_0        'B'
#define PRODUCT_NAME_1        'A'
#define PRODUCT_NAME_2        'R'
#define PRODUCT_NUM           110
#define PCBA_NUM              5520070209LL
#define PCBA_NUM_0            0X00
#define PCBA_NUM_1            0X00
#define PCBA_NUM_2            0X01
#define PCBA_NUM_3            0X49
#define PCBA_NUM_4            0X05
#define PCBA_NUM_5            0X96
#define PCBA_NUM_6            0X41
#define SOFTWARE_NUM          5520070209LL
#define SOFTWARE_NUM_0        0X00
#define SOFTWARE_NUM_1        0X00
#define SOFTWARE_NUM_2        0X01
#define SOFTWARE_NUM_3        0X49
#define SOFTWARE_NUM_4        0X05
#define SOFTWARE_NUM_5        0X96
#define SOFTWARE_NUM_6        0X41
#endif
#if     VER_220VAC
#define PRODUCT_NAME_0        'B'
#define PRODUCT_NAME_1        'A'
#define PRODUCT_NAME_2        'R'
#define PRODUCT_NUM           220
#define PCBA_NUM              5520070189LL
#define PCBA_NUM_0            0X00
#define PCBA_NUM_1            0X00
#define PCBA_NUM_2            0X01
#define PCBA_NUM_3            0X49  //73 
#define PCBA_NUM_4            0X05  //5
#define PCBA_NUM_5            0X96  //150
#define PCBA_NUM_6            0X2D  //45
#define SOFTWARE_NUM          5520070189LL
#define SOFTWARE_NUM_0        0X00
#define SOFTWARE_NUM_1        0X00
#define SOFTWARE_NUM_2        0X01
#define SOFTWARE_NUM_3        0X49
#define SOFTWARE_NUM_4        0X05
#define SOFTWARE_NUM_5        0X96
#define SOFTWARE_NUM_6        0X2D
#endif
#define SOFTWARE_VER_0        0
#define SOFTWARE_VER_1        2
#define SOFTWARE_VER_2        5
#define SERIAL_NUM_0          0
#define SERIAL_NUM_1          0
#define SERIAL_NUM_2          0

#define TUNE_VALUE_DEFAULT    20

EXTERN XRAM U8 e2pWrFlags;

EXTERN void SVC_Init( void );
EXTERN void SVC_Ctrl( void );
EXTERN U8   SVC_GetTuneValue( void );
EXTERN void SVC_SetTuneValue( U8 value );

#endif

