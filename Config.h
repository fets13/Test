#ifndef _CONFIG_H
#define _CONFIG_H

#include <EEPROM.h>
#include <WString.h>


struct Config
{
	protected:
	String	mSsid, mPwd ;
	String	mBordName ;
	String	mOtaServer, mOtaPHP ;
	bool	mAutoUpdate ;
	String	mBroker,  mBrokerId ;
	uint16	mBrokerPort ;
	uint16	mMaxSize ;
#define MAGICBYTES "CFG"
public:
	Config (uint16 maxSize = 1024) ;
	virtual ~Config () { }

	uint16	Size () ;
	bool Save () ;

	bool Load ()  ;

protected:
	virtual uint16	ExtraSize ()  { return 0 ; }
	virtual void	ExtraSave ()  { ; }
	virtual void	ExtraLoad ()  { ; }

	uint16	SizeStr (String & s)  ;
	void	Load (String & s, int& idx) ;
	void	Load (bool b, int& idx)  ;
	void	Load (uint16 w, int& idx) ;

	void	Save (bool b, int& idx) ;
	void	Save (uint16 w, int& idx) ;
	void	Save (String & s, int& idx) ;


} ;

#endif // _CONFIG_H
