#ifndef _CONFIG_H
#define _CONFIG_H

#include <EEPROM.h>
#include <WString.h>
#include <MyTypes.h>


class Config
{
	protected:
	String	mSsid, mPwd ;
	String	mBoardName ;
	String	mOtaServer, mOtaPHP ;
	bool	mAutoUpdate ;
	String	mBroker,  mBrokerId ;
	uint16	mBrokerPort ;
	String	mMqttUser,  mMqttPwd ;
	uint16	mMaxSize ;
	uint16	mPortalTimeout ;
#define MAGICBYTES "CFG"
public:
	Config (uint16 maxSize = 1024) ;
	virtual ~Config () { }

	uint16		Size () ;
	bool		Save () ;

	bool		Load ()  ;

	void		Erase () ;
#define FIELD_STR(nom, field)						\
	String &	nom##Str() {return field ; }		\
	PCSTR		nom() {return field.c_str() ; }		\

	FIELD_STR(SSID,mSsid) ;
	FIELD_STR(Pwd,mPwd) ;
	FIELD_STR(BoardName,mBoardName) ;
	FIELD_STR(OTAServer,mOtaServer) ;
	FIELD_STR(OTAPhp,mOtaPHP) ;
	FIELD_STR(Broker,mBroker) ;
	FIELD_STR(BrokerId,mBrokerId) ;
	FIELD_STR(MqttUser,mMqttUser) ;
	FIELD_STR(MqttPwd,mMqttPwd) ;

	bool &		AutoUpdate() {return mAutoUpdate ; }
	uint16 &  	BrokerPort() {return mBrokerPort ; }
	uint16 &	PortalTimeout() {return mPortalTimeout ; }

	void		Dump () ;

protected:
	virtual uint16	ExtraSize ()  { return 0 ; }
	virtual void	ExtraSave ()  { ; }
	virtual void	ExtraLoad ()  { ; }

	uint16	SizeStr (String & s)  ;
	void	Load (String & s, int& idx) ;
	void	Load (bool & b, int& idx)  ;
	void	Load (uint16& w, int& idx) ;
	void	Load (char& c, int& idx) ;

	void	Save (bool b, int& idx) ;
	void	Save (char b, int& idx) ;
	void	Save (uint16 w, int& idx) ;
	void	Save (String & s, int& idx) ;


} ;

extern Config config ;
#endif // _CONFIG_H
