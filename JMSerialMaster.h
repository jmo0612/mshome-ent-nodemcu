#ifndef JMSerialMaster_h
#define JMSerialMaster_h
#include <SoftwareSerial.h>

class JMWifi;
class JMSerialMaster
{
private:
    SoftwareSerial *serial;
    JMWifi *wifi;

public:
    JMSerialMaster();
    void setup();
    void checkSerial();
};
#endif
