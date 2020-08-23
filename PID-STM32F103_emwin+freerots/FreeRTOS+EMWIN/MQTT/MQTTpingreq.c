#include <MQTTpingreq.h>
#include <MQTTpacket.h>

int MQTT_pingreq(unsigned char* MQTT_buf)
{
unsigned char *ptr = MQTT_buf;

	MQTTHeader header = {0};
	header.byte = 0; //固定头部 所有位置0
	header.bits.type = PINGREQ;
	writeChar(&ptr, header.byte); /* 写入固定头 */
	
	/*****PINGREG剩余长度固定为0x00***********/
	MQTT_buf[1]= 0x00; /*写入剩余长度 */

return 2;//返回缓冲区长度2
};



