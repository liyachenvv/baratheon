
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Hardware.c                       //
//                                                       //
///////////////////////////////////////////////////////////

#define _HARDWARE_SRC_
#include "Globals.h"
#include "Hardware.h"
#include "Motor.h"

__CONFIG( 0X0F9C );
__CONFIG( 0X3FFC );

static bank1 U8  ODRA;
static bank1 U8  scnHall;
static bank1 U8  scnZero;
static bank1 U8  scnTest;
static bank1 U8  enADC;
static bank1 U8  tmADVAC;
static bank1 U8  tmADVRS;
static bank1 U8  tmADNTC;
static bank1 U8  tmTST0;
static bank1 U8  tmTST1;
static bank1 U16 sysTimer;
static bank1 U16 tmTriacOn;
static bank1 U16 tmTriacOff;
static bank1 U32 adCur2Buf;
static bank1 U8  adCurNBuf;
static bank1 U16 prevTrig;
static bank1 U8  trigFlag;
static bank1 U8  triggering;
CROM U8  FLT[ 256 ] @ 0X1D00 =
  {
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
    0x00,   0x02,   0x04,   0x86,   0x00,   0x82,   0x84,   0xDE,
    0x60,   0x0A,   0x0C,   0x8E,   0x08,   0x8A,   0x8C,   0x8E,
  };

CROM U16 SQ8[ 256 ] @ 0x1E00 =
  {
    0x0000, 0x0001, 0x0004, 0x0009, 0x0010, 0x0019, 0x0024, 0x0031, 
    0x0040, 0x0051, 0x0064, 0x0079, 0x0090, 0x00A9, 0x00C4, 0x00E1, 
    0x0100, 0x0121, 0x0144, 0x0169, 0x0190, 0x01B9, 0x01E4, 0x0211, 
    0x0240, 0x0271, 0x02A4, 0x02D9, 0x0310, 0x0349, 0x0384, 0x03C1, 
    0x0400, 0x0441, 0x0484, 0x04C9, 0x0510, 0x0559, 0x05A4, 0x05F1, 
    0x0640, 0x0691, 0x06E4, 0x0739, 0x0790, 0x07E9, 0x0844, 0x08A1, 
    0x0900, 0x0961, 0x09C4, 0x0A29, 0x0A90, 0x0AF9, 0x0B64, 0x0BD1, 
    0x0C40, 0x0CB1, 0x0D24, 0x0D99, 0x0E10, 0x0E89, 0x0F04, 0x0F81, 
    0x1000, 0x1081, 0x1104, 0x1189, 0x1210, 0x1299, 0x1324, 0x13B1, 
    0x1440, 0x14D1, 0x1564, 0x15F9, 0x1690, 0x1729, 0x17C4, 0x1861, 
    0x1900, 0x19A1, 0x1A44, 0x1AE9, 0x1B90, 0x1C39, 0x1CE4, 0x1D91, 
    0x1E40, 0x1EF1, 0x1FA4, 0x2059, 0x2110, 0x21C9, 0x2284, 0x2341, 
    0x2400, 0x24C1, 0x2584, 0x2649, 0x2710, 0x27D9, 0x28A4, 0x2971, 
    0x2A40, 0x2B11, 0x2BE4, 0x2CB9, 0x2D90, 0x2E69, 0x2F44, 0x3021, 
    0x3100, 0x31E1, 0x32C4, 0x33A9, 0x3490, 0x3579, 0x3664, 0x3751, 
    0x3840, 0x3931, 0x3A24, 0x3B19, 0x3C10, 0x3D09, 0x3E04, 0x3F01, 
    0x4000, 0x4101, 0x4204, 0x4309, 0x4410, 0x4519, 0x4624, 0x4731, 
    0x4840, 0x4951, 0x4A64, 0x4B79, 0x4C90, 0x4DA9, 0x4EC4, 0x4FE1, 
    0x5100, 0x5221, 0x5344, 0x5469, 0x5590, 0x56B9, 0x57E4, 0x5911, 
    0x5A40, 0x5B71, 0x5CA4, 0x5DD9, 0x5F10, 0x6049, 0x6184, 0x62C1, 
    0x6400, 0x6541, 0x6684, 0x67C9, 0x6910, 0x6A59, 0x6BA4, 0x6CF1, 
    0x6E40, 0x6F91, 0x70E4, 0x7239, 0x7390, 0x74E9, 0x7644, 0x77A1, 
    0x7900, 0x7A61, 0x7BC4, 0x7D29, 0x7E90, 0x7FF9, 0x8164, 0x82D1, 
    0x8440, 0x85B1, 0x8724, 0x8899, 0x8A10, 0x8B89, 0x8D04, 0x8E81, 
    0x9000, 0x9181, 0x9304, 0x9489, 0x9610, 0x9799, 0x9924, 0x9AB1, 
    0x9C40, 0x9DD1, 0x9F64, 0xA0F9, 0xA290, 0xA429, 0xA5C4, 0xA761, 
    0xA900, 0xAAA1, 0xAC44, 0xADE9, 0xAF90, 0xB139, 0xB2E4, 0xB491, 
    0xB640, 0xB7F1, 0xB9A4, 0xBB59, 0xBD10, 0xBEC9, 0xC084, 0xC241, 
    0xC400, 0xC5C1, 0xC784, 0xC949, 0xCB10, 0xCCD9, 0xCEA4, 0xD071, 
    0xD240, 0xD411, 0xD5E4, 0xD7B9, 0xD990, 0xDB69, 0xDD44, 0xDF21, 
    0xE100, 0xE2E1, 0xE4C4, 0xE6A9, 0xE890, 0xEA79, 0xEC64, 0xEE51, 
    0xF040, 0xF231, 0xF424, 0xF619, 0xF810, 0xFA09, 0xFC04, 0xFE01
  };
  
