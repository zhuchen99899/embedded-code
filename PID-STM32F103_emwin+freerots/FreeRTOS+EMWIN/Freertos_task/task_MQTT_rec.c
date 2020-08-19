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
	/****��Ϣ���в���**************/
	extern QueueHandle_t Wifi_buffer_Queue;
	extern SemaphoreHandle_t BinarySemaphore;	//��ֵ�ź������
	

	while(1)
	{	
	err=xSemaphoreTake(BinarySemaphore,portMAX_DELAY);	//��ȡ�ź���
	if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{
				
xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
	/*printf("���յ�������Ϊ %02x\r\n ,����Ϊ %d\r\n",&wifireceive->wifi_buffer,wifireceive->wifi_lenth);*/
 
for (i=0;i<4;i++)
	{
	printf("%02x",(wifireceive->wifi_buffer)[i]);
	
	}
				
			}
   vTaskDelay(30);                           //��ʱ3000ms
	}

}
