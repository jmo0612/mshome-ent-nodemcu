#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include "JMConnection.h"
#include "JMWifi.h"
#include "JMGlobal.h"
#include "JMFunctions.h"
#include "JMWifiWire.h"
#include "JMData.h"


JMWifi::JMWifi()
{
    this->http=new HTTPClient();
    this->client=new WiFiClient();
    this->wifi=new ESP8266WiFiClass();
};
void JMWifi::setReady(bool ready){
    this->ready=ready;
};
void JMWifi::setup(JMWifiWire *wifiWire)
{
    this->wifiWire=wifiWire;
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

    this->shutDownTimeStamp=atoi(this->httpGet2("/mshome-ent/g_sd_time.php"));
    //Serial.println(this->httpGet2("/mshome-ent/g_sd_time.php"));
    //Serial.println(this->shutDownTimeStamp);
    //Serial.println(WiFi.localIP()); // IP address assigned to your ESP
};

const char* JMWifi::httpGet2(const char* url, const uint8_t attempt){
    //Serial.println(attempt);
    if(attempt>5)return "";
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

void JMWifi::checkServer(){
    //Serial.println("CHECKING SERVER");
    if(!this->ready)return;
    if(this->shutDownTimeStamp>0){
        const uint32_t now=atoi(this->httpGet2("/mshome-ent/g_time.php"));
        if(now>this->shutDownTimeStamp){
            //shutdown
            uint64_t package=JMData::getMsgMultiplier(JMGlobal::PACKET_MSG_SPECIAL)+JMGlobal::MSG_SPECIAL_VALUE_SHUTDOWNALL;
            this->wifiWire->sendMessage2(JMData::msgToBytes(package),8);
            //Serial.println("SHUTTING DOWN");
            this->shutDownTimeStamp=0;
        }
    }

    //check queue
    const char* dequeued=this->httpGet2("/mshome-ent/g_myqueue.php?id=ARDUINO");
    char *ptr;  
    ptr = strtok((char*)dequeued, ",");  
    uint8_t i=0;
    uint32_t data[2];
    while (ptr != NULL)  
    {  
        data[i++]=atoi(ptr);
        if(i==2)break;
        ptr = strtok (NULL, ",");  
    }  
    if(i==2){
        this->ready=false;
        this->currentQueue=data[0];
        uint64_t package=JMData::getMsgMultiplier(JMGlobal::PACKET_MSG_DO_CMD)+data[1];
        this->wifiWire->sendMessage2(JMData::msgToBytes(package),8);
    }
};
