

#include "Web-OTA.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <Dbg.h>


WebOTA::WebOTA ()
{
}
WebOTA::WebOTA (String server, String uri, String firm_version)
				: mServer(server), mUri(uri), mFirmVersion(firm_version)
{
	Init (server, uri, firm_version) ;
}

void WebOTA::Init (String server, String uri, String firm_version)
{
	mServer = server ;
	mUri = uri ;
	mFirmVersion = firm_version ;
}
void WebOTA::Setup ()
{
	Action (true) ;
}

void WebOTA::Action (bool debug) {
	if (mServer == "") {
		Dbg ("WebOTA::Action FAILED: no server !!!\n") ;
		return ;
	}
  if (debug) {
	Dbg("MAC @: %s\n", WiFi.macAddress().c_str());
    Dbg ("start flashing......\n") ;
    Dbg ("\t%s/%s/%s", mServer.c_str(), mUri.c_str(), mFirmVersion.c_str());
  }

  t_httpUpdate_return ret = ESPhttpUpdate.update(mServer, 80, mUri, mFirmVersion);
//FETS	  t_httpUpdate_return ret = ESPhttpUpdate.update(mServer, 80, "/esp-test.bin");
  switch (ret) {
    case HTTP_UPDATE_FAILED:
      if (debug) Dbg ("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      if (debug) Dbg ("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      if (debug) Dbg ("HTTP_UPDATE_OK");
      break;
  }
}

