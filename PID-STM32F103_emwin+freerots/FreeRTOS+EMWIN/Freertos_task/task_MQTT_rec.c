#include "task_MQTT_rec.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"

typedef struct wifibuff
{
u32 wifi_lenth;
u8 wifi_buffer[256];
}wifibuff;

wifibuff *wifireceive;
extern
int i;
void MQTT_rec_task(void *pvParameters)
{
BaseType_t err=pdFALSE;
	/****消息队列参数**************/
	extern QueueHandle_t Wifi_buffer_Queue;
	extern SemaphoreHandle_t BinarySemaphore;	//二值信号量句柄
	

	while(1)
	{	
	err=xSemaphoreTake(BinarySemaphore,portMAX_DELAY);	//获取信号量
	if(err==pdTRUE)										//获取信号量成功
			{
				
xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
	/*printf("接收到的数据为 %02x\r\n ,长度为 %d\r\n",&wifireceive->wifi_buffer,wifireceive->wifi_lenth);*/
 
for (i=0;i<4;i++)
	{
	printf("%02x",(wifireceive->wifi_buffer)[i]);
	
	}
				
			}
   vTaskDelay(30);                           //延时3000ms
	}

}
