#include <MQTTpublish.h>


/********************************************************
����: MQTT_Publish_length
���ܣ�����MQTT PUBLISH���ĳ���
������qos      qos�ȼ�
			topicName ������  UTF-8 �ַ� 
			payloadlen  ���س���
����ֵ�� len   �������ֽ��� (���̶�ͷ��)
********************************************************/


int MQTT_Publish_length(int qos, MQTTString topicName, int payloadlen)
{
	int len = 0;

	len += 2 + MQTTstrlen(topicName) + payloadlen;
	if (qos > 0)
		len += 2; /* packetid */
	return len;
}

/********************************************************
����: MQTT_publish
���ܣMMQTT PUBLISH �������
������MQTT_buf                //���ķ��ͻ�����
			buflen                  //����������
			dup                     //ѭ�����ͱ�־  ���Ʊ��ı�־λ bit3λ��
			qos											//qos�ȼ�	���Ʊ��ı�־λ bit2��1λ��
			retained								//������־retained	���Ʊ��ı�־λ bit0λ��
			packetid								//���ı�ʶ��
			topicName								//������   UTF-8 �ַ� 
			payload									//��Ϣ�غ�
			payloadlen							//�غɳ���
����ֵ���������ֽ��� (���̶�ͷ��)
********************************************************/

int MQTT_publish(unsigned char* MQTT_buf, int buflen, unsigned char dup, int qos, unsigned char retained, unsigned short packetid,
		MQTTString topicName, unsigned char* payload, int payloadlen)
{
	unsigned char *ptr = MQTT_buf;
	MQTTHeader header = {0};
	int rem_len = 0;
	int rc = 0;

	if (MQTTPacket_len(rem_len = MQTT_Publish_length(qos, topicName, payloadlen)) > buflen)
	{
		rc = MQTTPACKET_BUFFER_TOO_SHORT;
		goto exit;
	}
/*************�̶���ͷд��*************/
	header.bits.type = PUBLISH;
	header.bits.dup = dup;
	header.bits.qos = qos;
	header.bits.retain = retained;
	writeChar(&ptr, header.byte); /* write header */
/**************ʣ�೤��д��************/
	ptr += MQTTPacket_encode(ptr, rem_len); /* write remaining length */;
/*************д��������***************/
	writeMQTTString(&ptr, topicName);
/*******ֻ��QOS�ȼ�����1ʱʹ�ñ��ı�ʶ��****/
	
	if (qos > 0)
		writeInt(&ptr, packetid);
/*******д�븺��***********/	
	memcpy(ptr, payload, payloadlen);
	ptr += payloadlen;

	rc = ptr - MQTT_buf;

exit:
	FUNC_EXIT_RC(rc);
	return rc;
}

/********************************************************
����: MQTTpublish_ack
���ܣMMQTTpublish  	pulish����Ӧ��  
������MQTT_buf                //���ķ��ͻ�����
			buflen                  //����������
			packettype              //������
			dup											//ѭ�����ͱ�־
			packetid								//messageid qos>0ʱʹ��
����ֵ���������ֽ��� (���̶�ͷ��)
********************************************************/

int MQTTpublish_ack(unsigned char* buf, int buflen, unsigned char packettype, unsigned char dup, unsigned short packetid)
{
	MQTTHeader header = {0};
	int rc = 0;
	unsigned char *ptr = buf;

	if (buflen < 4)
	{
		rc = MQTTPACKET_BUFFER_TOO_SHORT;
		goto exit;
	}
	
	header.bits.type = packettype;
	header.bits.dup = dup;
	
	header.bits.qos = (packettype == PUBREL) ? 1 : 0;
	writeChar(&ptr, header.byte); /* write header */

	ptr += MQTTPacket_encode(ptr, 2); /* write remaining length */
	writeInt(&ptr, packetid);
	rc = ptr - buf;
exit:
	FUNC_EXIT_RC(rc);
	return rc;
}

/********************************************************
����: MQTT_puback
���ܣMMQTT_puback  	PUBACK���
������buf                			//���Ľ��ջ�����
			buflen                  //����������
			packetid								//messageid qos>0ʱʹ��
����  MQTTpublish_ack  ����
********************************************************/
int MQTT_puback(unsigned char* buf, int buflen, unsigned short packetid)
{
	return MQTTpublish_ack(buf, buflen, PUBACK, 0, packetid);
}



/*****************QOS=2ʹ�ú���    ��ʱ����*********************/

int MQTTSerialize_pubrel(unsigned char* buf, int buflen, unsigned char dup, unsigned short packetid)
{
	return MQTTpublish_ack(buf, buflen, PUBREL, dup, packetid);
}


int MQTTSerialize_pubcomp(unsigned char* buf, int buflen, unsigned short packetid)
{
	return MQTTpublish_ack(buf, buflen, PUBCOMP, 0, packetid);
}





/********************************************************
����:MQTTDeserialize_publish
���� Deserialize  MQTT_publish 	MQTT_publish���ķ����л��������
������dup                		���Ʊ��ı�־λ dup
			qos                   ���Ʊ��ı�־λ qos
			retained							���Ʊ��ı�־λ retained
			packetid							publish���ĵ�message id
			topicName							publish���ĵ���������
			payload								��Ч�غ�
			payloadlen						��Ч�غɳ���
			buf										���ջ�����
			buflen								���ջ���������
����  rc  ����
********************************************************/

int MQTTDeserialize_publish(unsigned char* dup, int* qos, unsigned char* retained, unsigned short* packetid, MQTTString* topicName,
		unsigned char** payload, int* payloadlen, unsigned char* buf, int buflen)
{
	MQTTHeader header = {0};
	unsigned char* curdata = buf; //��ȡָ��
	unsigned char* enddata = NULL;
	int rc = 0;
	int mylen = 0;

	header.byte = readChar(&curdata);
	if (header.bits.type != PUBLISH)
		goto exit;
	*dup = header.bits.dup;
	*qos = header.bits.qos;
	*retained = header.bits.retain;

	curdata += (rc = MQTTPacket_decodeBuf(curdata, &mylen)); /* ��ȡʣ�೤�� */
	enddata = curdata + mylen;

	if (!readMQTTLenString(topicName, &curdata, enddata) ||
		enddata - curdata < 0) /* do we have enough data to read the protocol version byte? */
		goto exit;

	if (*qos > 0)
		*packetid = readInt(&curdata);

	*payloadlen = enddata - curdata;
	*payload = curdata;
	rc = 1;
exit:
	FUNC_EXIT_RC(rc);
	return rc;
}



