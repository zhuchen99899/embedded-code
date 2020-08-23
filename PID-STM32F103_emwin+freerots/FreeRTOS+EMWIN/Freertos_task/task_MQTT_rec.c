#include "task_MQTT_rec.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"

/*	WIFI���յ������
for (i=0;i<4;i++)
	{
	printf("%02x",(wifireceive->wifi_buffer)[i]);
	
	}
				*/

/**********��Ϣ���в���**************/

typedef struct wifibuff
{
u32 wifi_lenth;
u8 wifi_buffer[256];
}wifibuff;

/***************�ź�������************/
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//��ֵ�ź������
	/****��Ϣ���в���**************/
	extern QueueHandle_t Wifi_buffer_Queue;
	extern QueueHandle_t PINGREQ_Queue;

void MQTT_rec_task(void *pvParameters)
{
	//int i;

	/***************�ź�������************/
	BaseType_t err=pdFALSE;
	/****��Ϣ���в���**************/
	wifibuff *wifireceive;
	u8 PINGREQ_FLAG;
	while(1)
	{	
	err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//��ȡ����2�����ж϶�ֵ�ź���
	if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{
				
xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
	/*printf("���յ�������Ϊ %02x\r\n ,����Ϊ %d\r\n",&wifireceive->wifi_buffer,wifireceive->wifi_lenth);*/
 
				/************************CONNACT���ļ��**************************************************/
				
		if((wifireceive->wifi_buffer[0]==0x20)&&(wifireceive->wifi_buffer[1]==0x02)&&(wifireceive->wifi_buffer[2]==0x00))    //����յ�4���ֽڣ��ҵ�һ���ֽ���0x20����ʾ��connect ack���ģ�����if
		{
			switch(wifireceive->wifi_buffer[3])	                                                     //�ж��յ��ĵ�4���ֽ�
			{					
				case 0x00 : printf("CONNECT���ĳɹ�\r\n");
										PINGREQ_FLAG=1;   //��ʼ����PINGREQ���ı�־
										xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);
										/************SUBSCRIBE,publish*************/
							break;
				case 0x01 : printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");
				/***************E1��������*********************/		
						PINGREQ_FLAG=0;   //�رշ���PINGREQ���ı�־
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);		
						break;
				case 0x02 : printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");
				/***************E2��������*********************/	
						PINGREQ_FLAG=0;   //�رշ���PINGREQ���ı�־
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);		
						break;
				case 0x03 : printf("�����Ѿܾ�������˲����ã�׼������\r\n");
				/***************E3��������*********************/	
						PINGREQ_FLAG=0;   //�رշ���PINGREQ���ı�־
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);		
						break;
				case 0x04 : printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");
				/***************E4��������*********************/	
						PINGREQ_FLAG=0;   //�رշ���PINGREQ���ı�־
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);		
						break;
				case 0x05 : printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");
				/***************E5��������*********************/	
						PINGREQ_FLAG=0;   //�رշ���PINGREQ���ı�־
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);	
						break;
				default   : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");
				/***************E6��������*********************/	
						PINGREQ_FLAG=0;   //�رշ���PINGREQ���ı�־
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);	
				    break;
		
			}				
		}
		
		/*************��������******************************/
		else if((wifireceive->wifi_buffer[0]==0xD0)&&(wifireceive->wifi_buffer[1]==0x00)) 
		{
		
		printf("�ѽ��շ�����PINGRESP����\r\n");
		
		}

		
			}//WIFI���ڽ����ź���
			
			/************����δ�յ��κη���������(�ź�����������)***********************/
			
		//else{
			/************************
			����Freertos�����ʱ�� ���ڶ�ʱ ��ʱ��ΪMQTTconfig.h����ֵ��keepAlive_Interval 
			��ʱ���ص���������ʱ����
			**************/
			
			
			
			//	}
			
			
			
   vTaskDelay(30);                           //��ʱ3000ms
	}

}
