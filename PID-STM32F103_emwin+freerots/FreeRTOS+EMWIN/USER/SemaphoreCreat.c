#include "SemaphoreCreat.h"
#include "freertos.h"
#include "semphr.h"
extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET���Ķ�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_WIFI_PIDSET;//MQTT SUBSCRIBE���Ķ�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_WIFI_TEMSET;//MQTT SUBSCRIBE���Ķ�ֵ�ź������
/**************��ֵ�ź�������****************/

void BinarySemaphoreCreate(void)
{
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
	BinarySemaphore_MQTTconnect=xSemaphoreCreateBinary();
	BinarySemaphore_MQTTsubscribe=xSemaphoreCreateBinary();

};








/****************�ź�������*****************/
void SempaphoreCreate(void)
{

BinarySemaphoreCreate();
};


