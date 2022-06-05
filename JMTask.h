#ifndef JMTask_h
#define JMTask_h
#include <List.hpp>

class JMWifi;
class JMWifiWire;
class JMTask
{
private:
    JMWifi* wifi;
    JMWifiWire *wifiWire;
    char* stats;
    List<char*>* cmds;

    //siblings with uno's JMCommand
    const char *extractTaskType(const char *msg);
    const char *extractTaskMsg(const char *msg, const char *taskType);

public:
    JMTask(JMWifi* wifi,JMWifiWire *wifiWire);
    void setup();
    const char* getStatsPacked();
    const char* getCmdsPacked(int index);
    void checkCommands();

    //siblings with uno's JMCommand
    void processTask(const char *msg);
};
#endif
