#ifndef __DBG_H__
#define __DBG_H__

#include "Telnet.h"

#define _DEBUG 
#ifdef _DEBUG
    #define Dbg(...) { \
		char printBuf[128] ; /* need temporary buffer */		\
		sprintf(printBuf, __VA_ARGS__) ;					\
		Serial.print(printBuf);								\
		telnet.Printf(printBuf);								\
}
#else    // No debug wanted
    #define Dbg(...)                                     
#endif

#endif //  __DBG_H__
