#include "task_MQTT_Connect.h"
#include "freertos.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"
#include "semphr.h"

/******MQTTsubscribe�ṹ��************/
MQTTString SETSUBSCRIBE[subscribe_count];
int QoSs[subscribe_count];	
/**********�ź���**********/
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������

/********************WIFI����SUBSCRIBE����*****************/

void MQTT_Subscribe_task(void *pvParameters)
{
int i;
BaseType_t err=pdFALSE;
 u8 temp_buff[256];
int len;
subscribe_init(); //MQTTsubscribe ��ͷ��ʼ������
	while(1)
{	
	
	err=xSemaphoreTake(BinarySemaphore_MQTTsubscribe,portMAX_DELAY);	//��ȡSUBSCRIBE�����ź���
			if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{		
			printf("WIFI���ڷ���Subscribe����\r\n");
			len=MQTT_subscribe(temp_buff,256,subscribe_header_dup,subscribe_packetid,subscribe_count,SETSUBSCRIBE,QoSs);
			WIFI_send(temp_buff,len);

			//printf("%d",len);
					/*******��ӡwifi����*******/
		
		for (i=0;i<len;i++)
		{
		printf("%02x",temp_buff[i]);
	
		}
		
		printf("Subscribe���ķ������\r\n");
			

			}


	

	
      vTaskDelay(15000);                           //��ʱ15s

}

}



