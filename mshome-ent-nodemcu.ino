#include "JMFunctions.h"
#include "JMGlobal.h"
#include "JMWifi.h"
#include "JMWifiWire.h"
#include "JMData.h"


JMWifi *wifi=new JMWifi();
JMWifiWire *wifiWire=new JMWifiWire();
JMData *devData = new JMData();
unsigned long checkServerTime=0;
unsigned long requestUpdateTime=0;
bool ongoingPackage=false;
uint64_t package = 0;
bool initialized=false;
void setup()
{
    wifiWire->setAsMaster(D1,D2,receiveEvent);
    Serial.begin(JMGlobal::baudrate);
    delay(1000);
    wifi->setup(wifiWire);
    // Serial.println(F("RESET"));
}

void loop()
{
    if (ongoingPackage)
    {
      uint8_t msg = JMData::getMsgFromPacket(package);
      if (msg == JMGlobal::PACKET_MSG_DEVICES_DATA)
      {
        initialized=true;
        uint64_t val=JMData::getValueFromPacket(package);
        wifi->saveDevicesData(val);
      }else if (msg == JMGlobal::PACKET_MSG_QUEUED_DEVICES_DATA)
      {
        uint64_t val=JMData::getValueFromPacket(package);
        wifi->saveDevicesData(val);
        //delete queue db
        wifi->dequeueCommand();
        wifi->setReady(true);
      }else{
        //if(!initialized){
          // Serial.println("tleh");
          uint64_t package=JMData::devInitToInt64(wifi->httpGet2("/mshome-ent/g_dev_first_run.php"));
          wifiWire->sendMessage2(JMData::msgToBytes(package),8);
        //}
      }
      ongoingPackage=false;
      package=0;
    }else{
      unsigned long mil=millis();
      if(initialized){
        if(mil-checkServerTime>30000){
          wifi->checkServer();
          checkServerTime=mil;
        }
      }
      
      if(mil-requestUpdateTime>20000){
        // Serial.println("request");
        receiveEvent(wifiWire->getWire()->requestFrom(8,8));
        requestUpdateTime=mil;
      }
    }    
}

void receiveEvent(int howMany)
{
  // Serial.println("receive");
  byte data[howMany];
  int i = 0;
  while (wifiWire->getWire()->available())
  {
    byte c = wifiWire->getWire()->read();
    //Serial.println(c);
    data[i++] = c;
  }
  package = msgInt64(data);
  ongoingPackage=true;
}

uint64_t msgInt64(byte *msg)
{
  uint64_t d0 = msg[0];
  d0 = d0 << 56;
  uint64_t d1 = msg[1];
  d1 = d1 << 48;
  uint64_t d2 = msg[2];
  d2 = d2 << 40;
  uint64_t d3 = msg[3];
  d3 = d3 << 32;
  uint64_t d4 = msg[4];
  d4 = d4 << 24;
  uint64_t d5 = msg[5];
  d5 = d5 << 16;
  uint64_t d6 = msg[6];
  d6 = d6 << 8;
  uint64_t d7 = msg[7];
  return d0 + d1 + d2 + d3 + d4 + d5 + d6 + d7;
}