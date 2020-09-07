/****************MQTT�����غ���(�����л�)***************/

#include "MQTTunpacket.h"
#include "usart.h"

/**********MQTTpacket_decode�в�������int (*getcharfn)(unsigned char*, int)ָ��ָ��ĺ���*****************/
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
 * ������Ϣ����
 * @param �����ݱ����ж�ȡ�ַ�ָ��
 * @param ���ؽ��볤��ֵ
 * @return �ӽ��ջ�������ȡ���ֽڳ���
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
 * ���ն�ȡһ���ֽ�
 * @param ���ջ�����ָ��- incremented by the number of bytes used & returned
 * @return ������һ���ֽ�ָ��
 */
char readChar(unsigned char** pptr)
{
	char c = **pptr;
	(*pptr)++;
	return c;
}


static unsigned char* bufptr; //���ָ��

int bufchar(unsigned char* c, int count)
{
	int i;

	for (i = 0; i < count; ++i)
		*c = *bufptr++;
	return count;
	
}  //��ȡ�ַ�����

int MQTTPacket_decodeBuf(unsigned char* buf, int* value)
{
	bufptr = buf;
	return MQTTPacket_decode(bufchar, value);
}
//�������
 





/**
 * Calculates an integer from two bytes read from the input buffer
 * ����һ���ַ�����(ָ��)����������λ��λ�ϳ�
 * @param ������ָ��
 * @return ���ؾ��峤��
 */
int readInt(unsigned char** pptr)
{
	unsigned char* ptr = *pptr;
	int len = 256*(*ptr) + (*(ptr+1));
	*pptr += 2;
	return len;
}

/**
 * @param mqtt�ַ��ṹ��
 * @param ���ջ�����ָ��
 * @param β��ָ�루��ȡ�ǲ��������ָ�룩
 * @return 1 if successful, 0 if not
 */
int readMQTTLenString(MQTTString* mqttstring, unsigned char** pptr, unsigned char* enddata)
{
	int rc = 0;
/************��ʼ���ֽ�Ϊ�ַ�����*******************/
	if (enddata - (*pptr) > 1) /* ��������������δ��ȡ */
	{
		mqttstring->len = readInt(pptr); /* ���ӳ���ָ�� */
		if (&(*pptr)[mqttstring->len] <= enddata)
		{
			//�ַ�����δ����β��ָ��
			mqttstring->string = (char*)*pptr;
			*pptr += mqttstring->len;//��ȡ�ַ�
			rc = 1;
		}
	}
	
	//��ȡ����β

	FUNC_EXIT_RC(rc);
	return rc;
}



