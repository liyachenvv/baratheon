
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Hardware.h                       //
//                                                       //
///////////////////////////////////////////////////////////

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef _HARDWARE_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#define CFG_OSCCON    0XF0  //IRCF=1110 8MHz. FOSC internel osc. so clock is 32MHz
#define CFG_OSCTUNE   0X00  //osc runnig at factory calibrated frequency

#define CFG_APFCON0   0X00  //uart=rb5 rb7
#define CFG_APFCON1   0X00  //SDO2=RC1, SS2=RC0, P1D=RC2, P1C=RC3,P2B=RC2,CCP2=RC3(motor error detect)

#define CFG_PORTA     0X04  //RA2=1 high, (connect to gnd. also use as C1out), others low 0V.
#define CFG_TRISA     0X1B  //0001 1011 A5 (motor control),A2(comparator C1out)=out, A4,A3,A1,A0=in (tri-stated), 
#define CFG_ANSELA    0X11  //0001 0001 A0(ICSPDAT, use as C1in+=2.5V),A4(NTC)=Analog
#define CFG_WPUA      0X00  //pull up disabled (why A5 not pull up? it should be good)
#define CFG_INLVLA    0X00  //TTL input

#define CFG_PORTB     0XE0  //1110 0000 B5,B6,B7=high
#define CFG_TRISB     0X30  //0011 0000 B4 (gnd),B5(Rx)=in,B6(Relay),B7(Tx)=out
#define CFG_ANSELB    0X00  //digital 
#define CFG_WPUB      0X00  //no pull up
#define CFG_INLVLB    0X20  //0010 0000, B5=ST input

#define CFG_PORTC     0X00  //all low 
#define CFG_TRISC     0XEF  //1110 1111 RC4(20K to motor error detect)=out, others in.
#define CFG_ANSELC    0XCF  //1100 1111 RC4(C2out),RC5=digital. RC1(zero cross, analog ac wave), RC0(ICSPDAT,2.5V)
#define CFG_WPUC      0X00  //no pull up
#define CFG_INLVLC    0X00  //TTL input 

#define CFG_FVRCON    0X00  //not use fixed voltage reference
#define CFG_DACCON0   0X00  //dac is disabled
#define CFG_DACCON1   0X00
#define CFG_CM1CON0   0X82  //1000 0010 comparator C1 enabled. non-invert polarity. C1out(ra2) is internal only
                            //comparator hysteresis enable, output to T1 and I/O is asynchronous
#define CFG_CM1CON1   0X03  //0000 0011  no interrupt on rise/down edge. c1vp=RA0 (2.5V), c1vn=c12in3-(RC3 motor error detect) 
#define CFG_CM2CON0   0XA2  //1010 0010  comparator C2 enabled. normal use. C2OE=1, overwrite port pin RC4.C2out to pin RC4, then to RC3(c1vn)
#define CFG_CM2CON1   0X01  //0000 0001  no interrupt on rise/down edge. c2vp=RC0 2.5V, c2vn=c12in1-(RC1 zero cross) 

#define AIN_CUR       9  //AN9 current detect
#define AIN_VRS       6  //AN6 speed,Variable resistor 
#define AIN_NTC       3  //AN3 
#define AIN_VAC       5  //AN5 zero cross

#define SYS_RELAY_ON( )   LATB6 = 1  //output RB6 high,T1 off, relay close. has power
#define SYS_RELAY_OFF( )  LATB6 = 0  //output RB6 low, T1 on, relay open, no power

EXTERN bank1 U8  sysTicks;

EXTERN bank1 U32 adCur2;
EXTERN bank1 U8  adCurN;

EXTERN bank1 U8  adVrs;   //AN6 speed,Variable resistor 
EXTERN bank1 U8  adNtc;   //convert result of NTC.
EXTERN bank1 U8  adVac;   //AN5 zero cross

EXTERN bank1 U8  idrTest0;
EXTERN bank1 U8  idrTest1;

EXTERN XRAM  U16 mtrHallTime[ 32 ]  @ 0x320;   //32*2 bytes
EXTERN bank1 U8  mtrHallIndex;
EXTERN bank1 U8  mtrHallEvent;
EXTERN bank1 U8  mtrHallWDT;

EXTERN XRAM  U16 acCrossTime[ 8 ]   @ 0x360;   //360-320=40H=64=32*2 bytes size.
EXTERN bank1 U8  acCrossIndex;
EXTERN bank1 U8  acCrossEvent;
EXTERN bank1 U8  acCrossWDT;
EXTERN bank1 U16 acCycle;
EXTERN bank1 U8  acCycleHalf;
EXTERN bank1 U8  acCycleFlag;
EXTERN bank1 U16 tmTriac;

EXTERN bank3 U8  txCNT;
EXTERN bank3 U8  txWRP;
EXTERN bank3 U8  txRDP;
EXTERN bank3 U8  txTIM;
EXTERN XRAM  U8  txFIFO[ 32 ]       @ 0x3A0;
EXTERN bank3 U8  rxCNT;
EXTERN bank3 U8  rxWRP;
EXTERN bank3 U8  rxRDP;
EXTERN bank3 U8  rxTIM;
EXTERN XRAM  U8  rxFIFO[ 32 ]       @ 0x3C0;   //3c0-3a0=20H=32

EXTERN void MCU_Init( void );
EXTERN void MCU_Refresh( void );
EXTERN void E2P_WriteByte( U8 addr, U8 byte );
EXTERN U8   E2P_ReadByte( U8 addr );

#endif

