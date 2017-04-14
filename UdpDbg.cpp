
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include "UdpDbg.h"
#include "Config.h"

WiFiUDP UDP;
#define UDP_PORT 51400
IPAddress broadcastIp(255, 255, 255, 255);

bool UdpDbgSend (PCSTR debugBuffer)
{
	if (WiFi.status() != WL_CONNECTED) {
		Serial.print ("DbgUdpSend FAILED : no WiFi\n") ;
		return false ;
	}
	if (UDP.begin(UDP_PORT) != 1) {
		Serial.print ("DbgUdpSend FAILED : begin\n") ;
		return false ;
	}
	int len = strlen (debugBuffer) ;
	if (len == 0) return false ;

	UDP.beginPacket(broadcastIp, UDP_PORT);


	// send board name first
	char s[32] ;
	sprintf (s, "%s : ", config.BoardName()) ;
	UDP.write(s, strlen(s));

	// suppres 'new line'
	if (debugBuffer[len-1] == '\n') len -= 1 ;

	UDP.write(debugBuffer, len);
	UDP.endPacket();
	UDP.stop();

	return true ;
}
