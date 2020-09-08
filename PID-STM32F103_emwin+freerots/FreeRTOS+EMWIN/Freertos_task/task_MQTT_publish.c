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
	//Cjson
	/*
	cJSON *json = NULL;
	cJSON *array = NULL;
	cJSON *obj = NULL;	
	char *json_buf;
	*/
u8 PUBLISH_FLAG=0;
publish_init();
	

	
	while(1)
{	
	xQueuePeek(PUBLISH_Queue,&PUBLISH_FLAG,portMAX_DELAY);
	
	//��ȡPUBLISH���Ķ���
	if(PUBLISH_FLAG)
	{
			
			printf("WIFI���ڷ���Publish����\r\n");
		
		/**********Cjson����ADC����****************/
			xQueuePeek(Adc_Queue,&adcmsg,portMAX_DELAY);
			sprintf(stradc,"%f",adcmsg);
		/*
		//����Cjson����
			json = cJSON_CreateObject();                                  
			cJSON_AddStringToObject(json, "sensor", "adc1");
		//����һ��������
	array = cJSON_CreateArray();                         
	cJSON_AddItemToObject(json, "float number", array);         
		//����һ����������������������Ŀ
		obj = cJSON_CreateObject();                         
		cJSON_AddItemToArray(array, obj);                                
		cJSON_AddItemToObject(obj, "tempreture",cJSON_CreateString(stradc));   
		//JSON���ݴ��뻺����
	 json_buf = cJSON_Print(json);  
		
		*/
			len=MQTT_publish(temp_buff,256,publish1_header_dup,publish1_qos,publish1_retained,publish1_packetid,SETPUBLISH,(unsigned char *)stradc,strlen(stradc));
			
		
		 WIFI_send(temp_buff,len);

/*
					
		
		for (i=0;i<len;i++)
		{
		printf("%02x",temp_buff[i]);
	
		}
		printf("PUBLISH���ķ������");
		
*/
//printf("%s",json_buf);

	//cJSON_Delete(json); 
	}		


      vTaskDelay(15000);                           //��ʱ15s

}

}



