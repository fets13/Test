
#include "EspMem.h"
#include "Dbg.h"

extern "C" {
#include "user_interface.h" // this is for the RTC memory read/write functions
}

#include "Config.h"
#include "MyWifi.h"

ESPMem::ESPMem ()
{
	Serial.begin (115200) ;
	// No previous mem
	if (!ReadMem ()) {
//FETS			mBoardMode = BOOT_CONFIG ;	// mode normal
	}
	else mBoardMode = ReadBoardMode () ;

	mRtcMem.bootTimes++;
	WriteMem () ;
	Print () ;

	if (!config.Load ()) {
		config.Save () ;
	}
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

bool ESPMem::WriteBoardMode (eBootMode mode)
{
  system_rtc_mem_write(OffsetMode(), &mode, 1);   // Write the "boardMode" flag RTC memory to decide, if to go to config
	eBootMode modeR ;
  system_rtc_mem_read(OffsetMode(), &modeR, 1);   // Read the "boardMode" flag RTC memory to decide, if to go to config
  if (mode != modeR) {
	  Err ("ESPMem::WriteBoardMode FAILED : %d != %d\n", mode, modeR) ;
	  return false ;
  }
  return true ;
}

int	ESPMem::OffsetMode ()
{
	return RTCMEMBEGIN + sizeof (mRtcMem) ;
}

void ESPMem::ManageBoot ()
{
Dbg ("ESPMem::ManageBoot boardMode:%d\n", mBoardMode) ;
	switch (mBoardMode) {


//FETS			case BOOT_CONFIG_TIMEOUT:
//FETS				break ;

		case BOOT_CONFIG:
			Dbg ("ESPMem::ManageBoot CONFIG\n") ;
			wifi.Config() ;
			break ;

		case BOOT_NORMAL:
			Dbg ("ESPMem::ManageBoot NORMAL\n") ;
			wifi.ConnectNetwork () ;
			if (WiFi.status() != WL_CONNECTED) {
				Dbg ("ESPMem::ManageBoot NORMAL FAILED\n") ;
				ESPMem::Restart (BOOT_CONFIG, "Wifi not connected") ;
			}
			if (config.AutoUpdate()) {
				// TODO WebOTA
			}
			break ;
		default:
				ESPMem::Restart (BOOT_CONFIG, "no Board mode") ;
			break ;
	}

}

void ESPMem::Restart (eBootMode mode, PCSTR msg)
{
	Dbg (msg) ;
	esp.WriteBoardMode (mode) ;
	delay(500);
	ESP.restart();
}
