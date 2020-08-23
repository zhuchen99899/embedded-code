#ifndef __MQTTconnect_H
#define __MQTTconnect_H	 

#include "MQTTpacket.h"

/***************可变头:连接标志*******************/
typedef union
{
	unsigned char all;	/**所有标志(8bit) */
	struct
	{
		unsigned int : 1;	     					/**保留位 */
		unsigned int cleansession : 1;	  /*cleansession标志 */
		unsigned int will : 1;			    /*willflag 标志 */
		unsigned int willQoS : 2;				/*willQos标志*/ //2bit
		unsigned int willRetain : 1;		/*will retain 标志  */
		unsigned int password : 1; 			/*password 标志*/
		unsigned int username : 1;			/*username	标志*/
	} bits;

} MQTTConnectFlags;	



/***************MQTT消息遗嘱配设结构体************/
typedef struct
{
	/** 结构体识别序列 must be MQTW. */
	char struct_id[4];
	/** 结构体版本号  Must be 0 */
	int struct_version;
	/** 将发布的遗嘱主题的名称  */
	MQTTString topicName;
	/**将要发布的遗嘱消息 */
	MQTTString message;
	/**
      * The retained flag for the LWT message (see MQTTAsync_message.retained).
      */
	unsigned char retained;
	/**
      * 消息质量等级QOS (see
      * MQTTAsync_message.qos and @ref qos).
      */
	char qos;
	
} MQTTPacket_willOptions;


/*************mqtt CONNECT 配设********************/
typedef struct
{
	/** 结构体识别序列.  must be MQTC. */
	char struct_id[4];
/** 结构体版本号  Must be 0 */
	int struct_version;
	/** 使用的MQTT版本  3 = 3.1 4 = 3.1.1 */
	unsigned char MQTTVersion;
	MQTTString clientID;
	unsigned short keepAliveInterval;
	unsigned char cleansession;
	unsigned char willFlag;
	MQTTPacket_willOptions will;
	MQTTString username;
	MQTTString password;
} MQTTPacket_connectData ;








int MQTT_Connect_lenth(MQTTPacket_connectData* options);
int MQTT_Connect(unsigned char MQTT_buf[256], int buflen, MQTTPacket_connectData* options);


#endif
