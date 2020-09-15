#include "task_MQTT_publish.h"
#include "freertos.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"
#include "semphr.h"
#include <EventGroupCreat.h>
#include "event_groups.h"

/****************�¼���־����*************************/
extern EventGroupHandle_t EventGroupHandler;	//�¼���־����
/************��Ϣ���о��***********/
extern QueueHandle_t PUBLISH_Queue;
extern QueueHandle_t Adc_Queue;
extern QueueHandle_t PUBLISH_ShowHeap_Queue;
/*************ȫ�ֱ���**************/

MQTTString SETPUBLISH; //PUBLISH����ṹ��

void MQTT_Publish_task(void *pvParameters)
{
	size_t ShowPulishHeap;
	
//	int i;
 u8 temp_buff[256];
int len;
char stradc[50];
float adcmsg; 
	//Cjson
	
	cJSON *json = NULL;
	//cJSON *array = NULL;
	//cJSON *obj = NULL;	
	char *json_buf;
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
		
			json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json,"tempreture",adcmsg);

  	//JSON���ݴ��뻺����

   json_buf = cJSON_Print(json);

  
		/*  ��������Json������
		//ת��adc����
		sprintf(stradc,"%f",adcmsg);
		
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
		
		//publish �������л�
		len=MQTT_publish(temp_buff,256,publish1_header_dup,publish1_qos,publish1_retained,publish1_packetid,SETPUBLISH,(unsigned char *)json_buf,strlen(json_buf));
		//���ͱ���
		WIFI_send(temp_buff,len);


printf("%s",json_buf); //��ӡjson����
	
	cJSON_Delete(json); //�ͷ�json
	
	 vPortFree(json_buf);//�ͷ������json����
	
//����ʣ��Ѹ�emwin
ShowPulishHeap=xPortGetFreeHeapSize();
printf(" ��������������ڴ�ʣ���� = %d\r\n", ShowPulishHeap);
xQueueOverwrite(PUBLISH_ShowHeap_Queue,&ShowPulishHeap);	
	
//�¼���־��ι��	
	xEventGroupSetBits(EventGroupHandler,EVENTBIT_4);

		}	
	
      vTaskDelay(15000);                           //��ʱ15s

}

}



