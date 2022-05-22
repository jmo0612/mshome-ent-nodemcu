#include "JMFunctions.h"
#include "JMSerialMaster.h"
#include "JMGlobal.h"

JMSerialMaster *bridge=new JMSerialMaster();

void setup()
{
    
    Serial.begin(JMGlobal::baudrate);
    delay(1000);
    Serial.println("RESET");
    bridge->setup();
}

void loop()
{
    bridge->checkSerial();
}