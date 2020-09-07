#include "MQTTpacket.h"
#include "string.h"
/********************MQTT�����غ���********************/

/***************
������MQTTstrlen
���ܣ���ȡMQTT�ַ�����
������mqttstring //mqtt�ַ��ṹ��
����ֵ��len //�ַ�����
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
������MQTTPacket_len
���ܣ�ѡ��ʣ�೤���ֽ���
������rem_len �ַ����ȼ�
����ֵ��len //�ַ�����
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
����: MQTTPacket_encode
���ܣ�����,��д��ʣ�೤��
����: ���Ļ�����ָ��*buf����ǰ�ֽڳ���
����: ���Ļ�����ʣ�೤��֮��λ��ָ��
***************************/
int MQTTPacket_encode(unsigned char* buf, int length)
{
	int rc = 0;


	do  											//ѭ������̶���ͷ�е�ʣ�೤���ֽڣ��ֽ�������ʣ���ֽڵ���ʵ���ȱ仯
	{
		char d = length % 128; 	//ʣ�೤��ȡ��128
		length /= 128;					//ʣ�೤��ȡ��128
		/* if there are more digits to encode, set the top bit of this digit */
		if (length > 0)
			d |= 0x80;          //ʣ�೤�����λ��ʶ�Ƿ��и����ֽ�
		buf[rc++] = d;        //ʣ�೤���ֽڼ�¼һ������
	} while (length > 0);

	return rc;
};





/***********************
����:writeChar
����:д��һ���ֽ�(��:�汾��\���ӱ�־)��MQTT���Ļ�����
����:MQTT	������ָ��pprt������
***********************/
void writeChar(unsigned char** pptr, char c)
{
	**pptr = c;
	(*pptr)++;
}


/***********************
����:writeInt
����:д���ַ�����(HEX):��λ�ֽڣ���λ�ֽ�
����:MQTT	������ָ��pprt���ַ�����
***********************/
void writeInt(unsigned char** pptr, int anInt)
{
	**pptr = (unsigned char)(anInt / 256);
	(*pptr)++;
	**pptr = (unsigned char)(anInt % 256);
	(*pptr)++;
}


/**************************
������writeString
���ܣ�д��UTF-8�ַ����������Ļ�����������֮ǰд�볤��
������MQTT������ָ��pprt,�ַ���
 **************************/
void writeString(unsigned char** pptr, const char* string)
{
	int len = strlen(string);
	writeInt(pptr, len);  //д�볤��
	memcpy(*pptr, string, len);
	*pptr += len;
}

/**************************
������writeMQTTString
���ܣ�д�������е���Ч����
������MQTT������ָ��pprt,��Ч�����ַ���
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



