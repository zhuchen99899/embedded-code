#include "MQTT.h"



extern MQTTPacket_connectData SETCONNECT;

void conncect_init()
{
memcpy(SETCONNECT.struct_id,MQTT_CONNCET_ID,strlen(MQTT_CONNCET_ID));
SETCONNECT.struct_version =MQTT_CONNCET_version;
SETCONNECT.cleansession= cleansession_FLAG;
SETCONNECT.MQTTVersion=MQTT_Version;
SETCONNECT.clientID.string=MQTT_clientID;
SETCONNECT.clientID.len=strlen(MQTT_clientID);
SETCONNECT.keepAliveInterval=	keepAlive_Interval;
SETCONNECT.cleansession=cleansession_FLAG;
SETCONNECT.willFlag =Will_Flag;
memcpy(SETCONNECT.struct_id,MQTT_WILL_ID,strlen(MQTT_WILL_ID));
SETCONNECT.will.struct_version=MQTT_WILL_version;
SETCONNECT.will.topicName.string=will_topicName;
SETCONNECT.will.topicName.len=strlen(will_topicName);
SETCONNECT.will.message.string=will_message;
SETCONNECT.will.message.len=strlen(will_message);
SETCONNECT.will.qos=will_qos;
SETCONNECT.will.retained=will_retained;
SETCONNECT.username.string=MQTT_username;
SETCONNECT.username.len=strlen(MQTT_username);
SETCONNECT.password.string=MQTT_password;
SETCONNECT.password.len=strlen(MQTT_password);
};



	





