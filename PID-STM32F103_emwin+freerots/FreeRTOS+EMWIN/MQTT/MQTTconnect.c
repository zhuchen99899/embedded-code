#include "MQTTpacket.h"
#include "MQTTconnect.h"

/********************************************************
����: MQTT_Connect_lenth
���ܣ�����MQTT CONNECT���ĳ���
������temp_buff[256]  //MQTT���ͱ��Ļ���
			options         //MQTTconnect����ṹ��
����ֵ�� len   �������ֽ��� (���̶�ͷ��)
********************************************************/


int MQTT_Connect_lenth(MQTTPacket_connectData* options)
{
	int len = 0;
/*****************����̶���ͷ�Լ��ɱ䱨ͷ�����ֽ�����ͨ��MQTTPacket_len�����ж�ʣ���ֽ��ֽ�������룬��������������*************/	
	
	
	
	
	
	/*************�ɱ�ͷ=Э����+Э�鼶��+���ӱ�־(һ���ֽ�),����ʱ��(�����ֽ�)****************/
 /* �ж���MQlsdp����MQTTЭ��汾 */
	if (options->MQTTVersion == 3)
		len = 12;
	else if (options->MQTTVersion == 4)
		len = 10;
	
	
	/***********��Ч�غ�*************/
	/*cilentID*/
	  len += MQTTstrlen(options->clientID)+2;
	/*�������������־*/
	if (options->willFlag)
		/*���������������Ϣ*/
		len += MQTTstrlen(options->will.topicName)+2 + MQTTstrlen(options->will.message)+2;
	/*��������û�����־*/
	if (options->username.string )
		len += MQTTstrlen(options->username)+2;
	/*������������־*/
	if (options->password.string )
		len += MQTTstrlen(options->password)+2;
	return len;
};


/********************************************************
����: MQTT_Connect
���ܣ�MQTT CONNECT ���Ĳ��
������MQTT_buf  ���ķ��ͻ�����
			buflen ����������         
			options         //MQTTconnect����ṹ��
����ֵ�r rc  (���ĳ���)
********************************************************/



int MQTT_Connect(unsigned char* MQTT_buf, int buflen, MQTTPacket_connectData* options)
{
unsigned char *ptr = MQTT_buf;
	
	int len = 0;
	int rc = -1;
	MQTTHeader header = {0};//�̶�ͷ�� ����λ��0
	MQTTConnectFlags flags = {0}; //���ӱ�־����λ��0
	
	/****************��ʣ���ֽ�������������*******************************************/
	if (MQTTPacket_len(len = MQTT_Connect_lenth(options)) > buflen)
	{
		rc = MQTTPACKET_BUFFER_TOO_SHORT;
		goto exit;
	}

	
	/******д��̶�ͷ��*********/
	header.byte = 0;
	header.bits.type = CONNECT;
	writeChar(&ptr, header.byte); /* д��̶�ͷ */
	ptr += MQTTPacket_encode(ptr, len); /*д��ʣ�೤�� */
/**************д��ɱ�ͷ*****************/	
	/****����MQTT�汾д��Э�飬�汾��****/
	if (options->MQTTVersion == 4)
	{
		writeString(&ptr, "MQTT");
		writeChar(&ptr, (char) 4);
	}
	else
	{
		writeString(&ptr, "MQIsdp");
		writeChar(&ptr, (char) 3);
	}
	/********д�����ӱ�*********/
	flags.all = 0; //��ʼ���������ӱ��£���0
	flags.bits.cleansession = options->cleansession; //��ֵcleansession��־
	flags.bits.will = (options->willFlag) ? 1 : 0; //willflagΪ��ȡ1������Ϊ0
	 //�������������ע
	if (flags.bits.will)
	{
		//���� QOS retain ��־
		flags.bits.willQoS = options->will.qos;
		flags.bits.willRetain = options->will.retained;
	}

	//��������û�����־
	if (options->username.string )
		flags.bits.username = 1;
	//������������־
	if (options->password.string )
		flags.bits.password = 1;
	//д�����ӱ�־
	writeChar(&ptr, flags.all);
	
	/*************д�뱣��ʱ��*****************/
	writeInt(&ptr, options->keepAliveInterval);
	
	/***********д�븺��**********/
	writeMQTTString(&ptr, options->clientID);
	if (options->willFlag)
	{
		writeMQTTString(&ptr, options->will.topicName);
		writeMQTTString(&ptr, options->will.message);
	}
	if (flags.bits.username)
		writeMQTTString(&ptr, options->username);
	if (flags.bits.password)
		writeMQTTString(&ptr, options->password);

	
	//������

	rc = ptr - MQTT_buf;
	

	exit:
	FUNC_EXIT_RC(rc);
	return rc; //���س���
};


