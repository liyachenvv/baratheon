
///////////////////////////////////////////////////////////
//                                                       //
//          FileName:   Globals.h                        //
//                                                       //
///////////////////////////////////////////////////////////

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#ifdef EXTERN
#undef EXTERN
#endif
#ifdef _MAIN_SRC_
#define EXTERN
#else
#define EXTERN extern
#endif

#define VER_110VAC  0
#define VER_220VAC  1

#if  ( !VER_110VAC && !VER_220VAC ) || ( VER_110VAC && VER_220VAC )
#error  One and only one of "VER_110VAC & VER_220VAC" should be set as "1"
#endif

#include <htc.h>
#include "PicTypes.h"
#include "Hardware.h"

#endif
