#include "task_MQTT_Pingreq.h"

#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"


/********************WIFI����PINGREQ����*****************/
	extern QueueHandle_t PINGREQ_Queue;
	
void MQTT_Pingreq_task(void *pvParameters)
{
u8 PINGREQ_FLAG=0;
u8 temp_buff[256];
int len;
int i;



	while(1)
	{	
	 xQueuePeek(PINGREQ_Queue,&PINGREQ_FLAG,portMAX_DELAY);
	if (PINGREQ_FLAG)
		{
			
			printf("WIFI���ڷ���PINGREQ����\r\n");
			len=MQTT_pingreq(temp_buff);
			WIFI_send(temp_buff,len);
			
		/*******��ӡwifi����*******/
		
		for (i=0;i<len;i++)
		{
		printf("%02x",temp_buff[i]);
	
		}
		printf("����PINGREQ�������\r\n");
		}



	
      vTaskDelay(50000);                           //��ʱ50s
	}//while
};// pingreq_task


