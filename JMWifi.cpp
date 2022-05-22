#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "JMConnection.h"
#include "JMWifi.h"
#include "JMFunctions.h"
#include "JMGlobal.h"


JMWifi::JMWifi()
{
    this->http=new HTTPClient();
    this->client=new WiFiClient();
    this->wifi=new ESP8266WiFiClass();
};
void JMWifi::setup()
{
    // put your setup code here, to run once:
    this->wifi->mode(WIFI_OFF); // Prevents reconnection issue (taking too long to connect)
    delay(1000);
    this->wifi->mode(WIFI_STA); // This line hides the viewing of ESP as wifi hotspot

    this->wifi->begin(JMConnection::ssid(), JMConnection::ssid_pass());
    while (this->wifi->status() != WL_CONNECTED)
    {
        delay(1000);
    }
    Serial.println(this->wifi->localIP());

    //Serial.println(WiFi.localIP()); // IP address assigned to your ESP
};

const char* JMWifi::httpGet2(const char* url, const int attempt){
    Serial.println(attempt);
    if(attempt>5)return "FAILED";
    this->http->begin(*this->client, JMConnection::host(), JMConnection::host_port(), url, JMConnection::isHttps());
    this->http->addHeader("Content-Type", "application/x-www-form-urlencoded");

    const int httpCode = this->http->GET();         // Send the request
    const String payload = this->http->getString(); // Get the response payload
    //delay(1000);

    
    if(httpCode!=200){
        this->http->end(); // Close connection
        return httpGet2(url,attempt+1);
        //return "loop";
    }

    const char *ret=JMFunctions::strToCharP(payload);
    //char *ret="tolor";
    this->http->end(); // Close connection
    //Serial.println(payload);
    return ret;
    //return "KEODE";
};
const char* JMWifi::httpGet(const char* url){
    Serial.println("second");
    this->http->setTimeout(JMGlobal::wifiTimeOut);
    this->http->begin(*this->client, JMConnection::host(), JMConnection::host_port(), url, JMConnection::isHttps());
    this->http->addHeader("Content-Type", "application/x-www-form-urlencoded");

    const int httpCode = this->http->GET();         // Send the request
    const String payload = this->http->getString(); // Get the response payload
    //delay(1000);

    
    if(httpCode!=200){
        this->http->end(); // Close connection
        return "FAILED";
        //return "loop";
    }

    const char *ret=JMFunctions::strToCharP(payload);
    //char *ret="tolor";
    this->http->end(); // Close connection
    //Serial.println(payload);
    return ret;
    //return "KEODE";
};