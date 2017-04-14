#ifndef Mqtt_H
#define Mqtt_H

#include <PubSubClient2.h>
#include <ESP8266WiFi.h>
#include <Telnet.h>
#include <MyTypes.h>

#include <Ticker.h>


class MqttLow 
{
public:
	MqttLow (PCSTR topic, WiFiClient & client) ;
	MqttLow (WiFiClient & client) ;
	void			SetTopics (PCSTR topic) ;
	virtual ~MqttLow () {}
	void			SetIdent (PCSTR user, PCSTR pwd) ;
	void			SetServer (PCSTR server, uint16_t port) ;
	virtual void		Setup () ;
	virtual void		SetupBegin () {}
	virtual void		SetupEnd () {}
	virtual void		Loop () ;
	int					RSSI () ;
	bool				Publish (String & topic, float f, int precision=2, bool retain=false) ;
	bool				Publish (String & topic, int payload, bool retain=false) ;
	bool				Publish (String & topic, PCSTR payload, bool retain=false) ;
	bool				PublishSub (PCSTR subTopic, float f, int precision=2, bool retain=false) ;
	bool				PublishSub (PCSTR topic, int payload, bool retain=false) ;
	bool				PublishSub (PCSTR topic, PCSTR payload, bool retain=false) ;
protected:
	virtual void	ActionLoop () {}
	void		Reconnect () ;
//FETS		virtual void	Callback(const MQTT::Publish&) {}
	virtual void	Callback (const String & topic, const String & payload) ;
	virtual void	CallbackDefault (const String & topic, const String & payload) {}
	String		Mac2Str (const uint8_t * mac) ;
	virtual bool	Subscribe () ;
	bool		Subscribe (String topic) ;
	bool		SubscribeAll (String topic) ;
	virtual void	SetCbk () = 0 ;
	String &	TopicOut () { return mTopicOut ;}
	String &	TopicIn () { return mTopicIn ;}
	virtual void		ManageReboot () ;
	virtual void		ManageRssi () ;
	virtual void		ManageFlash () {} ;
	virtual void		ManageState ()  ;
	virtual void		ManageConfig ()  ;
	virtual void		ManageStateExtra () {} ;
	virtual void		ManageHelp ()  ;
	virtual void		ManageHelpExtra () {} ;
// FETS	static void	RssiCbk (MqttLow *) ;
protected:
	String		mSsid ;
	String		mUser, mPwd ;
	long		mTime ;
	PubSubClient2	mMqtt ;
	int		mRssi ;
	String		mTopicIn, mTopicOut ;
	String		mWillTopic, mWillMsg ;
//FETS		Ticker			mTickRssi ;
} ;

class Mqtt : public MqttLow 
{
public:
	Mqtt (PCSTR topic = NULL) ;
	virtual ~Mqtt () {}
protected:
	virtual void	SetCbk () ;
	static void		Cbk(const MQTT::Publish&) ;
protected:
	static Mqtt * 	mpThis ;
	WiFiClient		mWifiClient;
} ;

class MqttS : public MqttLow 
{
public:
	MqttS (PCSTR topic, PCSTR fingerPrint) ;
	MqttS () ;
	void SetFingerPrint (PCSTR fingerPrint) { mFingerPrint = fingerPrint ; }
	virtual ~MqttS () {}
	virtual void		Setup () ;
protected:
	virtual void	SetCbk () ;
	static void		Cbk(const MQTT::Publish&) ;
	bool			VerfiyTls () ;
protected:
	static MqttS * 	mpThis ;
	WiFiClientSecure		mWifiClient;
	String					mFingerPrint ;
} ;



#endif //  Mqtt_H
