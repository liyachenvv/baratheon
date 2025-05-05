
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
#define PRODUCT_NAME_0        'W'
#define PRODUCT_NAME_1        'I'
#define PRODUCT_NAME_2        'N'
#define PRODUCT_NUM           110
#define PCBA_NUM              7320080381LL
#define PCBA_NUM_0            0X00
#define PCBA_NUM_1            0X00
#define PCBA_NUM_2            0X01
#define PCBA_NUM_3            0XB4
#define PCBA_NUM_4            0X4F
#define PCBA_NUM_5            0X8F
#define PCBA_NUM_6            0XFD
#define SOFTWARE_NUM          7320080381LL
#define SOFTWARE_NUM_0        0X00
#define SOFTWARE_NUM_1        0X00
#define SOFTWARE_NUM_2        0X01
#define SOFTWARE_NUM_3        0XB4
#define SOFTWARE_NUM_4        0X4F
#define SOFTWARE_NUM_5        0X8F
#define SOFTWARE_NUM_6        0XFD
#endif
#if     VER_220VAC
#define PRODUCT_NAME_0        'W'
#define PRODUCT_NAME_1        'I'
#define PRODUCT_NAME_2        'N'
#define PRODUCT_NUM           220
#define PCBA_NUM              7320080381LL
#define PCBA_NUM_0            0X00
#define PCBA_NUM_1            0X00
#define PCBA_NUM_2            0X01
#define PCBA_NUM_3            0XB4
#define PCBA_NUM_4            0X4F
#define PCBA_NUM_5            0X8F
#define PCBA_NUM_6            0XFD
#define SOFTWARE_NUM          7320080381LL
#define SOFTWARE_NUM_0        0X00
#define SOFTWARE_NUM_1        0X00
#define SOFTWARE_NUM_2        0X01
#define SOFTWARE_NUM_3        0XB4
#define SOFTWARE_NUM_4        0X4F
#define SOFTWARE_NUM_5        0X8F
#define SOFTWARE_NUM_6        0XFD
#endif
#define SOFTWARE_VER_0        0
#define SOFTWARE_VER_1        0
#define SOFTWARE_VER_2        2
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

