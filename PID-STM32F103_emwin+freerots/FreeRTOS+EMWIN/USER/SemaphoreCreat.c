#include "SemaphoreCreat.h"
#include "freertos.h"
#include "semphr.h"
extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET���Ķ�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_WIFI_PIDSET;//MQTT WIFI_PIDSET���Ķ�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_WIFI_TEMSET;//MQTT WIFI_TEMSET���Ķ�ֵ�ź������
/**************��ֵ�ź�������****************/

void BinarySemaphoreCreate(void)
{
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
	BinarySemaphore_MQTTconnect=xSemaphoreCreateBinary();
	BinarySemaphore_MQTTsubscribe=xSemaphoreCreateBinary();
	BinarySemaphore_WIFI_PIDSET=xSemaphoreCreateBinary();
	BinarySemaphore_WIFI_TEMSET=xSemaphoreCreateBinary();
};








/****************�ź�������*****************/
void SempaphoreCreate(void)
{

BinarySemaphoreCreate();
};