void MCU_Init( void )
  {
    __DI( );
    // Initialize System Register.
    OSCCON  = CFG_OSCCON;
    OSCTUNE = CFG_OSCTUNE;
    // Initialize Watchdog. Underflow Time = 250mS.
    WDTCON  = 0X11;
    // Initialize I/O port function.
    APFCON0 = CFG_APFCON0;
    APFCON1 = CFG_APFCON1;
    ODRA    = CFG_PORTA;
    LATA    = CFG_PORTA;
    TRISA   = CFG_TRISA;
    ANSELA  = CFG_ANSELA;
    WPUA    = CFG_WPUA;
    INLVLA  = CFG_INLVLA;
    LATB    = CFG_PORTB;
    TRISB   = CFG_TRISB;
    ANSELB  = CFG_ANSELB;
    WPUB    = CFG_WPUB;
    INLVLB  = CFG_INLVLB;
    LATC    = CFG_PORTC;
    TRISC   = CFG_TRISC;
    ANSELC  = CFG_ANSELC;
    WPUC    = CFG_WPUC;
    INLVLC  = CFG_INLVLC;
    FVRCON  = CFG_FVRCON;
    DACCON1 = CFG_DACCON1;
    DACCON0 = CFG_DACCON0;
    CM1CON1 = CFG_CM1CON1;
    CM1CON0 = CFG_CM1CON0;
    CM2CON1 = CFG_CM2CON1;
    CM2CON0 = CFG_CM2CON0;
    // Initialize ADC.
    ADCON1  = 0XA0;
    ADCON0  = AIN_CUR*4+1;
    enADC   = 1;
    tmADVAC = 255;
    tmADVRS = 255;
    tmADNTC = 255;
    adVac = 0;
    adVrs = 0;
    adNtc = 0;
    adCur2Buf = 0;
    adCurNBuf = 0;
    adCur2 = 0;
    adCurN = 0;
    // Initialize UART.
    BAUDCON = 0X0A;
    SPBRGL  = 799%256;
    SPBRGH  = 799/256;
    RCSTA   = 0X00;
    TXSTA   = 0X04;
    SPEN    = 1;
    CREN    = 1;
    TXEN    = 1;
    txTIM = 0;
    txCNT = 0;
    txWRP = 0;
    txRDP = 0;
    rxTIM = 0;
    rxCNT = 0;
    rxWRP = 0;
    rxRDP = 0;
    // Initialize System Timer.
    T1GCON  = 0X00;
    T1CON   = 0X30;
    PR6     = 49;
    T6CON   = 0X05;
    // Initialize Interrupt.
    PIR1    = 0X00;
    PIR2    = 0X00;
    PIR3    = 0X00;
    PIR4    = 0X00;
    PIE1    = 0X00;
    PIE2    = 0X00;
    PIE3    = 0X08;
    PIE4    = 0X00;
    PEIE    = 1;
    // Initialize Variables.
    sysTicks = 0;
    sysTimer = 0;
    tmTST0 = 255;
    tmTST1 = 255;
    idrTest0 = 0;
    idrTest1 = 0;
    scnHall = 0;
    scnZero = 0;
    scnTest = 0;
    tmTriacOn = 0; //0XFED4;  //-300  //0;
    tmTriacOff = 0; //0XFECF;  //-305  //0;
    prevTrig=0;
    trigFlag=0;
    triggering = 0;	
  }

