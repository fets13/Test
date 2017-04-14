#include "Mqtt.h"
#include "Dbg.h"
#include "EspMem.h"


Mqtt * Mqtt::mpThis = NULL ;
MqttS * MqttS::mpThis = NULL ;

MqttLow::MqttLow (WiFiClient & client) : mMqtt (client)
{
	mRssi = -1 ;
}

MqttLow::MqttLow (const char * topic, WiFiClient & client) : mMqtt (client)
{
	mRssi = -1 ;
	SetTopics (topic) ;
}

void MqttLow::SetIdent (PCSTR user, PCSTR pwd)
{
	mUser = user ;
	mPwd = pwd ;
}
void MqttLow::SetTopics (PCSTR topic)
{
	mTopicIn = topic ;
	mTopicIn += "/in" ;
	mTopicOut = topic ;
	mTopicOut += "/out" ;
	mWillTopic = mTopicOut + "/state" ;
	mWillMsg  = "dead" ;
}

void MqttLow::SetServer (PCSTR server, uint16_t port)
{
	mMqtt.set_server (server, port) ;
}


void MqttLow::Setup ()
{
	SetupBegin () ;
//FETS		mSsid = ssid ;
//FETS		Serial.print("setupIP:"); Serial.println(myIP);
//FETS		Serial.print("gw:"); Serial.println(gw);
//FETS		Serial.print("subnet:"); Serial.println(subnet);
//FETS		WiFi.config(myIP, gw, subnet, dns1, dns2);
//FETS		WiFi.config(myIP, gw, subnet);
	SetupEnd () ;
	mTime = millis();
//FETS		mTickRssi.attach (60, RssiCbk, this) ;
}



void MqttLow::Loop ()
{
	if (WiFi.status() != WL_CONNECTED) {
		Dbg ("MqttLow::Reconnect : WIFI not connected !!!\n") ;
		WiFi.begin();
		if (WiFi.waitForConnectResult() != WL_CONNECTED) {
			Dbg ("MqttLow::Reconnect : FAILED !!!\n") ;
			return;
		}
	}
	//reconnect if connection is lost
	if (!mMqtt.connected() && WiFi.status() == WL_CONNECTED) {
		Reconnect();
	}

	//maintain MqttLow connection
    if (mMqtt.connected()) {
		mMqtt.loop();
	}
//FETS		else {
//FETS			Serial.println ("MqttLow.loop FAILED") ;
//FETS		}

	ActionLoop () ;
	
	//MUST delay to allow ESP8266 WIFI functions to run
	delay(10); 
}
//networking functions

void MqttLow::Reconnect()
{
	bool flgReconn = false ;
	if (WiFi.status() != WL_CONNECTED) {
		Dbg ("MqttLow::Reconnect : WIFI not connected !!!\n") ;
//FETS			return ;
#if 1
		WiFi.begin();
		if (WiFi.waitForConnectResult() != WL_CONNECTED) {
			Dbg ("MqttLow::Reconnect : FAILED !!!\n") ;
			return;
		}
		flgReconn = true ;
#else
		Serial.print("Connecting to ");
		Serial.print(ssid);
		Serial.println("...");
		WiFi.begin(ssid, password);

		if (WiFi.waitForConnectResult() != WL_CONNECTED) return;

		Serial.println("\nWiFi connected");  
		Serial.print("IP:"); Serial.println(WiFi.localIP());
		Serial.print("GW IP:"); Serial.println(WiFi.gatewayIP());
		Serial.print("DNS IP:"); Serial.println(WiFi.dnsIP());
		Serial.print("mask"); Serial.println(WiFi.subnetMask());
		Serial.print("mac@"); Serial.println(WiFi.macAddress().c_str());
#endif
	}

	if (WiFi.status() == WL_CONNECTED  && !mMqtt.connected()) {

  		long diff = (millis() - mTime) / 1000 ;
//FETS			Dbg("diffTime:%d s\n", diff);
		mTime = millis();

		// Generate client name based on MAC address and last 8 bits of microsecond counter
		String clientName;
		clientName += "esp8266-";
		uint8_t mac[6];
		WiFi.macAddress(mac);
		clientName += Mac2Str(mac);
		//if (mMqtt.connect((char*) clientName.c_str())) 
		MQTT::Connect conn (clientName) ;
		conn.set_keepalive(60) ;
		conn.set_will (mWillTopic, mWillMsg, 0, true) ;

		if (mUser != "") {
//FETS				ret = mMqtt.connect(MQTT::Connect(clientName).set_auth (mUser, mPwd)
//FETS						.set_keepalive(60)) ;
			conn.set_auth (mUser, mPwd) ;
//FETS				Dbg ("connect(%s, %s, %s) \n", clientName.c_str(), mUser.c_str(), mPwd.c_str()) ;
		}
		else {
//FETS				Dbg ("connect(%s)\n", clientName.c_str()) ;
//		ret = mMqtt.connect(MQTT::Connect((char*) clientName.c_str())) ;
//FETS				ret = mMqtt.connect((char*) clientName.c_str()) ;
		}
		if (mMqtt.connect(conn)) {
			Dbg ("Mqtt connected (%s) \n", clientName.c_str()) ;
			SetCbk () ;
			Subscribe () ;
			if (flgReconn) {
				PublishSub("ip", WiFi.localIP().toString().c_str());
			}
			Publish(mWillTopic, "alive", true) ;
		}
		else {
			delay (1000) ;
		}
	}
} // void MqttLow::Reconnect()

bool MqttLow::Subscribe ()
{
	return SubscribeAll (mTopicIn) ;
}

