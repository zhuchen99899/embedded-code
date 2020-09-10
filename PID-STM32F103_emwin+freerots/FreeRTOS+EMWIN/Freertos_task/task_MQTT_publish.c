#include "task_MQTT_publish.h"
#include "freertos.h"
#include "task.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"
#include "semphr.h"
/************消息队列句柄***********/
extern QueueHandle_t PUBLISH_Queue;
extern QueueHandle_t Adc_Queue;

/*************全局变量**************/

MQTTString SETPUBLISH; //PUBLISH封包结构体

void MQTT_Publish_task(void *pvParameters)
{
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
	
	//获取PUBLISH报文队列
	if(PUBLISH_FLAG)
	{
			
			printf("WIFI正在发送Publish报文\r\n");
		
			xQueuePeek(Adc_Queue,&adcmsg,portMAX_DELAY);
		
			sprintf(stradc,"%f",adcmsg);
		
			json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json,"tempreture",adcmsg);

  	//JSON数据传入缓冲区

   json_buf = cJSON_Print(json);

  
		/*  创建完整Json树方法
		//转换adc数据
		sprintf(stradc,"%f",adcmsg);
		
		//创建Cjson对象
			json = cJSON_CreateObject();                                  
			cJSON_AddStringToObject(json, "sensor", "adc1");
		//创建一个空数组
	array = cJSON_CreateArray();                         
	cJSON_AddItemToObject(json, "float number", array);         
		//创建一个数组对象并向数组中添加条目
		obj = cJSON_CreateObject();                         
		cJSON_AddItemToArray(array, obj);                                
		cJSON_AddItemToObject(obj, "tempreture",cJSON_CreateString(stradc));   
		//JSON数据传入缓冲区
	 json_buf = cJSON_Print(json);  
	*/	
		
		len=MQTT_publish(temp_buff,256,publish1_header_dup,publish1_qos,publish1_retained,publish1_packetid,SETPUBLISH,(unsigned char *)json_buf,strlen(json_buf));
			
		
		WIFI_send(temp_buff,len);

    vPortFree(json_buf);
					/*******打印wifi发送*******/
		/*
		for (i=0;i<len;i++)
		{
		printf("%02x",temp_buff[i]);
	
		}
		printf("PUBLISH报文发送完成");
			*/
	
printf(" 发布任务申请后内存剩余量 = %d\r\n", xPortGetFreeHeapSize());
	
	
printf("%s",json_buf);
	
	cJSON_Delete(json); 
	

		}	
	
      vTaskDelay(15000);                           //延时15s

}

}