void MCU_Refresh( void )
  {
    APFCON0 = CFG_APFCON0;
    APFCON1 = CFG_APFCON1;
    TRISA   = CFG_TRISA;
    ANSELA  = CFG_ANSELA;
    WPUA    = CFG_WPUA;
    INLVLA  = CFG_INLVLA;
    TRISB   = CFG_TRISB;
    ANSELB  = CFG_ANSELB;
    WPUB    = CFG_WPUB;
    INLVLB  = CFG_INLVLB;
    TRISC   = CFG_TRISC;
    ANSELC  = CFG_ANSELC;
    WPUC    = CFG_WPUC;
    INLVLC  = CFG_INLVLC;
    FVRCON  = CFG_FVRCON;
    DACCON0 = CFG_DACCON0;
    DACCON1 = CFG_DACCON1;
    CM1CON0 = CFG_CM1CON0;
    CM1CON1 = CFG_CM1CON1;
    CM2CON0 = CFG_CM2CON0;
    CM2CON1 = CFG_CM2CON1;
  }
  
void E2P_WriteByte( U8 addr, U8 byte )
  {
    EEADRL  = addr;
    EEDATL  = byte;
    CFGS    = 0;
    EEPGD   = 0;
    WREN    = 1;
    GIE     = 0;
    EECON2  = 0X55;
    EECON2  = 0XAA;
    WR      = 1;
    GIE     = 1;
    WREN    = 0;
  }
  
U8   E2P_ReadByte( U8 addr )
  {
    EEADRL  = addr;
    CFGS    = 0;
    EEPGD   = 0;
    RD      = 1;
    return EEDATL;
  }

