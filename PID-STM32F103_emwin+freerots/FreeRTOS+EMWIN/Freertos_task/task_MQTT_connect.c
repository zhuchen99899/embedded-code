#include "task_MQTT_Connect.h"

#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"
#include "semphr.h"
/**********�ź���**********/
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET���Ķ�ֵ�ź������

/******MQTTconncet�ṹ��************/
MQTTPacket_connectData SETCONNECT;




/********************WIFI����CONNCET����*****************/

void MQTT_Connect_task(void *pvParameters)
{
BaseType_t err=pdFALSE;
 u8 temp_buff[256];
int len;
vTaskDelay(20000);
conncect_init(); //MQTTconncet ��ͷ��ʼ������
	while(1)
{	
	
	err=xSemaphoreTake(BinarySemaphore_MQTTconnect,portMAX_DELAY);	//��ȡCONNECT�����ź���
			if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{		
			printf("WIFI���ڷ���CONNECT����\r\n");
			len=MQTT_Connect(temp_buff,256,&SETCONNECT);
			WIFI_send(temp_buff,len);

					/*******��ӡwifi����*******/
		/*
		for (i=0;i<len;i++)
		{
		printf("%02x",temp_buff[i]);
	
		}
	*/
			}




	

	
      vTaskDelay(15000);                           //��ʱ15s

}

}