bool MqttLow::Subscribe (String topic)
{
	Dbg("\tMqtt Connected, topic:<%s>\n", topic.c_str());
	if (!mMqtt.subscribe(topic.c_str())) {
		Dbg ("subscribe FAILED\n") ;
		return false ;
	}
	return true ;
}

bool MqttLow::SubscribeAll (String topic)
{
	String t(topic);
	t += "/#" ;
	return Subscribe (t) ;
}

//generate unique name from MAC addr
String MqttLow::Mac2Str(const uint8_t* mac){

  String result;

  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);

    if (i < 5){
      result += ':';
    }
  }

  return result;
}
int MqttLow::RSSI()
{
	int32_t rssi = WiFi.RSSI();
    unsigned char quality;
    if (rssi <= -100) {
      quality = 0;
    } else if (rssi >= -50) {
      quality = 100;
    } else {
      quality = 2 * (rssi + 100);
    }
	return quality ;
}

bool MqttLow::Publish (String & topic, float val, int precision, bool retain)
{
	MQTT::Publish pub(topic, String (val, precision));
	pub.set_retain(retain);
	return mMqtt.publish(pub);
}

bool MqttLow::Publish (String & topic, int val, bool retain)
{
	MQTT::Publish pub(topic, String (val));
	pub.set_retain(retain);
	return mMqtt.publish(pub);
}

bool MqttLow::Publish (String & topic, PCSTR payload, bool retain)
{
	MQTT::Publish pub(topic, String (payload));
	pub.set_retain(retain);
	return mMqtt.publish(pub);
}


bool MqttLow::PublishSub (PCSTR subTopic, float val, int precision, bool retain)
{
	String topic = mTopicOut + "/" + subTopic ;
	return Publish(topic, val, precision, retain);
}

bool MqttLow::PublishSub (PCSTR subTopic, int val, bool retain)
{
	String topic = mTopicOut + "/" + subTopic ;
	return Publish(topic, val, retain);
}

bool MqttLow::PublishSub (PCSTR subTopic, PCSTR val, bool retain)
{
	String topic = mTopicOut + "/" + subTopic ;
	return Publish(topic, val, retain);
}

void MqttLow::ManageRssi ()
{
	int rssi = RSSI () ;
	PublishSub("rssi", rssi);
}
void MqttLow::ManageReboot ()
{
	Dbg("REBOOT\n");
	ESP.restart();
}

void MqttLow::ManageState ()
{
	Dbg ("\n");
	Dbg("\tIP @     : %s\n", WiFi.localIP().toString().c_str());
	Dbg("\tMAC @    : %s\n", WiFi.macAddress().c_str());
	Dbg("\tRSSI     : %d%%\n", RSSI());
	ManageStateExtra () ;
}

void MqttLow::ManageConfig ()
{
	ESPMem::Restart (BOOT_CONFIG, "Config asked (mqtt)") ;
}

void MqttLow::ManageHelp ()
{
	Dbg ("\n");
	Dbg ("\t %s : reboot|flash|state\n", mTopicIn.c_str());
	ManageHelpExtra () ;
}
//FETS	void MqttLow::RssiCbk (MqttLow *mqtt)
//FETS	{
//FETS		mqtt->mFlgRssi = true ;
//FETS	}


void MqttLow::Callback(const String & topic, const String & payload)
{
    if (mTopicIn != topic) CallbackDefault (topic, payload) ;
    
    if (payload == "reboot") {
		ManageReboot () ;
    }
    else if (payload == "rssi") {
	    ManageRssi () ;
    }
	else if (payload == "flash") {
		ManageFlash () ;
	}
	else if (payload == "state") {
		ManageState () ;
	}
	else if (payload == "config") {
		ManageConfig () ;
	}
	else if (payload == "help") {
		ManageHelp () ;
	}
	else {
		CallbackDefault (topic, payload) ;
	}

    
}











Mqtt::Mqtt (PCSTR topic) : 
	MqttLow (topic, mWifiClient) 
{
	mpThis = this ;
}

void Mqtt::SetCbk ()
{
	mMqtt.set_callback (Mqtt::Cbk) ;
}

void Mqtt::Cbk(const MQTT::Publish& pub)
{
//FETS		mpThis->Callback(pub) ;
	mpThis->Callback(pub.topic(), pub.payload_string()) ;
}


MqttS::MqttS () : MqttLow (mWifiClient) 
{
	mpThis = this ;
}


MqttS::MqttS (const char * topic, const char * fingerprint) : 
	MqttLow (topic, mWifiClient) 
	, mFingerPrint(fingerprint)
{
	mpThis = this ;
}

void MqttS::SetCbk ()
{
	mMqtt.set_callback (MqttS::Cbk) ;
}

void MqttS::Cbk(const MQTT::Publish& pub)
{
//FETS		mpThis->Callback(pub) ;
	mpThis->Callback(pub.topic(), pub.payload_string()) ;
}

void MqttS::Setup ()
{
	MqttLow::Setup () ;
	VerfiyTls () ;
	
}

bool MqttS::VerfiyTls ()
{
	if (mFingerPrint.length() == 0) {
		Dbg ("MqttS::VerfiyTls FAILED : fingerprint\n");
		return false ;
	}

	if (!mWifiClient.connect(mMqtt.ServerName().c_str(), mMqtt.ServerPort())) {
		Dbg ("MqttS::VerfiyTls.connect FAILED\n");
		return false ;
	}

	if (mWifiClient.verify(mFingerPrint.c_str(), mMqtt.ServerName().c_str())) {
		Dbg ("MqttS::VerfiyTls.verify FAILED\n");
		return false ;
	}
	Dbg("certificate matches\n") ;

	return true ;
}
