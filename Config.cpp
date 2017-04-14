
#include "Config.h"
#include "Dbg.h"


Config::Config (uint16 maxSize) : mMaxSize(maxSize)
{
	mPortalTimeout = 600 ;
}
	
	
uint16 Config::Size ()
{
	uint16 sz = 0 ;
	sz += sizeof (uint16) ;
	sz += SizeStr (mSsid) ;
	sz += SizeStr (mPwd) ;
	sz += SizeStr (mBoardName) ;
	sz += sizeof (mPortalTimeout) ;
	sz += SizeStr (mOtaServer) ;
	sz += SizeStr (mOtaPHP) ;
	sz += sizeof (mAutoUpdate) ;
	sz += SizeStr (mBroker) ;
	sz += sizeof (mBrokerPort) ;
	sz += SizeStr (mBrokerId) ;
	sz += SizeStr (mMqttUser) ;
	sz += SizeStr (mMqttPwd) ;
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
	Dbg ("Config::Save : size:%d mMaxSize:%d\n", size, mMaxSize) ;
	int idx = 0 ;
#undef DStr
#undef DInt
#undef DCh
//FETS	#define DStr(ch)	Dbg ("Config::Save : " #ch "<%s> len:%d  idx:%d\n", ch.c_str(), ch.length(), idx) ;
//FETS	#define DCh(ch)	Dbg ("Config::Save : " #ch "<%c> idx:%d\n", ch, idx) ;
//FETS	#define DInt(ch)	Dbg ("Config::Save : " #ch ":%d   idx:%d\n", ch, idx) ;
#define DStr(ch)
#define DCh(ch)
#define DInt(ch)
	EEPROM.begin(mMaxSize);
	Save (size, idx) ;
DInt(size)
	Save (mSsid, idx) ;
DStr(mSsid)
	Save (mPwd, idx) ;
DStr(mPwd)
	Save (mBoardName, idx) ;
DStr(mBoardName)
	Save (mPortalTimeout, idx) ;
DInt(mPortalTimeout)
	Save (mOtaServer, idx) ;
DStr(mOtaServer)
	Save (mOtaPHP, idx) ;
DStr(mOtaPHP)
	Save (mAutoUpdate, idx) ;
DInt(mAutoUpdate)
	Save (mBroker, idx) ;
DStr(mBroker)
	Save (mBrokerPort, idx) ;
DInt(mBrokerPort)
	Save (mBrokerId, idx) ;
DStr(mBrokerId)
	Save (mMqttUser, idx) ;
DStr(mMqttUser)
	Save (mMqttPwd, idx) ;
DStr(mMqttPwd)
	ExtraSave () ;
	Save (MAGICBYTES[0], idx) ;
DCh(MAGICBYTES[0])
	Save (MAGICBYTES[1], idx) ;
DCh(MAGICBYTES[1])
	Save (MAGICBYTES[2], idx) ;
DCh(MAGICBYTES[2])
	EEPROM.end();
	return true ;
} // bool Config::Save ()

void Config::Erase ()
{
	Dbg ("Erasing Flash...\n");
	EEPROM.begin(mMaxSize);
	for (unsigned int t = 0; t < mMaxSize; t++) EEPROM.write(t, 0);
	EEPROM.end();
}

#undef DStr
#undef DInt
#undef DCh
#define DStr(ch)	Dbg ("Config::Load : " #ch "<%s> len:%d  idx:%d\n", ch.c_str(), ch.length(), idx) ;
#define DCh(ch)	Dbg ("Config::Load : " #ch "<%c> idx:%d\n", ch, idx) ;
#define DInt(ch)	Dbg ("Config::Load : " #ch ":%d   idx:%d\n", ch, idx) ;
bool Config::Load ()
{
	Dbg ("Config::Load \n") ;
	uint16 size = 0 ;
	int idx = 0 ;
	EEPROM.begin(mMaxSize);
	Load (size, idx) ;
//FETS	DInt(size) ;
	// check for magic bytes
   int	magicBegin = size - 3; // Magic bytes at the end of the structure
	if (EEPROM.read(magicBegin) != MAGICBYTES[0] 
			|| EEPROM.read(magicBegin + 1) != MAGICBYTES[1] 
			|| EEPROM.read(magicBegin + 2) != MAGICBYTES[2]) {
		Dbg ("Config::Load FAILED : magic bytes '%c%c%c'\n", (char)EEPROM.read(magicBegin)
				, (char)EEPROM.read(magicBegin+1), EEPROM.read(magicBegin+2)) ;
		return false ;
	}
	Load (mSsid, idx) ;
//FETS	DStr(mSsid)
	Load (mPwd, idx) ;
//FETS	DStr(mPwd)
	Load (mBoardName, idx) ;
//FETS	DStr(mBoardName)
	Load (mPortalTimeout, idx) ;
//FETS	DInt(mPortalTimeout)
	Load (mOtaServer, idx) ;
//FETS	DStr(mOtaServer)
	Load (mOtaPHP, idx) ;
//FETS	DStr(mOtaPHP)
	Load (mAutoUpdate, idx) ;
//FETS	DInt(mAutoUpdate)
	Load (mBroker, idx) ;
//FETS	DStr(mBroker)
	Load (mBrokerPort, idx) ;
//FETS	DInt(mBrokerPort)
	Load (mBrokerId, idx) ;
//FETS	DStr(mBrokerId)
	Load (mMqttUser, idx) ;
//FETS	DStr(mMqttUser)
	Load (mMqttPwd, idx) ;
//FETS	DStr(mMqttPwd)
	ExtraLoad () ;
  
	EEPROM.end();
	//
	//modif BrokerID
	//
	for (int i = 0; i < mBrokerId.length(); i++) {
		if (mBrokerId[i] == ':') mBrokerId[i] = ' ';
	}
	
//FETS		Dump () ;
	return true ;
} // bool Config::Load ()

void Config::Load (String & s, int& idx)
{
	uint8 sz = EEPROM.get (idx++, sz) ;
	s="";
	s.reserve (sz+1) ;
	uint8 i ;
	for (i = 0; i < sz; i++) {
		char c = EEPROM.read(idx+i);
		s += c ;
	}
	s += '\0' ;
	idx += sz ;
}

void Config::Load (bool& b, int& idx)
{
	EEPROM.get(idx, b);
	idx += 1 ;
}

void Config::Load (char& c, int& idx)
{
	EEPROM.get(idx, c);
	idx += 1 ;
}

void Config::Load (uint16& w, int& idx)
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

void Config::Save (char c, int& idx)
{
	EEPROM.write(idx++, c);
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

void Config::Dump()
{
	Dbg ("Config::Dump : \n") ;
#define Dstr(n) Dbg ("\t"#n":%s\n", n.c_str()) ;
#define Dint(n) Dbg ("\t"#n":%d\n", n) ;
	Dstr (mSsid) ;
	Dstr (mPwd) ;
	Dstr (mBoardName) ;
	Dint (mPortalTimeout) ;
	Dstr (mOtaServer) ;
	Dstr (mOtaPHP) ;
	Dint (mAutoUpdate) ;
	Dstr (mBroker) ;
	Dint (mBrokerPort) ;
	Dstr (mBrokerId) ;
	Dstr (mMqttUser) ;
	Dstr (mMqttPwd) ;
}
