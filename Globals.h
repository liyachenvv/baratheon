
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

#include <htc.h>
#include "PicTypes.h"
#include "Hardware.h"

#endif
