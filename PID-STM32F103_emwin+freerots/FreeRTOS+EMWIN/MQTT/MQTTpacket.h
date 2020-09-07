#ifndef _MQTTPACKET_H_
#define _MQTTPACKET_H_

/*******MQTT发包错误退出******/
#define FUNC_EXIT_RC(x)
/***********MQTT报文返回码************/
enum errors
{
	MQTTPACKET_BUFFER_TOO_SHORT = -2,  //报文过短
	MQTTPACKET_READ_ERROR = -1,  //报文错误
	MQTTPACKET_READ_COMPLETE  //报文阅读完成
};

/***************MQTT控制报文类型*********************/
enum msgTypes
{
	CONNECT = 1, CONNACK, PUBLISH, PUBACK, PUBREC, PUBREL,
	PUBCOMP, SUBSCRIBE, SUBACK, UNSUBSCRIBE, UNSUBACK,
	PINGREQ, PINGRESP, DISCONNECT
};

/**********MQTT字符（字符+字节长度）*************/
typedef struct
{
	int len;
	char* string;
} MQTTString;



/****************固定头部**********************/
typedef union
{
	unsigned char byte;	                /*固定头部首字节（控制报文类型+控制报文标志位）*/

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

