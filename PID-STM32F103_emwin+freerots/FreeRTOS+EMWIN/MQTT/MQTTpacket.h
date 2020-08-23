#ifndef _MQTTPACKET_H_
#define _MQTTPACKET_H_


/***********MQTT���ķ�����************/
enum errors
{
	MQTTPACKET_BUFFER_TOO_SHORT = -2,  //���Ĺ���
	MQTTPACKET_READ_ERROR = -1,  //���Ĵ���
	MQTTPACKET_READ_COMPLETE  //�����Ķ����
};

/***************MQTT���Ʊ�������*********************/
enum msgTypes
{
	CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL,
	PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK,
	PINGREQ=12, PINGRESP=13, DISCONNECT
};

/**********MQTT�ַ����ַ�+�ֽڳ��ȣ�*************/
typedef struct
{
	char* string;
	int len;
} MQTTString;



/****************�̶�ͷ��**********************/
typedef union
{
	unsigned char byte;	                /*�̶�ͷ�����ֽڣ����Ʊ�������+���Ʊ��ı�־λ��*/

	struct
	{
		unsigned int retain : 1;		/**< retained flag bit */
		unsigned int qos : 2;				/**< QoS value, 0, 1 or 2 */
		unsigned int dup : 1;				/**< DUP flag bit */
		unsigned int type : 4;			/**< message type nibble */
	} bits;

} MQTTHeader;




int MQTTstrlen(MQTTString mqttstring);
int MQTTPacket_len(int rem_len);
int MQTTPacket_encode(unsigned char* buf, int length);
void writeChar(unsigned char** pptr, char c);
void writeInt(unsigned char** pptr, int anInt);
void writeString(unsigned char** pptr, const char* string);
void writeMQTTString(unsigned char** pptr, MQTTString mqttstring);
#endif

