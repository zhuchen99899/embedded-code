#ifndef __MQTTconnect_H
#define __MQTTconnect_H	 

#include "MQTTpacket.h"

/***************�ɱ�ͷ:���ӱ�־*******************/
typedef union
{
	unsigned char all;	/**���б�־(8bit) */
	struct
	{
		unsigned int : 1;	     					/**����λ */
		unsigned int cleansession : 1;	  /*cleansession��־ */
		unsigned int will : 1;			    /*willflag ��־ */
		unsigned int willQoS : 2;				/*willQos��־*/ //2bit
		unsigned int willRetain : 1;		/*will retain ��־  */
		unsigned int password : 1; 			/*password ��־*/
		unsigned int username : 1;			/*username	��־*/
	} bits;

} MQTTConnectFlags;	



/***************MQTT��Ϣ��������ṹ��************/
typedef struct
{
	/** �ṹ��ʶ������ must be MQTW. */
	char struct_id[4];
	/** �ṹ��汾��  Must be 0 */
	int struct_version;
	/** ���������������������  */
	MQTTString topicName;
	/**��Ҫ������������Ϣ */
	MQTTString message;
	/**
      * The retained flag for the LWT message (see MQTTAsync_message.retained).
      */
	unsigned char retained;
	/**
      * ��Ϣ�����ȼ�QOS (see
      * MQTTAsync_message.qos and @ref qos).
      */
	char qos;
	
} MQTTPacket_willOptions;


/*************mqtt CONNECT ����********************/
typedef struct
{
	/** �ṹ��ʶ������.  must be MQTC. */
	char struct_id[4];
/** �ṹ��汾��  Must be 0 */
	int struct_version;
	/** ʹ�õ�MQTT�汾  3 = 3.1 4 = 3.1.1 */
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