void interrupt INTSR( void )
  {
            #asm
            ;++++++++++++++++++++++++++++++++++++++++++++++
            ; COMMON REGS.
            INDF0       EQU         0X00
            INDF1       EQU         0X01
            PCL         EQU         0X02
            STATUS      EQU         0X03
            C           EQU         0
            Z           EQU         2
            FSR0L       EQU         0X04
            FSR0H       EQU         0X05
            FSR1L       EQU         0X06
            FSR1H       EQU         0X07
            BSR         EQU         0X08
            WREG        EQU         0X09
            PCLATH      EQU         0X0A
            INTCON      EQU         0X0B
            ; BANK0 REGS.
            PORTA       EQU         0X0C
            PORTB       EQU         0X0D
            PORTC       EQU         0X0E
            PIR1        EQU         0X11
            TMR1IF      EQU         0
            TMR2IF      EQU         1
            TXIF        EQU         4
            RCIF        EQU         5
            PIR2        EQU         0X12
            PIR3        EQU         0X13
            TMR4IF      EQU         1
            TMR6IF      EQU         3
            PIR4        EQU         0X14
            ; BANK1 REGS.
            TRISA       EQU         0X0C
            TRISB       EQU         0X0D
            TRISC       EQU         0X0E
            PIE1        EQU         0X11
            PIE2        EQU         0X12
            PIE3        EQU         0X13
            PIE4        EQU         0X14
            ADRESL      EQU         0X1B
            ADRESH      EQU         0X1C
            ADCON0      EQU         0X1D
            ADCON1      EQU         0X1E
            ; BANK2 REGS.
            LATA        EQU         0X0C
            LATB        EQU         0X0D
            LATC        EQU         0X0E
            CM1CON0     EQU         0X11
            CM1CON1     EQU         0X12
            CM2CON0     EQU         0X13
            CM2CON1     EQU         0X14
            CMOUT       EQU         0X15
            DACCON0     EQU         0X18
            DACCON1     EQU         0X19
            ; BANK3 REGS.
            ANSELA      EQU         0X0C
            ANSELB      EQU         0X0D
            ANSELC      EQU         0X0E
            RCREG       EQU         0X19
            TXREG       EQU         0X1A
            RCSTA       EQU         0X1D
            TXSTA       EQU         0X1E
            ;++++++++++++++++++++++++++++++++++++++++++++++
            ; PORT DEFINITION.
            ODRA        EQU         (127&_ODRA)
            ODR_TRIAC   EQU         ODRA
            PIN_TRIAC   EQU         5
            ;++++++++++++++++++++++++++++++++++++++++++++++
            ; DATA DEFINITION.
            SYS_TICKS   EQU         (127&_sysTicks)
            SYS_TIMER   EQU         (127&_sysTimer)
            HL_SCAN     EQU         (127&_scnHall)
            AC_SCAN     EQU         (127&_scnZero)
            TS_SCAN     EQU         (127&_scnTest)
            nEDGE       EQU         6
            nFALL       EQU         5
            nRISE       EQU         4
            EN_ADC      EQU         (127&_enADC)
            TAD_VAC     EQU         (127&_tmADVAC)
            TAD_VRS     EQU         (127&_tmADVRS)
            TAD_NTC     EQU         (127&_tmADNTC)
            TM_TST0     EQU         (127&_tmTST0)
            TM_TST1     EQU         (127&_tmTST1)
            DR_TST0     EQU         (127&_idrTest0)
            DR_TST1     EQU         (127&_idrTest1)
            AD_VRS      EQU         (127&_adVrs)
            AD_NTC      EQU         (127&_adNtc)
            AD_VAC      EQU         (127&_adVac)
            AD_CUR_2    EQU         (127&_adCur2)
            AD_CUR_N    EQU         (127&_adCurN)
            AD_CUR_2B   EQU         (127&_adCur2Buf)
            AD_CUR_NB   EQU         (127&_adCurNBuf)
            TX_CNT      EQU         (127&_txCNT)
            TX_WRP      EQU         (127&_txWRP)
            TX_RDP      EQU         (127&_txRDP)
            TX_TIM      EQU         (127&_txTIM)
            TX_FIFO_PL  EQU         LOW (_txFIFO)
            TX_FIFO_PH  EQU         HIGH(_txFIFO)
            RX_CNT      EQU         (127&_rxCNT)
            RX_WRP      EQU         (127&_rxWRP)
            RX_RDP      EQU         (127&_rxRDP)
            RX_TIM      EQU         (127&_rxTIM)
            RX_FIFO_PL  EQU         LOW (_rxFIFO)
            RX_FIFO_PH  EQU         HIGH(_rxFIFO)
            AC_CYCLE    EQU         (127&_acCycle)
            AC_CYCLE_2  EQU         (127&_acCycleHalf)
            AC_FLAG     EQU         (127&_acCycleFlag)
            AC_INDEX    EQU         (127&_acCrossIndex)
            AC_EVENT    EQU         (127&_acCrossEvent)
            AC_WATCH    EQU         (127&_acCrossWDT)
            AC_TIME_PL  EQU         LOW (_acCrossTime)
            AC_TIME_PH  EQU         HIGH(_acCrossTime)
            HL_INDEX    EQU         (127&_mtrHallIndex)
            HL_EVENT    EQU         (127&_mtrHallEvent)
            HL_WATCH    EQU         (127&_mtrHallWDT)
            HL_TIME_PL  EQU         LOW (_mtrHallTime)
            HL_TIME_PH  EQU         HIGH(_mtrHallTime)
            TM_TRIAC    EQU         (127&_tmTriac)
            TMR_ON      EQU         (127&_tmTriacOn)
            TMR_OFF     EQU         (127&_tmTriacOff)
            MTRERR      EQU         (127&_mtrError)   
            PREV_TRIG   EQU         (127&_prevTrig)
            TRIG_FLAG   EQU         (127&_trigFlag)
            TRIGGERING  EQU         (127&_triggering)
			
            SUB16       macro       alb, ahb, blb, bhb
                        movf       alb,W
                        subwf       blb
                        movf       ahb,W
                        skpc
                        incfsz      ahb,        W
                        subwf       bhb
                        endm 
            ;++++++++++++++++++++++++++++++++++++++++++++++
           ;MOVLP       0
            MOVLB       1
            BCF         TRISA,      1    ;RA1=0            
            BTFSC       EN_ADC,     0
            BSF         ADCON0,     1
            BSF         ODRA,       2
            MOVF        ODRA,       W
            MOVLB       2
            XORWF       LATA,       W
            ANDLW       0X26
            XORWF       LATA,       F
            MOVLB       0
            BCF         PIR3,       TMR6IF      ; 12
            ;++++++++++++++++++++++++++++++++++++++++++++++
            MOVLW       HIGH _FLT | 0X80
            MOVWF       FSR0H
            MOVLB       1                       ; 3
            ;++++++++++++++++++++++++++++++++++++++++++++++
            SCAN_HALL:
            MOVF        HL_SCAN,    W
            MOVLB       0
            BTFSC       PORTC,      5
            IORLW       1
            MOVWF       FSR0L
            MOVIW       [0]FSR0
            MOVLB       1
            MOVWF       HL_SCAN                 ; 9
            ;++++++++++++++++++++++++++++++++++++++++++++++
            SCAN_ZERO:
            MOVF        AC_SCAN,    W
            MOVLB       2
            BTFSC       CMOUT,      1
            IORLW       1
            MOVWF       FSR0L
            MOVIW       [0]FSR0
            MOVLB       1
            MOVWF       AC_SCAN                 ; 9
            ;++++++++++++++++++++++++++++++++++++++++++++++
            SCAN_TEST:
            MOVF        TS_SCAN,    W
            MOVLB       2
            BTFSC       CMOUT,      0
            IORLW       1
            MOVWF       FSR0L
            MOVIW       [0]FSR0
            MOVLB       1
            MOVWF       TS_SCAN                 ; 9
            ;++++++++++++++++++++++++++++++++++++++++++++++
            HALL_CHECK:
            BTFSS       HL_SCAN,    nFALL
            GOTO        HALL_CHECK_E            ; 3
            LSLF        HL_INDEX,   W
            ANDLW       31*2
            ADDLW       HL_TIME_PL
            MOVWF       FSR0L
            MOVLW       HL_TIME_PH
            MOVWF       FSR0H
            MOVF        SYS_TIMER,  W
            MOVWI       FSR0++
            MOVF        SYS_TIMER+1,W
            MOVWI       FSR0++
            INCF        HL_INDEX,   F
            INCF        HL_EVENT,   F
            CLRF        HL_WATCH                ; 15
            HALL_CHECK_E:
            ;++++++++++++++++++++++++++++++++++++++++++++++
            ZERO_CHECK:

            BTFSS       TRIGGERING,   0
            BCF         ODR_TRIAC,  PIN_TRIAC
            BTFSC       TRIG_FLAG,   0	
			BCF         ODR_TRIAC,  PIN_TRIAC
            BTFSC       AC_SCAN,    nEDGE
            GOTO        ZERO_EDGE               ; 4
            ZERO_TRIG:
            ;MOVLB       2
            ;MOVF        LATA,    w
            ;ANDLW       0XFD
            ;MOVWF       LATA
            ;MOVLB       1
            MOVLB       1
            BCF         ODRA,       1
            MOVF        ODRA,       W
            MOVLB       2
            XORWF       LATA,       W
            ANDLW       0X26
            XORWF       LATA,       F
            MOVLB       1
            
            BTFSS       TMR_OFF+1,  7
            GOTO        ZERO_CHECK_E            ; 6
            INCFSZ      TMR_OFF,    F
            GOTO        $+2
            INCF        TMR_OFF+1,  F
            INCFSZ      TMR_ON,     F
            GOTO        $+2
            INCF        TMR_ON+1,   F
            BTFSC       TMR_ON+1,   7
            GOTO        ZERO_CHECK_E            ; 14

            BTFSC       TRIG_FLAG,  0
			GOTO        ZERO_CHECK_E
            MOVLW       PREV_TRIG
            MOVWF       FSR1L
            MOVLW       PREV_TRIG+1
            MOVWF       FSR1H
            SUB16       SYS_TIMER,SYS_TIMER+1,FSR1L,FSR1H
            BTFSS       FSR1H,7
            GOTO        POS_MINUS
            COMF        FSR1L
            COMF        FSR1H       
            INCF        FSR1L      
            ;SUB16       SYS_TIMER,SYS_TIMER+1,PREV_TRIG,PREV_TRIG+1
            ;BTFSS       PREV_TRIG+1,7
            ;GOTO        POS_MINUS
            ;COMF        PREV_TRIG
            ;COMF        PREV_TRIG+1        
            ;INCF        PREV_TRIG              
            POS_MINUS:
            ;300=0X12C 200=0XC8 240=0XF0
            MOVLW       0XC8
            SUBWF       FSR1L,   W
            MOVLW       0X00
            SUBWFB      FSR1H, W
            ;MOVLW       0XC8
            ;SUBWF       PREV_TRIG,   W
            ;MOVLW       0X00
            ;SUBWFB      PREV_TRIG+1, W         

            BTFSS       STATUS,     C  
            GOTO        TIMING_TRIG           
            BSF         ODR_TRIAC,  PIN_TRIAC  
            BCF         TRIG_FLAG,   0        
            BSF         TRIGGERING,  0
            TIMING_TRIG:             
            MOVF        TMR_ON,   W
            SUBLW       20                      ;5->20
            BTFSS       STATUS,    Z
            ;BTFSC       STATUS,     C   ;yes,here. it is wondrous.
            ;if use this one, the speed will surge, pulsed.
            GOTO        ZERO_CHECK_E            ; 16            
            MOVF        SYS_TIMER,    W
            MOVWF       PREV_TRIG
            MOVF        SYS_TIMER+1,   W
            MOVWF       PREV_TRIG+1
            MOVLW       0XF0
            MOVWF       TMR_ON
            MOVLW       0XF0
            MOVWF       TMR_ON+1          ; //0XFED4=-300,0xF0F0=-3856
            MOVLW       0X0F
            MOVWF       TMR_OFF
            MOVLW       0X0F
            MOVWF       TMR_OFF+1          ; //0XFED4=-300,0xF0F0=-3856
           
            ;MOVLB       1
            BSF         TRIG_FLAG,   0
            BCF         TRIGGERING,  0
            GOTO        ZERO_CHECK_E
            ZERO_EDGE:
            ;MOVLB       2
            ;MOVF        LATA,    w
            ;IORLW       0x02
            ;MOVWF       LATA
            ;MOVLB       1            
            ;MOVLB       1
            BSF         ODRA,       1
            MOVF        ODRA,       W
            MOVLB       2
            XORWF       LATA,       W
            ANDLW       0X26
            XORWF       LATA,       F
            MOVLB       1      
			BCF         TRIG_FLAG,  0
			BCF         TRIGGERING,  0
            CLRF        TMR_OFF+1
            MOVF        AC_CYCLE_2, W
            BTFSC       STATUS,     Z
            GOTO        ZERO_CYCLE              ; 9
            ADDLW       256-3
            MOVWF       TM_TST0
            ADDLW       10
            MOVWF       TM_TST1
            MOVLW       55                      ;40->55
            SUBWF       TM_TRIAC,   W
            MOVLW       0
            SUBWFB      TM_TRIAC+1, W
            BTFSS       STATUS,     C
            GOTO        ZERO_CYCLE              ; 19
            MOVF        AC_CYCLE,   W
            SUBWF       TM_TRIAC,   W
            MOVWF       TMR_ON
            MOVF        AC_CYCLE+1, W
            SUBWFB      TM_TRIAC+1, W
            MOVWF       TMR_ON+1
            MOVLW       240
            BTFSC       STATUS,     C
            MOVWF       TMR_ON
            BTFSC       STATUS,     C
            MOVWF       TMR_ON+1
            MOVF        AC_CYCLE,   W
            SUBLW       36
            MOVWF       FSR0L
            MOVF        AC_CYCLE+1, W
            SUBWFB      TMR_OFF+1,  W
            MOVWF       FSR0H
            MOVLW       20                    ;5->20       
            SUBWF       TMR_ON,     W
            MOVWF       TMR_OFF
            MOVLW       0
            SUBWFB      TMR_ON+1,   W
            MOVWF       TMR_OFF+1
            MOVF        FSR0L,      W
            SUBWF       TMR_OFF,    W
            MOVF        FSR0H,      W
            SUBWFB      TMR_OFF+1,  W
            BTFSC       STATUS,     C
            GOTO        ZERO_CYCLE              ; 48
            MOVF        FSR0L,      W
            MOVWF       TMR_OFF
            MOVF        FSR0H,      W
            MOVWF       TMR_OFF+1               ; 51
            ZERO_CYCLE:
            BTFSS       AC_SCAN,    nRISE
            GOTO        ZERO_CHECK_E
            BSF         AC_FLAG,    0
            LSLF        AC_INDEX,   W
            ANDLW       7*2
            ADDLW       AC_TIME_PL
            MOVWF       FSR0L
            MOVLW       AC_TIME_PH
            MOVWF       FSR0H
            MOVF        SYS_TIMER,  W
            MOVWI       FSR0++
            MOVF        SYS_TIMER+1,W
            MOVWI       FSR0++
            INCF        AC_INDEX,   F
            INCF        AC_EVENT,   F
            CLRF        AC_WATCH
            MOVF        AD_CUR_2B,  W
            MOVWF       AD_CUR_2
            MOVF        AD_CUR_2B+1,W
            MOVWF       AD_CUR_2+1
            MOVF        AD_CUR_2B+2,W
            MOVWF       AD_CUR_2+2
            MOVF        AD_CUR_2B+3,W
            MOVWF       AD_CUR_2+3
            MOVF        AD_CUR_NB,  W
            MOVWF       AD_CUR_N
            CLRF        AD_CUR_2B
            CLRF        AD_CUR_2B+1
            CLRF        AD_CUR_2B+2
            CLRF        AD_CUR_2B+3
            CLRF        AD_CUR_NB
            MOVF        AC_CYCLE_2, W
            BTFSC       STATUS,     Z
            GOTO        ZERO_CHECK_E
            ADDLW       2
            MOVWF       TAD_VAC
            #if         VER_110VAC
            ADDLW       8
            #endif
            #if         VER_220VAC
            ADDLW       1
            #endif
            MOVWF       TAD_VRS
            ADDLW       2
            MOVWF       TAD_NTC
            ZERO_CHECK_E:
            ;++++++++++++++++++++++++++++++++++++++++++++++
            MOVF        SYS_TIMER,  W
            INCFSZ      SYS_TIMER,  F
            GOTO        $+2
            INCF        SYS_TIMER+1,F
            ANDLW       3
            BRW
            GOTO        ISR_BRCH_0              ; 9
            GOTO        ISR_BRCH_1              ; 9
            GOTO        ISR_BRCH_2              ; 9
            GOTO        ISR_BRCH_3              ; 9
            ;++++++++++++++++++++++++++++++++++++++++++++++
            ISR_BRCH_0:
            UART_TX_PRO:
            MOVLB       3
            BTFSS       TX_TIM,     7
            INCF        TX_TIM,     F
            MOVF        TX_CNT,     W
            BTFSC       STATUS,     Z
            GOTO        UART_TX_PRO_E           ; 7
            MOVLW       12
            SUBWF       TX_TIM,     W
            BTFSS       STATUS,     C
            GOTO        UART_TX_PRO_E           ; 11
            CLRF        TX_TIM
            MOVF        TX_RDP,     W
            ANDLW       31
            ADDLW       TX_FIFO_PL
            MOVWF       FSR0L
            MOVLW       TX_FIFO_PH
            MOVWF       FSR0H
            MOVIW       [0]FSR0
            MOVWF       TXREG
            INCF        TX_RDP,     F
            DECF        TX_CNT,     F           ; 21
            UART_TX_PRO_E:
            MOVLB       1
            INCF        SYS_TICKS,  F
            BTFSS       EN_ADC,     0
            GOTO        AD_VAC_START
            AD_CUR_READ:
            BTFSC       ADCON0,     1
            GOTO        $-1                     ; ?
            COMF        ADRESL,     W
            MOVWF       FSR0L
            MOVLW       HIGH _SQ8 | 0X80
            MOVWF       FSR0H
            LSLF        FSR0L,      F
            BTFSC       STATUS,     C
            INCF        FSR0H,      F
            MOVIW       FSR0++
            ADDWF       AD_CUR_2B,  F
            MOVIW       FSR0++
            ADDWFC      AD_CUR_2B+1,F
            CLRW
            ADDWFC      AD_CUR_2B+2,F
            ADDWFC      AD_CUR_2B+3,F
            INCF        AD_CUR_NB,  F
            AD_VAC_START:
            CLRF        EN_ADC
            BTFSS       TAD_VAC,    7
            DECFSZ      TAD_VAC,    F
            RETFIE
            MOVLW       AIN_VAC*4+1
            MOVWF       ADCON0
            MOVLW       0X20
            MOVWF       ADCON1
            BSF         EN_ADC,     0
            RETFIE
            ;++++++++++++++++++++++++++++++++++++++++++++++
            ISR_BRCH_1:
            UART_RX_PRO:
            MOVLB       3
            BTFSS       RX_TIM,     7
            INCF        RX_TIM,     F
            MOVLB       0
            BTFSS       PIR1,       RCIF
            GOTO        UART_RX_PRO_E           ; 11
            MOVLB       3
            CLRF        RX_TIM
            MOVF        RCREG,      W
            MOVWF       FSR0H
            MOVLW       32
            SUBWF       RX_CNT,     W
            BTFSS       STATUS,     C
            GOTO        UART_RX_PRO_E           ; 19
            MOVF        RX_WRP,     W
            ANDLW       31
            ADDLW       RX_FIFO_PL
            MOVWF       FSR0L
            MOVLW       RX_FIFO_PH
            XORWF       FSR0H,      W
            XORWF       FSR0H,      F
            XORWF       FSR0H,      W
            MOVWI       [0]FSR0
            INCF        RX_WRP,     F
            INCF        RX_CNT,     F
            UART_RX_PRO_E:
            MOVLB       1
            BTFSS       EN_ADC,     0
            GOTO        AD_VRS_START
            AD_VAC_READ:
            BTFSC       ADCON0,     1
            GOTO        $-1                     ; ?
            MOVF        ADRESH,     W
            SUBLW       128
            BTFSS       STATUS,     C
            SUBLW       0
            MOVWF       AD_VAC
            AD_VRS_START:
            CLRF        EN_ADC
            BTFSS       TAD_VRS,    7
            DECFSZ      TAD_VRS,    F
            RETFIE
            MOVLW       AIN_VRS*4+1
            MOVWF       ADCON0
            MOVLW       0X20
            MOVWF       ADCON1
            BSF         EN_ADC,     0
            RETFIE
            ;++++++++++++++++++++++++++++++++++++++++++++++
            ISR_BRCH_2:
            TST0_PRO:
            BTFSS       TM_TST0,    7
            DECFSZ      TM_TST0,    F
            GOTO        TST0_PRO_E
            MOVF        TS_SCAN,    W
            XORWF       AC_SCAN,    W
            XORLW       0X80
            RLF         WREG,       W
            RLF         DR_TST0,    F
            TST0_PRO_E:
            BTFSS       EN_ADC,     0
            GOTO        AD_NTC_START
            AD_VRS_READ:
            BTFSC       ADCON0,     1
            GOTO        $-1                     ; ?
            MOVF        ADRESH,     W
            MOVWF       AD_VRS
            AD_NTC_START:
            CLRF        EN_ADC
            BTFSS       TAD_NTC,    7
            DECFSZ      TAD_NTC,    F
            RETFIE
            MOVLW       AIN_NTC*4+1
            MOVWF       ADCON0
            MOVLW       0X20
            MOVWF       ADCON1
            BSF         EN_ADC,     0
            RETFIE
            ;++++++++++++++++++++++++++++++++++++++++++++++
            ISR_BRCH_3:
            TST1_PRO:
            BTFSS       TM_TST1,    7
            DECFSZ      TM_TST1,    F
            GOTO        TST1_PRO_E
            MOVF        TS_SCAN,    W
            XORWF       AC_SCAN,    W
            XORLW       0X80
            RLF         WREG,       W
            RLF         DR_TST1,    F
            TST1_PRO_E:
            BTFSS       EN_ADC,     0
            GOTO        AD_CUR_START
            AD_NTC_READ:
            BTFSC       ADCON0,     1
            GOTO        $-1                     ; ?
            MOVF        ADRESH,     W
            MOVWF       AD_NTC
            AD_CUR_START:
            MOVLW       AIN_CUR*4+1
            MOVWF       ADCON0
            MOVLW       0XA0
            MOVWF       ADCON1
            BSF         EN_ADC,     0
            RETFIE
            ;++++++++++++++++++++++++++++++++++++++++++++++
            #endasm
  }
