#include "JMTask.h"
#include "JMWifi.h"
#include "JMWifiWire.h"
#include "JMFunctions.h"
#include <ArduinoJson.h>

JMTask::JMTask(JMWifi* wifi,JMWifiWire *wifiWire){
    this->wifi=wifi;
    this->wifiWire=wifiWire;
    //this->stats="00000000000000";
    this->stats="";
    this->cmds=new List<char*>();
};
void JMTask::setup(){
    const char* res=this->wifi->httpGet2("/mshome-ent/g_dev.php");
    //Serial.println(res);
    if(res!="1" && res!=""){
        this->stats=(char*)res;
        //Serial.println(this->stats);
        //Serial.println(this->getStatsPacked());
        //Serial.println("done");
        this->wifiWire->sendMessage(this->getStatsPacked(),8);
    }

};

const char* JMTask::getStatsPacked(){
    if(this->stats=="")return "";
    char ret[32];
    ret[0]='|';
    ret[1]='S';
    ret[2]='T';
    ret[3]='A';
    ret[4]='T';
    ret[5]='S';
    for(int i=6;i<32;i++){
        if(this->stats[i-6]=='\0'){
            ret[i]='|';
        }else{
            ret[i]=this->stats[i-6];
        }
    }
    //Serial.println(ret);
    return JMFunctions::strToCharP(ret);
};
const char* JMTask::getCmdsPacked(int index){
    if(this->cmds->getSize()==0)return "";
    char ret[32];
    ret[0]='|';
    ret[1]='C';
    ret[2]='M';
    ret[3]='D';
    for(int i=4;i<32;i++){
        if(this->cmds->getValue(index)[i-4]=='\0'){
            ret[i]='|';
        }else{
            ret[i]=this->cmds->getValue(index)[i-4];
        }
    }
    //Serial.println(ret);
    return JMFunctions::strToCharP(ret);
};


//Siblings with uno's JMCommand
const char *JMTask::extractTaskType(const char *msg)
{
    if (msg[1] == 'I' && msg[2] == 'N' && msg[3] == 'I' && msg[4] == 'T')
    {
        return "INIT";
    }
    else if (msg[1] == 'U' && msg[2] == 'P' && msg[3] == 'D')
    {
        return "UPD";
    }
    return "NONE";
};
const char *JMTask::extractTaskMsg(const char *msg, const char *taskType)
{
    // Serial.println(taskType);
    if (taskType == "NONE")
        return "";
    int ind = 32;
    if (taskType == "INIT")
    {
        // Serial.println("stats");
        ind = 5;
    }
    else if (taskType == "UPD")
    {
        // Serial.println("cmd");
        ind = 4;
    }
    int j = 0;
    for (int i = ind; i < 32; i++)
    {
        if (msg[i] == '|')
            break;
        j++;
    }
    String ret = "";
    for (int i = 0; i < j; i++)
    {
        ret += msg[ind++];
    }
    // Serial.println(ret);
    return JMFunctions::strToCharP(ret);
};
void JMTask::checkCommands(){
    const char* res=this->wifi->httpGet2("/mshome-ent/g_cmd.php");
    Serial.println(res);
    if(res!="1" && res!=""){
        StaticJsonDocument<256> doc;
        deserializeJson(doc, res);
        for(int i=0;i<doc.size();i++){
            const char* cmd=doc[i]["cmd"];
            char* tmp=(char*)cmd;
            this->cmds->add(tmp);
            Serial.println(tmp);
        }
        for(int i=0;i<this->cmds->getSize();i++){
            //Serial.println(this->getCmdsPacked(i));
            this->wifiWire->sendMessage(this->getCmdsPacked(i),8);
        }
        this->cmds->clear();
    }else{

    }
};
void JMTask::processTask(const char *msg)
{
    const char *taskType = this->extractTaskType(msg);
    const char *taskData = this->extractTaskMsg(msg, taskType);
    if (taskType == "INIT")
    {
        this->setup();
    }
    else if (taskType == "UPD")
    {
        //Serial.println(F("masuk"));
        String strStats=this->stats;
        String strData=taskData;
        if(strStats.compareTo(strData)!=0){
            String url="/mshome-ent/p_dev.php?sta="+strData;
            const char* res=this->wifi->httpGet2(JMFunctions::strToCharP(url));
            //Serial.println(res);
            if(res!="1" && res!=""){
                this->stats=(char*)taskData;
            }
        }
    }
};