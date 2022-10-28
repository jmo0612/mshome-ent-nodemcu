#ifndef JMGlobal_h
#define JMGlobal_h

class JMGlobal
{
public:
    static const unsigned long baudrate = 115200;
    static const unsigned long wifiTimeOut = 5000;
    static const unsigned int wireMaxChar = 20;

    static const int PACKET_MSG_UPDATE_DEVICES_DATA = 0;
    static const int PACKET_MSG_REQUEST_DEVICES_DATA = 1;
    static const int PACKET_MSG_DO_CMD = 2;

    static const int DO_CMD_BOX_TO_LG = 1;
    static const int DO_CMD_INDI_TO_LG = 2;
    static const int DO_CMD_PS_TO_LG = 3;
    static const int DO_CMD_ELSE_TO_LG = 4;
    static const int DO_CMD_BOX_TO_AKARI = 5;
    static const int DO_CMD_INDI_TO_AKARI = 6;
    static const int DO_CMD_PS_TO_AKARI = 7;
    static const int DO_CMD_ELSE_TO_AKARI = 8;
    static const int DO_CMD_TOGGLE_POWER_SPEAKER = 9;
    static const int DO_CMD_CALIBRATE_POWER_LG = 10;
    static const int DO_CMD_CALIBRATE_POWER_AKARI = 11;
    static const int DO_CMD_CALIBRATE_POWER_SPEAKER = 12;
    static const int DO_CMD_CALIBRATE_POWER_MATRIX = 13;

    static const int DEV_DISPLAY_LG = 0;
    static const int DEV_DISPLAY_AKARI = 1;
    static const int DEV_HDMI_AMP_LG = 2;
    static const int DEV_HDMI_AMP_AKARI = 3;
    static const int DEV_HDMI_AMP_PS = 4;
    static const int DEV_HDMI_MATRIX = 5;
    static const int DEV_PLAYER_BOX = 6;
    static const int DEV_PLAYER_INDI = 7;
    static const int DEV_PLAYER_PS = 8;
    static const int DEV_SERVER_EVERCOSS_BAT = 9;
    static const int DEV_SERVER_EVERCOSS_CHARGER = 10;
    static const int DEV_SERVER_NAS = 11;
    static const int DEV_HDD_DOCK = 12;
    static const int DEV_SPEAKER = 13;

    static const int DEV_REMOTE_A1 = 16;
    static const int DEV_REMOTE_A2 = 17;
    static const int DEV_REMOTE_A3 = 18;
    static const int DEV_REMOTE_A4 = 19;
    static const int DEV_REMOTE_B1 = 20;
    static const int DEV_REMOTE_B2 = 21;
    static const int DEV_REMOTE_B3 = 22;
    static const int DEV_REMOTE_B4 = 23;
};
#endif
