#include "MQTTpacket.h"
#include "string.h"
/********************MQTT打包相关函数********************/

/***************
函数：MQTTstrlen
功能：获取MQTT字符长度
参数：mqttstring //mqtt字符结构体
返回值：len //字符长度
****************/
int MQTTstrlen(MQTTString mqttstring)
{
	int len = 0;

	if (mqttstring.string)
		len = strlen(mqttstring.string);
	else
		len = mqttstring.len;
	return len;
};

/****************
函数：MQTTPacket_len
功能：选择剩余长度字节数
参数：rem_len 字符长度及
返回值：len //字符长度
*****************/
int MQTTPacket_len(int rem_len)
{
	rem_len += 1; /* header byte */

	/* now remaining_length field */
	if (rem_len < 128)
		rem_len += 1;
	else if (rem_len < 16384)
		rem_len += 2;
	else if (rem_len < 2097151)
		rem_len += 3;
	else
		rem_len += 4;
	return rem_len;
}



/**************************
函数: MQTTPacket_encode
功能：计算,并写入剩余长度
参数: 报文缓冲区指针*buf，当前字节长度
返回: 报文缓冲区剩余长度之后位置指针
***************************/
int MQTTPacket_encode(unsigned char* buf, int length)
{
	int rc = 0;


	do  											//循环处理固定报头中的剩余长度字节，字节量根据剩余字节的真实长度变化
	{
		char d = length % 128; 	//剩余长度取余128
		length /= 128;					//剩余长度取整128
		/* if there are more digits to encode, set the top bit of this digit */
		if (length > 0)
			d |= 0x80;          //剩余长度最高位标识是否有更多字节
		buf[rc++] = d;        //剩余长度字节记录一个数据
	} while (length > 0);

	return rc;
};





/***********************
函数:writeChar
功能:写入一个字节(如:版本号\连接标志)至MQTT报文缓冲区
参数:MQTT	缓冲区指针pprt，数据
***********************/
void writeChar(unsigned char** pptr, char c)
{
	**pptr = c;
	(*pptr)++;
}


/***********************
函数:writeInt
功能:写入字符长度(HEX):高位字节，低位字节
参数:MQTT	缓冲区指针pprt，字符长度
***********************/
void writeInt(unsigned char** pptr, int anInt)
{
	**pptr = (unsigned char)(anInt / 256);
	(*pptr)++;
	**pptr = (unsigned char)(anInt % 256);
	(*pptr)++;
}


/**************************
函数：writeString
功能：写入UTF-8字符串，至报文缓冲区，并在之前写入长度
参数：MQTT缓冲区指针pprt,字符串
 **************************/
void writeString(unsigned char** pptr, const char* string)
{
	int len = strlen(string);
	writeInt(pptr, len);  //写入长度
	memcpy(*pptr, string, len);
	*pptr += len;
}

/**************************
函数：writeMQTTString
功能：写入配设中的有效负载
参数：MQTT缓冲区指针pprt,有效负载字符串
 **************************/
void writeMQTTString(unsigned char** pptr, MQTTString mqttstring)
{
	if (mqttstring.len > 0)
	{
		writeInt(pptr, mqttstring.len);
		memcpy(*pptr, mqttstring.string, mqttstring.len);
		*pptr += mqttstring.len;
	}
	else if (mqttstring.string)
		writeString(pptr, mqttstring.string);
	else
		writeInt(pptr, 0);
}



