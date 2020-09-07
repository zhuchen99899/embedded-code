#include "MQTT.h"


//conncet封包配设
extern MQTTPacket_connectData SETCONNECT;  //Connect封包结构体

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

//SUBSCRITBE 封包配设
extern MQTTString SETSUBSCRIBE[subscribe_count];
extern int QoSs[subscribe_count];	
void subscribe_init()
{
/****订阅主题1***********/

SETSUBSCRIBE[subscribe_count-2].string=subscrible1_topicFilters1;
	
SETSUBSCRIBE[subscribe_count-2].len=strlen(subscrible1_topicFilters1);
	
QoSs[subscribe_count-2]=subscribe1_qos1;    

	
SETSUBSCRIBE[subscribe_count-1].string=subscrible1_topicFilters2;
	
SETSUBSCRIBE[subscribe_count-1].len=strlen(subscrible1_topicFilters2);
	
QoSs[subscribe_count-1]=subscribe1_qos2;    


};

extern MQTTString SETPUBLISH;

void publish_init()
{
SETPUBLISH.string=publish1_topicname;
SETPUBLISH.len=strlen(publish1_topicname);

};

