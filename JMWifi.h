#ifndef JMWifi_h
#define JMWifi_h
//#include <WiFiClient.h>
//#include <ESP8266HTTPClient.h>
//#include "JMConnection.h"

class ESP8266WiFiClass;
class HTTPClient;
class WiFiClient;
class JMWifiWire;
class JMWifi
{
private:
    HTTPClient *http; // Declare object of class HTTPClient
    WiFiClient *client;
    ESP8266WiFiClass *wifi;
    JMWifiWire *wifiWire;
    bool ready=true;
    int currentQueue=0;
    
    uint32_t shutDownTimeStamp;
    //const char* httpGet2(const char* url, const uint8_t attempt=0);
    const char* httpGet(const char* url);

public:
    static const unsigned long wifiTimeOut = 5000;

    JMWifi();
    const char* httpGet2(const char* url, const uint8_t attempt=0);
    void setReady(bool ready);
    void setup(JMWifiWire *wifiWire);
    void checkServer();
    void saveDevicesData(uint64_t value);
    void dequeueCommand();
};
#endif
