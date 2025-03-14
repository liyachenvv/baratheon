
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   System.c                         //
//                                                       //
///////////////////////////////////////////////////////////

#define _SYSTEM_SRC_
#include "Globals.h"
#include "System.h"
#include "Motor.h"
#include "Service.h"
#include "COM.h"
#include "Uart.h"

#define SECOND  100UL
#define MINUTE  SECOND*60

//ad variable resistor,-> speed level
U8 CAL_MtrLevel( void )   
  {
    U8  ofs;
    U8  ad;
    ofs = SVC_GetTuneValue( );
    if ( ofs < 20 )  //20 is TUNE_VALUE_DEFAULT
      {
        ofs = 20 - ofs;
        ad = adVrs + ofs;  //if ad bigger than 255, discard bit8, lead to ad smaller.
        if ( ad < adVrs ) ad = 255;   //if ofs=10,adVrs=250,ad=260=>4. ad<adVrs
      }
    else
      {
        ofs = ofs - 20;
        ad = adVrs - ofs;  //if ad<0, complement, negative value bigger than 127.
        if ( ad > adVrs ) ad = 0;  //if ofs=40,adVrs=18,ad=18-20=-2=>254, ad>adVrs
      }
    if      ( !acCycleHalf )  mtrLevel = 0;   //no ac power,no zero across, stop
    if ( ad <  12     )  mtrLevel = 10;  //pulse 13501 rps, &1 to distinguish max.
    else if ( ad <  42 - 1 )  mtrLevel = 1;  //2500 fold  -->off
    else if ( ad <  42 + 1 )  mtrLevel = mtrLevel < 2 ? 1 : 2;
    else if ( ad <  94 - 1 )  mtrLevel = 2;  //3000 min
    else if ( ad <  94 + 1 )  mtrLevel = mtrLevel < 3 ? 2 : 3;
    else if ( ad < 117 - 1 )  mtrLevel = 3;  //4000
    else if ( ad < 117 + 1 )  mtrLevel = mtrLevel < 4 ? 3 : 4;
    else if ( ad < 137 - 1 )  mtrLevel = 4;
    else if ( ad < 137 + 1 )  mtrLevel = mtrLevel < 5 ? 4 : 5;
    else if ( ad < 158 - 1 )  mtrLevel = 5;
    else if ( ad < 158 + 1 )  mtrLevel = mtrLevel < 6 ? 5 : 6;
    else if ( ad < 184 - 1 )  mtrLevel = 6;
    else if ( ad < 184 + 1 )  mtrLevel = mtrLevel < 7 ? 6 : 7;
    else if ( ad < 209 - 1 )  mtrLevel = 7;
    else if ( ad < 209 + 1 )  mtrLevel = mtrLevel < 8 ? 7 : 8;
    else if ( ad < 240 - 1 )  mtrLevel = 8;
    else if ( ad < 240 + 1 )  mtrLevel = mtrLevel < 9 ? 8 : 9;
    else                      mtrLevel = 9;  //Max, 13500
    return ad;
  }

//calibrate variable resistor
void CAL_TuneValue( void )  
  {
    static XRAM U8 state = 0;
    static XRAM U16 time = 0;
    static XRAM U8 min = 0;
    static XRAM U8 max = 0;
    if ( time ) time--;   //10ms each time
    switch ( state )
      {
        case 0:
            if ( acCycleHalf )
              {
                state++;
                time = 6;
              }
            break;
        case 1:
            if ( !time )  //60ms
              {
                if ( mtrLevel == 10 )
                  {
                    state++;
                  }
                else
                  {
                    state = 255;
                  }
              }
            break;
        case 2:
            if ( mtrLevel == 1 )
              {
                state++;
                time = 300;  //stop at fold, hold 3s
              }
            break;
        case 3:
            if ( mtrLevel != 1 )
              {
                state = 255;
              }
            else if ( time >= 100 )  //3s-1s=2s
              {
                min = adVrs;
                max = adVrs;
              }
            else
              {
                if ( adVrs < min ) min = adVrs;
                if ( adVrs > max ) max = adVrs;
                if ( !time )
                  {
                    if ( min < 10 || max >= 30 )   //default 20
                      {
                        state = 255;
                      }
                    else
                      {
                        state++;
                      }
                  }
              }
            break;
        case 4:
            SVC_SetTuneValue( ( min + max ) >> 1 );  //average
            state = 255;
            break;
        default:
            break;
      }
  }

