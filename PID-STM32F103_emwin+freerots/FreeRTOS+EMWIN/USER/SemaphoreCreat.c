#include "SemaphoreCreat.h"
#include "freertos.h"
#include "semphr.h"
extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET���Ķ�ֵ�ź������

/**************��ֵ�ź�������****************/

void BinarySemaphoreCreate(void)
{
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
	BinarySemaphore_MQTTconnect=xSemaphoreCreateBinary();

};








/****************�ź�������*****************/
void SempaphoreCreate(void)
{

BinarySemaphoreCreate();
};


