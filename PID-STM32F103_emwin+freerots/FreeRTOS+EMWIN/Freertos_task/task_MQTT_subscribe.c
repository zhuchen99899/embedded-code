#include "task_MQTT_Connect.h"
#include "freertos.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"
#include "semphr.h"
#include "timers.h"




/******MQTTsubscribe结构体************/
MQTTString SETSUBSCRIBE[subscribe_count];
int QoSs[subscribe_count];	

/************Freertos软件定时器句柄********************/
//周期性软件定时器
extern TimerHandle_t   AutoReloadTimer_For_MqttSubscribeErr_Handle;

/**********信号量**********/
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE报文二值信号量句柄

/********************WIFI发送SUBSCRIBE任务*****************/

void MQTT_Subscribe_task(void *pvParameters)
{
int i;
BaseType_t err=pdFALSE;
 u8 temp_buff[256]; //可以使用packet包中函数获取长度， 使用malloc创建动态内存
int len;
subscribe_init(); //MQTTsubscribe 报头初始化设置
	while(1)
{	
	
	err=xSemaphoreTake(BinarySemaphore_MQTTsubscribe,portMAX_DELAY);	//获取SUBSCRIBE报文信号量
			if(err==pdTRUE)										//获取信号量成功
	{		
			printf("WIFI正在发送Subscribe报文\r\n");
			len=MQTT_subscribe(temp_buff,256,subscribe_header_dup,subscribe_packetid,subscribe_count,SETSUBSCRIBE,QoSs);
			WIFI_send(temp_buff,len);

			//printf("%d",len);
					/*******打印wifi发送*******/
		
		for (i=0;i<len;i++)
		{
		printf("%02x",temp_buff[i]);
	
		}
		
		printf("Subscribe报文发送完成\r\n");
			

		
		xTimerStart(AutoReloadTimer_For_MqttSubscribeErr_Handle,0);	

		printf("开启SUBSCRIBE应答检测定时器");
		
		
	}


	

	
      vTaskDelay(15000);                           //延时15s

}

}



