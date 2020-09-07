#include "task_MQTT_publish.h"
#include "freertos.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"
#include "semphr.h"
/************��Ϣ���о��***********/
extern QueueHandle_t PUBLISH_Queue;
extern QueueHandle_t Adc_Queue;

/*************ȫ�ֱ���**************/

MQTTString SETPUBLISH; //PUBLISH����ṹ��

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
	
	//��ȡPUBLISH���Ķ���
	if(PUBLISH_FLAG)
	{
			
			printf("WIFI���ڷ���Publish����\r\n");
		
			xQueuePeek(Adc_Queue,&adcmsg,portMAX_DELAY);
		
			sprintf(stradc,"%f",adcmsg);
			len=MQTT_publish(temp_buff,256,publish1_header_dup,publish1_qos,publish1_retained,publish1_packetid,SETPUBLISH,(unsigned char *)stradc,strlen(stradc));
			
		
		WIFI_send(temp_buff,len);


					/*******��ӡwifi����*******/
		
		for (i=0;i<len;i++)
		{
		printf("%02x",temp_buff[i]);
	
		}
		printf("PUBLISH���ķ������");
			
	}		

	
      vTaskDelay(15000);                           //��ʱ15s

}

}



