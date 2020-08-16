#include "task_MQTT_send.h"

#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"


extern QueueHandle_t Adc_Queue;
/********************WIFI发送任务*****************/

void WIFI_task(void *pvParameters)
{
float adcmsg;
char adcmsgstring[10] ;
	/****消息队列参数**************/
//	extern QueueHandle_t Wifi_buffer_Queue;
//	extern QueueHandle_t Wifi_lenth_Queue;
//		int length;
//	u8 showbuffer[256];
	

	while(1)
{	
	xQueuePeek(Adc_Queue,&adcmsg,portMAX_DELAY);
	printf("WIFI任务正在运行\r\n ");
	sprintf((char*)adcmsgstring,"%f",adcmsg);
	WIFI_send((u8*)adcmsgstring,sizeof(adcmsgstring));
		
	/*
	

	printf("接收:%s",showbuffer[256]);
	*/
	
      vTaskDelay(5000);                           //延时3000ms

}

}


