#include <MQTTpublish.h>


/********************************************************
函数: MQTT_Publish_length
功能：计算MQTT PUBLISH报文长度
参数：qos      qos等级
			topicName 主题名  UTF-8 字符 
			payloadlen  负载长度
返回值： len   报文总字节数 (除固定头部)
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
函数: MQTT_publish
功能MQTT PUBLISH 报文组包
参数：MQTT_buf                //报文发送缓冲区
			buflen                  //缓冲区长度
			dup                     //循环发送标志  控制报文标志位 bit3位置
			qos											//qos等级	控制报文标志位 bit2、1位置
			retained								//保留标志retained	控制报文标志位 bit0位置
			packetid								//报文标识符
			topicName								//主题名   UTF-8 字符 
			payload									//消息载荷
			payloadlen							//载荷长度
返回值：报文总字节数 (除固定头部)
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
/*************固定报头写入*************/
	header.bits.type = PUBLISH;
	header.bits.dup = dup;
	header.bits.qos = qos;
	header.bits.retain = retained;
	writeChar(&ptr, header.byte); /* write header */
/**************剩余长度写入************/
	ptr += MQTTPacket_encode(ptr, rem_len); /* write remaining length */;
/*************写入主题名***************/
	writeMQTTString(&ptr, topicName);
/*******只在QOS等级大于1时使用报文标识符****/
	
	if (qos > 0)
		writeInt(&ptr, packetid);
/*******写入负载***********/	
	memcpy(ptr, payload, payloadlen);
	ptr += payloadlen;

	rc = ptr - MQTT_buf;

exit:
	FUNC_EXIT_RC(rc);
	return rc;
}

/********************************************************
函数: MQTTpublish_ack
功能MQTTpublish  	pulish包文应答  
参数：MQTT_buf                //报文发送缓冲区
			buflen                  //缓冲区长度
			packettype              //包类型
			dup											//循环发送标志
			packetid								//messageid qos>0时使用
返回值：报文总字节数 (除固定头部)
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
函数: MQTT_puback
功能MQTT_puback  	PUBACK组包
参数：buf                			//报文接收缓冲区
			buflen                  //缓冲区长度
			packetid								//messageid qos>0时使用
返回  MQTTpublish_ack  函数
********************************************************/
int MQTT_puback(unsigned char* buf, int buflen, unsigned short packetid)
{
	return MQTTpublish_ack(buf, buflen, PUBACK, 0, packetid);
}



/*****************QOS=2使用函数    暂时不用*********************/

int MQTTSerialize_pubrel(unsigned char* buf, int buflen, unsigned char dup, unsigned short packetid)
{
	return MQTTpublish_ack(buf, buflen, PUBREL, dup, packetid);
}


int MQTTSerialize_pubcomp(unsigned char* buf, int buflen, unsigned short packetid)
{
	return MQTTpublish_ack(buf, buflen, PUBCOMP, 0, packetid);
}





/********************************************************
函数:MQTTDeserialize_publish
功能 Deserialize  MQTT_publish 	MQTT_publish报文反序列化（解包）
参数：dup                		控制报文标志位 dup
			qos                   控制报文标志位 qos
			retained							控制报文标志位 retained
			packetid							publish报文的message id
			topicName							publish报文的主题名称
			payload								有效载荷
			payloadlen						有效载荷长度
			buf										接收缓冲区
			buflen								接收缓冲区长度
返回  rc  函数
********************************************************/

int MQTTDeserialize_publish(unsigned char* dup, int* qos, unsigned char* retained, unsigned short* packetid, MQTTString* topicName,
		unsigned char** payload, int* payloadlen, unsigned char* buf, int buflen)
{
	MQTTHeader header = {0};
	unsigned char* curdata = buf; //读取指针
	unsigned char* enddata = NULL;
	int rc = 0;
	int mylen = 0;

	header.byte = readChar(&curdata);
	if (header.bits.type != PUBLISH)
		goto exit;
	*dup = header.bits.dup;
	*qos = header.bits.qos;
	*retained = header.bits.retain;

	curdata += (rc = MQTTPacket_decodeBuf(curdata, &mylen)); /* 读取剩余长度 */
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



