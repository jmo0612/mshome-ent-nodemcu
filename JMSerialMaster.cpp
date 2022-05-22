#include "JMSerialMaster.h"
#include "JMFunctions.h"
#include "JMWifi.h"
#include "JMGlobal.h"

JMSerialMaster::JMSerialMaster()
{
    this->wifi=new JMWifi();
};
void JMSerialMaster::setup()
{
    this->serial=new SoftwareSerial(D6,D5);
    this->serial->begin(JMGlobal::baudrate);
    //this->serial->setTimeout(JMGlobal::serialTimeOut);
    this->wifi->setup();
};
void JMSerialMaster::checkSerial(){
    if(this->serial->available()!=0){
        const String data=this->serial->readString();
        Serial.println(data);
    }
};