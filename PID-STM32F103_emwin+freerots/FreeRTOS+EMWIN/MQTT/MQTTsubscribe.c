#include <MQTTsubscribe.h>
#include <MQTTpacket.h>
#include <MQTTunpacket.h>
#include <stdio.h>

/********************************************************
函数: MQTTS_subscribeLength
功能：计算MQTT SUBSCRIBE报文总长度
参数：count									 //主题计数
			topicFilters[]         //主题过滤器
返回值： len 报文总字节数 (除固定头部)
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
函数: MQTT_subscribe
功能MQTT SUBSCRIBE 报文组包
参数：MQTT_buf                //报文发送缓冲区
			buflen                  //缓冲区长度
			dup                     //循环发送标志  控制报文标志位 bit3位置
			packetid								//报文标识符
			count									 //主题计数
			topicFilters[]         //主题过滤器
			requestedQoSs[]				 //Qos等级
返回值：报文总字节数 (除固定头部)
********************************************************/
int MQTT_subscribe(unsigned char* MQTT_buf, int buflen, unsigned char dup, unsigned short packetid, int count,
		MQTTString topicFilters[], int requestedQoSs[])
{
	unsigned char *ptr = MQTT_buf;
	
	MQTTHeader header = {0};//固定头部 所有位置0
	int rem_len = 0;
	int rc = 0;
	int i = 0;


	if (MQTTPacket_len(rem_len = MQTTS_subscribeLength(count, topicFilters)) > buflen)
	{
		rc = MQTTPACKET_BUFFER_TOO_SHORT;
		goto exit;
	}

	header.byte = 0;      //固定头首字节初始化
	header.bits.type = SUBSCRIBE;  //控制报文类型
	header.bits.dup = dup;          //控制报文标志位，SUBSCRIBE固定bit4为0
	header.bits.qos = 1;						//控制报文标志位，SUBSCRIBE固定bit1位为1
	/* 写入固定头部 */
	writeChar(&ptr, header.byte);
	/*写入剩余长度*/
	ptr += MQTTPacket_encode(ptr, rem_len); /* write remaining length */;
	/*写入报文标识符*/
	writeInt(&ptr, packetid);
	/*写入主题过滤器以及其qos等级，必须成对出现*/
	for (i = 0; i < count; ++i)
	{
		writeMQTTString(&ptr, topicFilters[i]);
		writeChar(&ptr, requestedQoSs[i]);
	}
	/*返回长度*/
	rc = ptr - MQTT_buf;
	/*长度超出缓冲区长度退出口*/
exit:
	FUNC_EXIT_RC(rc);
	return rc;
}





/********************************************************
函数: MQTTDeserialize_suback
功能SUBACK 报文解包
参数：
			packetid								//报文标识符
			maxcount								//最大主题计数
			count									 //主题计数
			grantedQoSs						 //允许的qos返回
			buf                    //接收缓冲
			buflen                 //接收缓冲长度
返回值：rc        错误返回码，暂时未定义使用
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


