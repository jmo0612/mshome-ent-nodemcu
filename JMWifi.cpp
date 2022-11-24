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
    // Serial.println(this->wifi->localIP());
    

    while(this->shutDownTimeStamp==0){
        //Serial.println(F("looping"));
        const char* shutStr=this->httpGet2("/mshome-ent/g_sd_time.php");
        if(shutStr!=""){
            this->shutDownTimeStamp=atoi(shutStr);    
            if(this->shutDownTimeStamp>0)break;
        }
        delay(10000);
    }

    //uint64_t package=JMData::devInitToInt64(this->httpGet2("/mshome-ent/g_dev.php"));
    //this->wifiWire->sendMessage2(JMData::msgToBytes(package),8);

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
    // Serial.println("second");
    this->http->setTimeout(JMWifi::wifiTimeOut);
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
    // Serial.println("CHECKING SERVER");
    if(!this->ready)return;
    if(this->shutDownTimeStamp>0){
        const uint32_t now=atoi(this->httpGet2("/mshome-ent/g_time.php"));
        if(now>this->shutDownTimeStamp){
            //shutdown
            uint64_t package=JMData::getMsgMultiplier(JMGlobal::PACKET_MSG_SPECIAL)+JMGlobal::MSG_SPECIAL_VALUE_SHUTDOWNALL;
            this->wifiWire->sendMessage2(JMData::msgToBytes(package),8);
            // Serial.println("SHUTTING DOWN");
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
        this->currentQueue=data[0];
        uint64_t package=JMData::getMsgMultiplier(JMGlobal::PACKET_MSG_DO_CMD)+data[1];
        if(this->currentQueue>0){
            // Serial.println(F("send"));
            this->wifiWire->sendMessage2(JMData::msgToBytes(package),8);
            this->ready=false;
        }
    }
};

void JMWifi::saveDevicesData(uint64_t value){
    //     /mshome-ent/p_dev.php?sta=10000000000000000000000000000001
    static char dev[58]={'/','m','s','h','o','m','e','-','e','n','t','/','p','_','d','e','v','.','p','h','p','?','s','t','a','=','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    dev[0]='/';
    dev[1]='m';
    dev[2]='s';
    dev[3]='h';
    dev[4]='o';
    dev[5]='m';
    dev[6]='e';
    dev[7]='-';
    dev[8]='e';
    dev[9]='n';
    dev[10]='t';
    dev[11]='/';
    dev[12]='p';
    dev[13]='_';
    dev[14]='d';
    dev[15]='e';
    dev[16]='v';
    dev[17]='.';
    dev[18]='p';
    dev[19]='h';
    dev[20]='p';
    dev[21]='?';
    dev[22]='s';
    dev[23]='t';
    dev[24]='a';
    dev[25]='=';
    uint8_t indStart = 26;//start index of 10000000000000000000000000000001 (cth)
    uint8_t ind = 57;
    uint64_t quotient = value;
    int remain;

    do
    {
        remain = quotient % 3;
        char buffer [1];
        itoa(remain,buffer,10);
        dev[ind--] = buffer[0];
        quotient = quotient / 3;
    } while (quotient > 0);
    for(uint8_t i=indStart;i<=ind;i++){
        dev[i]='0';
    }
    this->httpGet2(dev);
    // Serial.println(dev);
};
void JMWifi::dequeueCommand(){
    
    //     /mshome-ent/p_dequeue.php?id=2147483647
    static char dev[39]={'/','m','s','h','o','m','e','-','e','n','t','/','p','_','d','e','q','u','e','u','e','.','p','h','p','?','i','d','=','0','0','0','0','0','0','0','0','0','0'};
    dev[0]='/';
    dev[1]='m';
    dev[2]='s';
    dev[3]='h';
    dev[4]='o';
    dev[5]='m';
    dev[6]='e';
    dev[7]='-';
    dev[8]='e';
    dev[9]='n';
    dev[10]='t';
    dev[11]='/';
    dev[12]='p';
    dev[13]='_';
    dev[14]='d';
    dev[15]='e';
    dev[16]='q';
    dev[17]='u';
    dev[18]='e';
    dev[19]='u';
    dev[20]='e';
    dev[21]='.';
    dev[22]='p';
    dev[23]='h';
    dev[24]='p';
    dev[25]='?';
    dev[26]='i';
    dev[27]='d';
    dev[28]='=';
    uint8_t indStart = 29;//start index of 10000000000000000000000000000001 (cth)
    uint8_t ind = 38;
    char buffer [10];
    int myQueue=this->currentQueue;
    //int myQueue=52;
    itoa(myQueue,buffer,10);
    uint8_t j=0;
    for(uint8_t i=indStart;i<=ind;i++){
        if(buffer[j]!=NULL)
        dev[i]=buffer[j++];
    }
    this->httpGet2(dev);
    this->currentQueue=0;
    // Serial.println(dev);
};
