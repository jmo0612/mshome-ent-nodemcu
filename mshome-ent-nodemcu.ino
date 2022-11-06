#include "JMFunctions.h"
#include "JMGlobal.h"
#include "JMWifi.h"
#include "JMWifiWire.h"
//#include "JMTask.h"
#include "JMData.h"
//#include "JMMsgs.h"


JMWifi *wifi=new JMWifi();
JMWifiWire *wifiWire=new JMWifiWire();
JMData *devData = new JMData();
unsigned long checkServerTime=0;
unsigned long requestUpdateTime=0;
//JMMsgs *msgs = new JMMsgs();
//JMTask *tasks=new JMTask(wifi,wifiWire);
void setup()
{
    wifiWire->setAsMaster(D1,D2,receiveEvent);
    Serial.begin(JMGlobal::baudrate);
    delay(1000);
    
    Serial.println(F("RESET"));
    wifi->setup(wifiWire);
    
    //wifiWire->sendMessage("D100000000000000000000000000001D",8);
    //Serial.println(wifi->httpGet2("/mshome-ent/g_dev.php"));
    //tasks->setup();
}

void loop()
{
    unsigned long mil=millis();
    if(mil-checkServerTime>30000){
      //wifi->checkServer();
      checkServerTime=mil;
    }
    if(mil-requestUpdateTime>20000){
      //receiveEvent(wifiWire->getWire()->requestFrom(8,8));
      wifiWire->getWire()->requestFrom(8,8);
      receiveEvent(8);
      requestUpdateTime=mil;
    }
    
    //receiveEvent(wifiWire->getWire()->requestFrom(8,8));
    //w->sendMessage("Nama saya Jimmy Mondow, tinggal di kalawac, minahasa utara");
    /*static char msgTot[4];
    msgTot[0]=haha[0][8];
    msgTot[1]=haha[0][9];
    msgTot[2]=haha[0][10];
    msgTot[3]=haha[0][11];
    //Serial.println(int(msgTot[]));
    //JMFunctions::bytesToInt(msgTot);
    Serial.println(JMFunctions::bytesToInt(msgTot));
*/
    //receiveEvent(Wire.requestFrom(8,58));
    //tes();
    //tasks->checkCommands();
    //receiveEvent(wifiWire->getWire()->requestFrom(8,8));
    //processMsg();
    //delay(5000);
}

void receiveEvent(int howMany)
{
  Serial.println("ada");
  byte data[howMany];
  int i = 0;
  while (wifiWire->getWire()->available())
  {
    byte c = wifiWire->getWire()->read();
    Serial.println(c);
    data[i++] = c;
  }

  /*if (wifiWire->getWire()->available()){
    wifiWire->getWire()->readBytes(data,8);
  }*/

  //uint64_t data=0;
  //wifiWire->getWire()->readBytes(data,8);
  /*for(int i=0;i<8;i++){
    int x=data[i];
    Serial.println("puks");
  }*/





  //uint64_t tmp=msgInt64(data);
  //Serial.println(ToString(tmp));

  //uint64_t data2 = devData->devDataToInt64(JMGlobal::PACKET_MSG_UPDATE_DEVICES_DATA);
  //msgStr(tmp);
  //Serial.println(ToString(data2));
  //uint64_t tmp=1000;
  //msgs->queueMsg(tmp);
}

uint64_t msgInt64(char *msg)
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

/*void processMsg()
{
  uint64_t msg = msgs->dequeueMsg();
  if (msg == 0)
    return;
  uint64_t msgType = msg >> 56;
  uint64_t tmp = msg << 8;
  uint64_t data = tmp >> 8;
  if (msgType == JMGlobal::PACKET_MSG_UPDATE_DEVICES_DATA)
  {
    //devData->updateDevData(data);
  }
  else if (msgType == JMGlobal::PACKET_MSG_REQUEST_DEVICES_DATA)
  {
    //uint64_t data = devData->devDataToInt64(JMGlobal::PACKET_MSG_UPDATE_DEVICES_DATA);
    // wifiWire->sendMessage(msgStr(data));
  }
  else
  {
    //cmd->doCommand(data);
  }
}*/