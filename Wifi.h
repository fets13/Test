
#include <ESP8266WiFi.h>

void InitWifi (const char * nom, const char * pwd)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(nom, pwd);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}
