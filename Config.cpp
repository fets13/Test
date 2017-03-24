
#include "Config.h"
#include "Dbg.h"

Config::Config (uint16 maxSize) : mMaxSize(maxSize)
{
}
	
	
uint16 Config::Size ()
{
	uint16 sz = 0 ;
	sz += SizeStr (mSsid) ;
	sz += SizeStr (mPwd) ;
	sz += SizeStr (mOtaServer) ;
	sz += SizeStr (mOtaPHP) ;
	sz += sizeof (mAutoUpdate) ;
	sz += SizeStr (mBroker) ;
	sz += sizeof (mBrokerPort) ;
	sz += SizeStr (mBrokerId) ;
	sz += ExtraSize () ;
	sz += 3 ; // magic bytes
	return sz ;
}

bool Config::Save ()
{
	uint16 size = Size () ;
	if (size > mMaxSize) {
		Dbg ("Config::Save FAILED : %d > %d\n", size, mMaxSize) ;
		return false ;
	}
	int idx = 0 ;
	EEPROM.begin(mMaxSize);
	Save (size, idx) ;
	Save (mSsid, idx) ;
	Save (mPwd, idx) ;
	Save (mOtaServer, idx) ;
	Save (mOtaPHP, idx) ;
	Save (mAutoUpdate, idx) ;
	Save (mBroker, idx) ;
	Save (mBrokerPort, idx) ;
	Save (mBrokerId, idx) ;
	ExtraSave () ;
	Save ("C", idx) ;
	Save ("F", idx) ;
	Save ("G", idx) ;
	return true ;
}

bool Config::Load ()
{
	uint16 size = 0 ;
	int idx = 0 ;
	EEPROM.begin(mMaxSize);
	Load (size, idx) ;
	// check for magic bytes
   int	magicBegin = size - 4; // Magic bytes at the end of the structure
	if (EEPROM.read(magicBegin) != MAGICBYTES[0] 
			|| EEPROM.read(magicBegin + 1) != MAGICBYTES[1] 
			|| EEPROM.read(magicBegin + 2) == MAGICBYTES[2]) {
		Dbg ("Config::Load FAILED : magic bytes '%c%c%c'\n", (char)EEPROM.read(magicBegin)
				, (char)EEPROM.read(magicBegin+1), EEPROM.read(magicBegin+2)) ;
		return false ;
	}
	Load (mSsid, idx) ;
	Load (mPwd, idx) ;
	Load (mOtaServer, idx) ;
	Load (mOtaPHP, idx) ;
	Load (mAutoUpdate, idx) ;
	Load (mBroker, idx) ;
	Load (mBrokerPort, idx) ;
	Load (mBrokerId, idx) ;
	ExtraLoad () ;
	return true ;
}

void Config::Load (String & s, int& idx)
{
	uint8 sz = EEPROM.get (idx, sz) ;
	s.reserve (sz+1) ;
	EEPROM.write(idx++, sz); sz += sizeof (uint8) ;
	uint8 i ;
	for (i = 0; i < sz; i++) s[i] = EEPROM.read(idx+i);
	s[i]= '\0' ;
	idx += sz ;
}

void Config::Load (bool b, int& idx)
{
	EEPROM.get(idx, b);
	idx += 1 ;
}

void Config::Load (uint16 w, int& idx)
{
	EEPROM.get(idx, w);
	idx += sizeof (uint16) ;
}

uint16 Config::SizeStr (String & s)
{
	return  (1 + s.length()) ;
}

void Config::Save (bool b, int& idx)
{
	EEPROM.write(idx++, b);
}

void Config::Save (uint16 w, int& idx)
{
//FETS			uint8 * p = (uint8 *)&w ;
//FETS			EEPROM.write(idx++, p[0]);
//FETS			EEPROM.write(idx++, p[1]);
	EEPROM.put(idx, w);
	idx += sizeof (uint16) ;
}
	
void Config::Save (String & s, int& idx)
{
	uint8 sz = s.length() ;
	EEPROM.write(idx++, sz);
	for (uint8 i = 0; i < sz; i++) EEPROM.write(idx++, s[i]);
}
