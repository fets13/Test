
//FETS	#include <ESP8266WiFi.h>
//FETS	#include <ESP8266mDNS.h>
//FETS	#include <WiFiUdp.h>
//FETS	#include <WiFiServer.h>
//FETS	#include <ArduinoOTA.h>

#include "Telnet.h"



Telnet::Telnet () : mServer(23)
{
}

void Telnet::Setup()
{
	mServer.begin() ;
	mServer.setNoDelay (true) ;
}

bool Telnet::Loop () 
{
	if (!mServer.hasClient()) {
		return false ;
	}
	  Serial.println("Telnet.HasClient");


	  bool ret = false ;
	
	// client is connected
	if (mClient || !mClient.connected()) {
		if (mClient) mClient.stop() ; // client disconnected
		mClient = mServer.available() ; // ready for new client
		if (!mClient) Serial.println("Telnet. Not Client");
	  	else {
  			ret = true ;
			Serial.println("Telnet. Client");
			Printf("MAC @: %s\n", WiFi.macAddress().c_str());
			Printf("IP @: %s\n", WiFi.localIP().toString().c_str());
		}
	}
	else {
		mServer.available().stop() ; // have client
		  Serial.println("Telnet. available.stop");
	}

	return ret ;
}

//FETS	Print& Telnet::Printf()
//FETS	{
//FETS		return mClient ;
//FETS	} 
//FETS		size_t print(const __FlashStringHelper *);
//FETS		size_t print(const String &);
//FETS	        size_t print(const char[]);
