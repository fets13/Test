
#include "EspMem.h"
#include "Dbg.h"
#include <user_interface.h>

ESPMem::ESPMem ()
{

	// No previous mem
	if (!ReadMem ()) {
		mBoardMode = BOOT_CONFIG ;	// mode normal
	}
	else mBoardMode = ReadBoardMode () ;

	mRtcMem.bootTimes++;
	WriteMem () ;
	Print () ;
}

ESPMem::~ESPMem ()
{
}

void ESPMem::Print ()
{
  Dbg ("\nESPMem\n ");
  Dbg ("\tmarkerFlag:%d\n ", mRtcMem.markerFlag);
  Dbg ("\tbootTimes:%d\n ", mRtcMem.bootTimes);
}

void ESPMem::WriteBoardMode (eBootMode mode)
{
  system_rtc_mem_read(OffsetMode(), &mode, 1);   // Read the "boardMode" flag RTC memory to decide, if to go to config
}

void ESPMem::InitWifi ()
{
//FETS			  WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed

	  if (WiFi.SSID() != "") {
		WiFi.mode(WIFI_STA); // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.
		unsigned long startedAt = millis();
		int connRes = WiFi.waitForConnectResult();
		uint32 waited = (millis() - startedAt);
		Dbg ("After waiting %d secs in setup() connection result is %d\n", waited / 1000, connRes);
	  } else {
		Dbg ("We haven't got any access point credentials, so get them now\n");
	  }

	  if (WiFi.status() != WL_CONNECTED) {
		Dbg ("Failed to connectn");
	  } else {
		Dbg("Local IP @: %s\n", WiFi.localIP().toString().c_str());
		Dbg("MAC @: %s\n", WiFi.macAddress().c_str());
	  }
}

void ESPMem::LoopWifiManager ()
{
}

bool ESPMem::ReadMem ()
{
	system_rtc_mem_read(RTCMEMBEGIN, &mRtcMem, sizeof(mRtcMem));
	if (mRtcMem.markerFlag != MAGICBYTE) {
		mRtcMem.bootTimes = 0;
		WriteMem ();
		return false ;
	}
	return true ;
}

void ESPMem::WriteMem()
{
  mRtcMem.markerFlag = MAGICBYTE;
  system_rtc_mem_write(RTCMEMBEGIN, &mRtcMem, sizeof(mRtcMem));
}

eBootMode ESPMem::ReadBoardMode ()
{
	eBootMode mode ;
  system_rtc_mem_read(OffsetMode(), &mode, 1);   // Read the "boardMode" flag RTC memory to decide, if to go to config
  return mode ;
}

int	ESPMem::OffsetMode ()
{
	return RTCMEMBEGIN + sizeof (mRtcMem) ;
}
