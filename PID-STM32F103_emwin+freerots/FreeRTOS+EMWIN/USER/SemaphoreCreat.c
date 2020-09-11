#include "SemaphoreCreat.h"
#include "freertos.h"
#include "semphr.h"
extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2空闲中断二值信号量句柄
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET报文二值信号量句柄
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE报文二值信号量句柄
extern SemaphoreHandle_t BinarySemaphore_WIFI_PIDSET;//MQTT WIFI_PIDSET报文二值信号量句柄
extern SemaphoreHandle_t BinarySemaphore_WIFI_TEMSET;//MQTT WIFI_TEMSET报文二值信号量句柄
/**************二值信号量创建****************/

void BinarySemaphoreCreate(void)
{
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
	BinarySemaphore_MQTTconnect=xSemaphoreCreateBinary();
	BinarySemaphore_MQTTsubscribe=xSemaphoreCreateBinary();
	BinarySemaphore_WIFI_PIDSET=xSemaphoreCreateBinary();
	BinarySemaphore_WIFI_TEMSET=xSemaphoreCreateBinary();
};








/****************信号量创建*****************/
void SempaphoreCreate(void)
{

BinarySemaphoreCreate();
};


