#ifndef _MQTTSUBSCRIBE_H_
#define _MQTTSUBSCRIBE_H_
#include "MQTTpacket.h"


int MQTTS_subscribeLength(int count, MQTTString topicFilters[]);


int MQTT_subscribe(unsigned char* MQTT_buf, int buflen, unsigned char dup, unsigned short packetid, int count,
		MQTTString topicFilters[], int requestedQoSs[]);

int MQTTDeserialize_suback(unsigned short* packetid, int maxcount, int* count, int grantedQoSs[], unsigned char* buf, int buflen);



#endif


