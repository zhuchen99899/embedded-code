#include <MQTTpingreq.h>
#include <MQTTpacket.h>

int MQTT_pingreq(unsigned char* MQTT_buf)
{
unsigned char *ptr = MQTT_buf;

	MQTTHeader header = {0};
	header.byte = 0; //�̶�ͷ�� ����λ��0
	header.bits.type = PINGREQ;
	writeChar(&ptr, header.byte); /* д��̶�ͷ */
	
	/*****PINGREGʣ�೤�ȹ̶�Ϊ0x00***********/
	MQTT_buf[1]= 0x00; /*д��ʣ�೤�� */

return 2;//���ػ���������2
};



