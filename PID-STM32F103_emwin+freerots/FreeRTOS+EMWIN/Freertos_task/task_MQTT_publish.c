#include "task_MQTT_publish.h"
#include "freertos.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"
#include "semphr.h"
/************消息队列句柄***********/
extern QueueHandle_t PUBLISH_Queue;
extern QueueHandle_t Adc_Queue;

/*************全局变量**************/

MQTTString SETPUBLISH; //PUBLISH封包结构体

void MQTT_Publish_task(void *pvParameters)
{
	int i;
 u8 temp_buff[256];
int len;
char stradc[50];
float adcmsg; 
u8 PUBLISH_FLAG=0;
publish_init();
	while(1)
{	
	xQueuePeek(PUBLISH_Queue,&PUBLISH_FLAG,portMAX_DELAY);
	
	//获取PUBLISH报文队列
	if(PUBLISH_FLAG)
	{
			
			printf("WIFI正在发送Publish报文\r\n");
		
			xQueuePeek(Adc_Queue,&adcmsg,portMAX_DELAY);
		
			sprintf(stradc,"%f",adcmsg);
			len=MQTT_publish(temp_buff,256,publish1_header_dup,publish1_qos,publish1_retained,publish1_packetid,SETPUBLISH,(unsigned char *)stradc,strlen(stradc));
			
		
		WIFI_send(temp_buff,len);


					/*******打印wifi发送*******/
		
		for (i=0;i<len;i++)
		{
		printf("%02x",temp_buff[i]);
	
		}
		printf("PUBLISH报文发送完成");
			
	}		

	
      vTaskDelay(15000);                           //延时15s

}

}



