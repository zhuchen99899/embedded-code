#include <MQTTsubscribe.h>
#include <MQTTpacket.h>
#include <MQTTunpacket.h>
#include <stdio.h>

/********************************************************
����: MQTTS_subscribeLength
���ܣ�����MQTT SUBSCRIBE�����ܳ���
������count									 //�������
			topicFilters[]         //���������
����ֵ�� len �������ֽ��� (���̶�ͷ��)
********************************************************/

int MQTTS_subscribeLength(int count, MQTTString topicFilters[])
{
	int i;
	int len = 2; /* packetid */

	for (i = 0; i < count; ++i)
		len += 2 + MQTTstrlen(topicFilters[i]) + 1; /* length + topic + req_qos */
	return len;
}


/********************************************************
����: MQTT_subscribe
���ܣMMQTT SUBSCRIBE �������
������MQTT_buf                //���ķ��ͻ�����
			buflen                  //����������
			dup                     //ѭ�����ͱ�־  ���Ʊ��ı�־λ bit3λ��
			packetid								//���ı�ʶ��
			count									 //�������
			topicFilters[]         //���������
			requestedQoSs[]				 //Qos�ȼ�
����ֵ���������ֽ��� (���̶�ͷ��)
********************************************************/
int MQTT_subscribe(unsigned char* MQTT_buf, int buflen, unsigned char dup, unsigned short packetid, int count,
		MQTTString topicFilters[], int requestedQoSs[])
{
	unsigned char *ptr = MQTT_buf;
	
	MQTTHeader header = {0};//�̶�ͷ�� ����λ��0
	int rem_len = 0;
	int rc = 0;
	int i = 0;


	if (MQTTPacket_len(rem_len = MQTTS_subscribeLength(count, topicFilters)) > buflen)
	{
		rc = MQTTPACKET_BUFFER_TOO_SHORT;
		goto exit;
	}

	header.byte = 0;      //�̶�ͷ���ֽڳ�ʼ��
	header.bits.type = SUBSCRIBE;  //���Ʊ�������
	header.bits.dup = dup;          //���Ʊ��ı�־λ��SUBSCRIBE�̶�bit4Ϊ0
	header.bits.qos = 1;						//���Ʊ��ı�־λ��SUBSCRIBE�̶�bit1λΪ1
	/* д��̶�ͷ�� */
	writeChar(&ptr, header.byte);
	/*д��ʣ�೤��*/
	ptr += MQTTPacket_encode(ptr, rem_len); /* write remaining length */;
	/*д�뱨�ı�ʶ��*/
	writeInt(&ptr, packetid);
	/*д������������Լ���qos�ȼ�������ɶԳ���*/
	for (i = 0; i < count; ++i)
	{
		writeMQTTString(&ptr, topicFilters[i]);
		writeChar(&ptr, requestedQoSs[i]);
	}
	/*���س���*/
	rc = ptr - MQTT_buf;
	/*���ȳ��������������˳���*/
exit:
	FUNC_EXIT_RC(rc);
	return rc;
}





/********************************************************
����: MQTTDeserialize_suback
���ܣMSUBACK ���Ľ��
������
			packetid								//���ı�ʶ��
			maxcount								//����������
			count									 //�������
			grantedQoSs						 //�����qos����
			buf                    //���ջ���
			buflen                 //���ջ��峤��
����ֵ��rc        ���󷵻��룬��ʱδ����ʹ��
********************************************************/


int MQTTDeserialize_suback(unsigned short* packetid, int maxcount, int* count, int grantedQoSs[], unsigned char* buf, int buflen)
{
	MQTTHeader header = {0};
	unsigned char* curdata = buf;
	unsigned char* enddata = NULL;
	int rc = 0;
	int mylen;


	header.byte = readChar(&curdata);
	if (header.bits.type != SUBACK)
		goto exit;

	curdata += (rc = MQTTPacket_decodeBuf(curdata, &mylen)); /* read remaining length */
	enddata = curdata + mylen;
	if (enddata - curdata < 2)
		goto exit;

	*packetid = readInt(&curdata);

	*count = 0;
	while (curdata < enddata)
	{
		if (*count > maxcount)
		{
			rc = -1;
			goto exit;
		}
		grantedQoSs[(*count)++] = readChar(&curdata);
	}

	rc = 1;
exit:
	FUNC_EXIT_RC(rc);
	return rc;
}


