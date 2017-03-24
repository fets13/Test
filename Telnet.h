#ifndef __TELNET_H__
#define __TELNET_H__
#include <ESP8266WiFi.h>

class Telnet
{
public:
	Telnet () ;
	void Setup() ;

	bool Loop ()  ;
//FETS		Print& Printf() ; 

public:
	WiFiClient mClient;
protected:
	WiFiServer mServer;
} ;
#define Printf	mClient.printf
#define P	mClient.print
extern Telnet	telnet ;
#endif // __TELNET_H__
