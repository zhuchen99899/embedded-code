#ifndef __MQTTPUBLISH_H
#define __MQTTPUBLISH_H	 

#include "MQTTpacket.h"
#include "MQTTunpacket.h"
#include "string.h"


/*********************publish消息使用函数***********************/
int MQTT_Publish_length(int qos, MQTTString topicName, int payloadlen);
int MQTT_publish(unsigned char* MQTT_buf, int buflen, unsigned char dup, int qos, unsigned char retained, unsigned short packetid,
		MQTTString topicName, unsigned char* payload, int payloadlen);


/**********************接收publish消息使用***********************/

int MQTTDeserialize_publish(unsigned char* dup, int* qos, unsigned char* retained, unsigned short* packetid, MQTTString* topicName,
		unsigned char** payload, int* payloadlen, unsigned char* buf, int buflen);
#endif


