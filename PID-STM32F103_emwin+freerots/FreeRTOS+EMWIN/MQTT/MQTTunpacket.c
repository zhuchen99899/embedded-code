/****************MQTT拆包相关函数(反序列化)***************/

#include "MQTTunpacket.h"
#include "usart.h"

/**********MQTTpacket_decode中参数函数int (*getcharfn)(unsigned char*, int)指针指向的函数*****************/
int MQTTPacket_encodeused(unsigned char* buf, int length)
{
	int rc = 0;

	do
	{
		char d = length % 128;
		length /= 128;
		/* if there are more digits to encode, set the top bit of this digit */
		if (length > 0)
			d |= 0x80;
		buf[rc++] = d;
	} while (length > 0);
	FUNC_EXIT_RC(rc);
	return rc;
}


/**
 * 解码消息长度
 * @param 从数据报文中读取字符指针
 * @param 返回解码长度值
 * @return 从接收缓冲区读取的字节长度
 */
int MQTTPacket_decode(int (*getcharfn)(unsigned char*, int), int* value)
{
	unsigned char c;
	int multiplier = 1;
	int len = 0;
#define MAX_NO_OF_REMAINING_LENGTH_BYTES 4

	*value = 0;
	do
	{
		int rc = MQTTPACKET_READ_ERROR;

		if (++len > MAX_NO_OF_REMAINING_LENGTH_BYTES)
		{
			rc = MQTTPACKET_READ_ERROR;	/* bad data */
			goto exit;
		}
		rc = (*getcharfn)(&c, 1);
		if (rc != 1)
			goto exit;
		*value += (c & 127) * multiplier;
		multiplier *= 128;
	} while ((c & 128) != 0);
exit:
	FUNC_EXIT_RC(len);
	return len;
}



/**
 * 接收读取一个字节
 * @param 接收缓冲区指针- incremented by the number of bytes used & returned
 * @return 返回下一个字节指针
 */
char readChar(unsigned char** pptr)
{
	char c = **pptr;
	(*pptr)++;
	return c;
}


static unsigned char* bufptr; //解包指针

int bufchar(unsigned char* c, int count)
{
	int i;

	for (i = 0; i < count; ++i)
		*c = *bufptr++;
	return count;
	
}  //读取字符长度

int MQTTPacket_decodeBuf(unsigned char* buf, int* value)
{
	bufptr = buf;
	return MQTTPacket_decode(bufchar, value);
}
//解包长度
 





/**
 * Calculates an integer from two bytes read from the input buffer
 * 计算一个字符长度(指针)————高位低位合成
 * @param 缓冲区指针
 * @return 返回具体长度
 */
int readInt(unsigned char** pptr)
{
	unsigned char* ptr = *pptr;
	int len = 256*(*ptr) + (*(ptr+1));
	*pptr += 2;
	return len;
}

/**
 * @param mqtt字符结构体
 * @param 接收缓冲区指针
 * @param 尾部指针（读取是不超过这个指针）
 * @return 1 if successful, 0 if not
 */
int readMQTTLenString(MQTTString* mqttstring, unsigned char** pptr, unsigned char* enddata)
{
	int rc = 0;
/************开始两字节为字符长度*******************/
	if (enddata - (*pptr) > 1) /* 缓冲区存在数据未读取 */
	{
		mqttstring->len = readInt(pptr); /* 增加长度指针 */
		if (&(*pptr)[mqttstring->len] <= enddata)
		{
			//字符长度未超过尾部指针
			mqttstring->string = (char*)*pptr;
			*pptr += mqttstring->len;//读取字符
			rc = 1;
		}
	}
	
	//读取到结尾

	FUNC_EXIT_RC(rc);
	return rc;
}



