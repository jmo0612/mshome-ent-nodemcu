#ifndef JMData_h
#define JMData_h
#include "Arduino.h"

class JMData
{
private:
    int dev[32] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    uint64_t dataToInt64(int msg, uint64_t val);
    int getMsgFromPacket(uint64_t packet);
    uint64_t getValueFromPacket(uint64_t packet);
    void processPacket(uint64_t packet);
    uint64_t getPow3s(int ind);

public:
    JMData();
    uint64_t devDataToInt64(int msg);
    long long devDataToInt64b(int msg);
    void updateDevData(uint64_t val);
    void cek();
};
#endif
