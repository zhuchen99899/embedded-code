#include "MQTTpacket.h"
#include "MQTTconnect.h"

/********************************************************
函数: MQTT_Connect_lenth
功能：计算MQTT CONNECT报文长度
参数：temp_buff[256]  //MQTT发送报文缓冲
			options         //MQTTconnect配设结构体
返回值： len   报文总字节数 (除固定头部)
********************************************************/


int MQTT_Connect_lenth(MQTTPacket_connectData* options)
{
	int len = 0;
/*****************不算固定报头以及可变报头部分字节数，通过MQTTPacket_len函数判定剩余字节字节数后加入，并计算完整包长*************/	
	
	
	
	
	
	/*************可变头=协议名+协议级别+连接标志(一个字节),保持时间(两个字节)****************/
 /* 判断是MQlsdp还是MQTT协议版本 */
	if (options->MQTTVersion == 3)
		len = 12;
	else if (options->MQTTVersion == 4)
		len = 10;
	
	
	/***********有效载荷*************/
	/*cilentID*/
	  len += MQTTstrlen(options->clientID)+2;
	/*如果启用遗嘱标志*/
	if (options->willFlag)
		/*遗嘱主题和遗嘱消息*/
		len += MQTTstrlen(options->will.topicName)+2 + MQTTstrlen(options->will.message)+2;
	/*如果启用用户名标志*/
	if (options->username.string )
		len += MQTTstrlen(options->username)+2;
	/*如果启用密码标志*/
	if (options->password.string )
		len += MQTTstrlen(options->password)+2;
	return len;
};


/********************************************************
函数: MQTT_Connect
功能：MQTT CONNECT 报文拆包
参数：MQTT_buf  报文发送缓冲区
			buflen 缓冲区长度         
			options         //MQTTconnect配设结构体
返回值 rc  (报文长度)
********************************************************/



int MQTT_Connect(unsigned char* MQTT_buf, int buflen, MQTTPacket_connectData* options)
{
unsigned char *ptr = MQTT_buf;
	
	int len = 0;
	int rc = -1;
	MQTTHeader header = {0};//固定头部 所有位置0
	MQTTConnectFlags flags = {0}; //连接标志所有位置0
	
	/****************若剩余字节数超出缓冲区*******************************************/
	if (MQTTPacket_len(len = MQTT_Connect_lenth(options)) > buflen)
	{
		rc = MQTTPACKET_BUFFER_TOO_SHORT;
		goto exit;
	}

	
	/******写入固定头部*********/
	header.byte = 0;
	header.bits.type = CONNECT;
	writeChar(&ptr, header.byte); /* 写入固定头 */
	ptr += MQTTPacket_encode(ptr, len); /*写入剩余长度 */
/**************写入可变头*****************/	
	/****根据MQTT版本写入协议，版本号****/
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
	/********写入连接标*********/
	flags.all = 0; //初始化所有连接标致，置0
	flags.bits.cleansession = options->cleansession; //赋值cleansession标志
	flags.bits.will = (options->willFlag) ? 1 : 0; //willflag为真取1，否则为0
	 //如果启用遗嘱标注
	if (flags.bits.will)
	{
		//配置 QOS retain 标志
		flags.bits.willQoS = options->will.qos;
		flags.bits.willRetain = options->will.retained;
	}

	//如果启用用户名标志
	if (options->username.string )
		flags.bits.username = 1;
	//如果启用密码标志
	if (options->password.string )
		flags.bits.password = 1;
	//写入连接标志
	writeChar(&ptr, flags.all);
	
	/*************写入保持时间*****************/
	writeInt(&ptr, options->keepAliveInterval);
	
	/***********写入负载**********/
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

	
	//包长度

	rc = ptr - MQTT_buf;
	

	exit:
	FUNC_EXIT_RC(rc);
	return rc; //返回长度
};


