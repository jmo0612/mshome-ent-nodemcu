#ifndef JMWifi_h
#define JMWifi_h
//#include <WiFiClient.h>
//#include <ESP8266HTTPClient.h>
//#include "JMConnection.h"

class ESP8266WiFiClass;
class HTTPClient;
class WiFiClient;
class JMWifi
{
private:
    HTTPClient *http; // Declare object of class HTTPClient
    WiFiClient *client;
    ESP8266WiFiClass *wifi;

public:
    JMWifi();
    void setup();
    const char* httpGet2(const char* url, const int attempt=0);
    const char* httpGet(const char* url);
};
#endif
