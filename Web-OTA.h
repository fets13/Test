
#ifndef __WEB_OTA_H__
#define __WEB_OTA_H__
#include <WString.h>

class WebOTA
{
public:
	WebOTA ()  ;
	WebOTA (String server, String uri, String firm_version)  ;
	void Init (String server, String uri, String firm_version)  ;
	void Setup () ;
	void Action (bool) ;
public:
	String	mServer ;
	String	mUri ;
	String	mFirmVersion ;  
};

#endif // __WEB_OTA_H__
