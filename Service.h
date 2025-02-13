
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
#define PRODUCT_NAME          "KPL9000"
#define PRODUCT_NAME_0        'K'
#define PRODUCT_NAME_1        'P'
#define PRODUCT_NAME_2        'L'
#define PRODUCT_NUM_0         90
#define PRODUCT_NUM_1         0
#define PCBA_NUM              "5520070209"
#define PCBA_NUM_0            0
#define PCBA_NUM_1            0
#define PCBA_NUM_2            55
#define PCBA_NUM_3            20
#define PCBA_NUM_4            7
#define PCBA_NUM_5            2
#define PCBA_NUM_6            9
#define SOFTWARE_NUM          "1420000343"
#define SOFTWARE_NUM_0        0
#define SOFTWARE_NUM_1        0
#define SOFTWARE_NUM_2        14
#define SOFTWARE_NUM_3        20
#define SOFTWARE_NUM_4        0
#define SOFTWARE_NUM_5        3
#define SOFTWARE_NUM_6        43
#endif
#if     VER_220VAC
#define PRODUCT_NAME          "KPL9000"
#define PRODUCT_NAME_0        'K'
#define PRODUCT_NAME_1        'P'
#define PRODUCT_NAME_2        'L'
#define PRODUCT_NUM_0         90
#define PRODUCT_NUM_1         0
#define PCBA_NUM              "5520070189"
#define PCBA_NUM_0            0
#define PCBA_NUM_1            0
#define PCBA_NUM_2            55
#define PCBA_NUM_3            20
#define PCBA_NUM_4            7
#define PCBA_NUM_5            1
#define PCBA_NUM_6            89
#define SOFTWARE_NUM          "1420000333"
#define SOFTWARE_NUM_0        0
#define SOFTWARE_NUM_1        0
#define SOFTWARE_NUM_2        14
#define SOFTWARE_NUM_3        20
#define SOFTWARE_NUM_4        0
#define SOFTWARE_NUM_5        3
#define SOFTWARE_NUM_6        33
#endif
#define SOFTWARE_VER_0        1
#define SOFTWARE_VER_1        2
#define SOFTWARE_VER_2        0
#define SERIAL_NUM_0          0
#define SERIAL_NUM_1          0
#define SERIAL_NUM_2          0

#define DEFAULT_TUNE_VALUE    20

EXTERN XRAM  U8  e2pWrFlags;

EXTERN void SVC_Init( void );
EXTERN void SVC_Ctrl( void );
EXTERN U8   SVC_GetTuneValue( void );
EXTERN void SVC_SetTuneValue( U8 value );

#endif

