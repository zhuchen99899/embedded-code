#include "task_MQTT_Connect.h"

#include "freertos.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"
#include "semphr.h"
#include "timers.h"
/************Freertos软件定时器句柄********************/
//周期性软件定时器
extern TimerHandle_t 	AutoReloadTimer_For_MqttConnectErr_Handle;			

/**********信号量**********/
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET报文二值信号量句柄

/******MQTTconncet结构体************/
MQTTPacket_connectData SETCONNECT;




/********************WIFI发送CONNCET任务*****************/

void MQTT_Connect_task(void *pvParameters)
{
BaseType_t err=pdFALSE;
 u8 temp_buff[256];
int len;
int i;
vTaskDelay(20000);
conncect_init(); //MQTTconncet 报头初始化设置
	while(1)
{	
	
	err=xSemaphoreTake(BinarySemaphore_MQTTconnect,portMAX_DELAY);	//获取CONNECT报文信号量
			if(err==pdTRUE)										//获取信号量成功
			{		
			printf("WIFI正在发送CONNECT报文\r\n");
			len=MQTT_Connect(temp_buff,256,&SETCONNECT);
			WIFI_send(temp_buff,len);

					/*******打印wifi发送*******/
		
		for (i=0;i<len;i++)
		{
		printf("%02x",temp_buff[i]);
	
		}
	
			
		printf("CONNECT报文发送完成\r\n");
	
		xTimerStart(AutoReloadTimer_For_MqttConnectErr_Handle,0);	
		printf("开启CONNECT报文应答检测定时器");
			}

	
      vTaskDelay(15000);                           //延时15s

}

}


