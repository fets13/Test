#ifndef __DBG_H__
#define __DBG_H__

#include "Telnet.h"
#include "UdpDbg.h"

#define _DEBUG 
#ifdef _DEBUG
    #define Dbg(...) { \
		char printBuf[128] ; /* need temporary buffer */	\
		sprintf(printBuf, __VA_ARGS__) ;					\
		Serial.print(printBuf);								\
		telnet.Printf(printBuf);							\
		UdpDbgSend(printBuf);								\
}
    #define Err(...) { \
		Serial.print("E:");									\
		telnet.Printf("E:");								\
		Dbg(__VA_ARGS__) ;									\
}
#else    // No debug wanted
    #define Dbg(...)                                     
    #define Err(...)
#endif

#endif //  __DBG_H__