void SYS_Init( void )
  {
    sysStatus = E_SYS_INIT;  //0
    sysLevel  = E_LVL_STOP;  //0
    sysFault  = 0;
    mtrLevel  = 0;
  }

void SYS_Ctrl( void )
  {
    static XRAM U32 time = 0;
    static XRAM U8 checkCount=0;
    static XRAM U8  preAdvalue;
    U8  ad;
    static XRAM U16 timingGrade = 0;
    static XRAM U8 min;
    ad=CAL_MtrLevel( );
    CAL_TuneValue( );
    if ( time ) time--;
    timingGrade++;
    switch ( sysStatus )
      {
        case E_SYS_INIT:
            mtrSpeedRef = 0;
            if(min<12)
              min=68;
            //timingGrade=0;  
            //SRL_Putc('<');
            //COM_Print(acCycleHalf);  //acCycleHalf=0.   50
            //SRL_Putc('>');
            //if dial in off, power off, all start from beginning. CheckCount=0
            //if(checkCount==0 && acCycleHalf) {
            if(checkCount==0 ) {
              //postion min
              SRL_Putc('h');
              COM_Print(timingGrade);
              SRL_Putc('j');     
              COM_Print(preAdvalue);
              SRL_Putc('k');     
              COM_Print(ad);
              SRL_Putc('m');
              COM_Print(mtrLevel);
              SRL_Putc('n');
              SRL_Putc('\n');    
              if((ad >12) && (ad<=min)) 
              {
                min=ad;
                SRL_Putc('+');
                SRL_Putc('+');
                COM_Print(min);
                SRL_Putc('+');
                SRL_Putc('+');
              }
              

              if((ad >12) && (ad <68) && (preAdvalue !=0) && (mtrLevel<3))
              {
                //rotate right. if rotate left, meaning turn off.
                if((preAdvalue+2)<ad) {
                  SRL_Putc('*');
                  SRL_Putc('*');
                  sysStatus = E_SYS_PREPARE;
                  //time=10;
                  //time = SECOND*5;   //500*10ms=5s 
                  preStatus=1;
                }
              }
              //postion 1-max, rotate left
              else if((ad >12) && (ad <68) && (preAdvalue >(ad+2))) {
                SRL_Putc('c');
                SRL_Putc('c');
                sysStatus = E_SYS_PREPARE;
                //time=50;
                preStatus=2;
              }
              //rapid rotate dial. this should not allow to go further. must start from a low ad.
              //else if((ad >43)  && (preAdvalue >(ad+2))) {
              //  SRL_Putc('a');
              //  COM_Print(0x50);  //80
              //  COM_Print(preAdvalue);
              //  COM_Print(0x4f);
              //  COM_Print(ad);
              //  COM_Print(0x4e);
              //  COM_Print(mtrLevel);
              //  SRL_Putc('a');
              //  sysStatus = E_SYS_PREPARE;
              //  time=50;
              //  preStatus=3;
              //} 
              //postion off
              else if((ad>=0)  && (preAdvalue ==0) && (mtrLevel==1 || mtrLevel==2) ) {
                SRL_Putc('t');
                SRL_Putc('t');
                sysStatus = E_SYS_PREPARE;
                //time=50;
                preStatus=4;
                //}
              }      
              else if( (ad>=0)  && (preAdvalue ==0) && (mtrLevel>2) && (mtrLevel<10) && (timingGrade==0)) {
                SRL_Putc('k');
                SRL_Putc('k');
                sysStatus = E_SYS_PREPARE;
                //time=50;
                preStatus=5;
                //}
              }   
              else if( (ad>=0)  && (mtrLevel>1) && (mtrLevel<10) ) {
                SRL_Putc('y');
                SRL_Putc('y');
                sysStatus = E_SYS_PREPARE;
                //time=50;
                preStatus=6;
                //}
              }   
              preAdvalue=ad;

            }
            else if(checkCount!=0){
              sysStatus = E_SYS_PREPARE;
              time=10;
              //time = SECOND*5;   //500*10ms=5s
            }
            break;
        case E_SYS_PREPARE:
            mtrSpeedRef = 0;
            checkCount=1;
            //if ( mtrError || !time )  //5s, motor not run.
            //  {
            //    sysFault = mtrError;
            //    sysStatus = E_SYS_TURN_OFF;
            //    time = SECOND/2;  //50
            //  }
            //else 
            if((ad >12) && (ad<=min)) {
              min=ad;
              SRL_Putc('+');
              SRL_Putc('+');
              COM_Print(min);
              SRL_Putc('+');    
              SRL_Putc('+');
            }        
            if ( idrTest0 == 0xFF && idrTest1 == 0xFF && mtrTemp >= 0 )
              {
                if(preStatus>0) {
                  SRL_Putc('j');
                  COM_Print(min);
                  SRL_Putc('t');  
                  COM_Print(timingGrade);   
                  SRL_Putc('p');  
                  COM_Print(preAdvalue); 
                  SRL_Putc('q');   
                  SRL_Putc('\n');              
                  //if(preAdvalue>68 && (preAdvalue+2)<ad)
                  //direct start from off position
                  if((min==68) && (preAdvalue+2)<ad)
                  {          
                    if(timingGrade < 200) {
                      SRL_Putc('m');  
                      sysStatus = E_SYS_RUN;
                      time = MINUTE*30;  //100*60*30    
                    }                 
                  }
                  else if((min<68) && (preAdvalue+2)<ad)   //off and rotate right
                  {
                    SRL_Putc('=');  
                    sysStatus = E_SYS_RUN;
                    time = MINUTE*30;  //100*60*30                   
                  }
                  else if(preAdvalue>(ad+2))  //rotate left
                  {
                    if((ad >12) && (ad<min)) min=ad;

                    if(min <68) {
                      SRL_Putc('-');  
                      if(preStatus!=5) {
                        sysStatus = E_SYS_RUN;
                        time = MINUTE*30;  //100*60*30  
                      }
                    }
                  }
                }
                else {  //preStatus==0
                  sysStatus = E_SYS_RUN;
                  time = MINUTE*30;  //100*60*30                     
                  COM_Print(0x14);  //20
                  SRL_Putc(';');
                }
              }
            preAdvalue=ad;
            break;
        case E_SYS_RUN:
            preStatus=0;  
            timingGrade=0;
            SRL_Putc('s');
            if ( mtrError || !time )
              {
                mtrSpeedRef = 0;
                sysFault = mtrError;
                sysStatus = E_SYS_TURN_OFF;
                time = SECOND/2;  //50
              }
            else switch ( mtrLevel )
              {
                
                default:    // 0 Stop
                    sysLevel = E_LVL_STOP;
                    mtrSpeedRef = 0;  //speed goal
                    break;
                case 1:     // Fold  ->off
                    sysLevel = E_LVL_STOP;
                    mtrSpeedRef = 0;
                    break;
                case 2:     // Min
                    sysLevel = E_LVL_1;
                    mtrSpeedRef = 3000;
                    break;
                case 3:     // 1
                    sysLevel = E_LVL_1;
                    mtrSpeedRef = 4000;
                    break;
                case 4:     // 2
                    sysLevel = E_LVL_2;
                    mtrSpeedRef = 6000;
                    break;
                case 5:     // 3
                    sysLevel = E_LVL_3;
                    mtrSpeedRef = 8000;
                    break;
                case 6:     // 4
                    sysLevel = E_LVL_4;
                    mtrSpeedRef = 9500;
                    break;
                case 7:     // 5
                    sysLevel = E_LVL_5;
                    mtrSpeedRef = 10500;
                    break;
                case 8:     // 6
                    sysLevel = E_LVL_6;
                    mtrSpeedRef = 12000;
                    break;
                case 9:     // Max
                    sysLevel = E_LVL_6;
                    mtrSpeedRef = 13500;
                    break;
                case 10:    // Pulse
                    sysLevel = E_LVL_PULSE;
                    mtrSpeedRef = 13501;
                    break;
              }
            break;
        default:  //E_SYS_TURN_OFF, E_SYS_OFF
            mtrSpeedRef = 0;
            SRL_Putc('q');
            if ( !time ) sysStatus = E_SYS_OFF;
            break;
      }
  }
  
