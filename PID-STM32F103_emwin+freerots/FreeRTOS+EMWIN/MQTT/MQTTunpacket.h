#ifndef _MQTTUNPACKET_H_
#define _MQTTUNPACKET_H_

#include "MQTTpacket.h"
#include "string.h"
int MQTTPacket_encodeused(unsigned char* buf, int length);
int MQTTPacket_decode(int (*getcharfn)(unsigned char*, int), int* value);
char readChar(unsigned char** pptr);
int bufchar(unsigned char* c, int count);
int MQTTPacket_decodeBuf(unsigned char* buf, int* value);
int readInt(unsigned char** pptr);
int readMQTTLenString(MQTTString* mqttstring, unsigned char** pptr, unsigned char* enddata);

#endif



