#include "JMFunctions.h"
#include "JMGlobal.h"
#include "JMWifi.h"
#include "JMWifiWire.h"
#include "JMTask.h"


JMWifi *wifi=new JMWifi();
JMWifiWire *wifiWire=new JMWifiWire();
JMTask *tasks=new JMTask(wifi,wifiWire);
void setup()
{
    wifiWire->setAsMaster(D1,D2,receiveEvent);
    Serial.begin(JMGlobal::baudrate);
    delay(1000);
    
    Serial.println(F("RESET"));
    wifi->setup();
    //receiveEvent(wifiWire->getWire()->requestFrom(8,32));
    //wifiWire->sendMessage("D100000000000000000000000000001D",8);
    //Serial.println(wifi->httpGet2("/mshome-ent/g_dev.php"));
    tasks->setup();
}

void loop()
{

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
    tasks->checkCommands();
    receiveEvent(wifiWire->getWire()->requestFrom(8,32));
    delay(5000);
}

void receiveEvent(size_t howMany)
{
  // Serial.println(F("received"));
  char data[32] = {};
  int i = 0;
  while (wifiWire->getWire()->available())
  {
    char c = wifiWire->getWire()->read();
    // Serial.println(int(c));
    data[i++] = c;
  }
  //Serial.println(data);
  tasks->processTask(data);
  //wifiWire->sendMessage("D100000000000000000000000000001D",8);
}