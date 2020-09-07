#include "task_MQTT_rec.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"


/**********��Ϣ���в���**************/

typedef struct wifibuff
{
u32 wifi_lenth;
u8 wifi_buffer[256];
}wifibuff;

/***************�ź�������************/
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//��ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������
	/****��Ϣ���в���**************/
	extern QueueHandle_t Wifi_buffer_Queue;
	extern QueueHandle_t PINGREQ_Queue;
	extern QueueHandle_t PUBLISH_Queue;

/*************************WIFI����������******************/
void MQTT_rec_task(void *pvParameters)
{
	/**********************PUBLISH�������*********************/
		unsigned char DeserializePublish_dup;
		int DeserializePublish_qos;
		unsigned char DeserializePublish_retained;
		unsigned short DeserializePublish_packetid;
		MQTTString DeserializePublish_topicName;
		unsigned char* DeserializePublish_payload;
		int DeserializePublish_payloadlen;
	
	//PUBLISH�����ж�
		char * topic1=subscrible1_topicFilters1;
	
	
									char topic[256];
									char data[256];									
									
	
/********************����****************************/
	int i;

	/***************�ź�������************/
	BaseType_t err=pdFALSE;
	/****��Ϣ���в���**************/
	wifibuff *wifireceive;
	u8 PINGREQ_FLAG=0;
	u8 PUBLISH_FLAG=0;
	
/*******************�����ȴ�DMA�ж��д�����ISR�ź���***************/
	while(1)
	{	
	err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//��ȡ����2�����ж϶�ֵ�ź���
	if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{
				
xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
	/*printf("���յ�������Ϊ %02x\r\n ,����Ϊ %d\r\n",&wifireceive->wifi_buffer,wifireceive->wifi_lenth);*/
 
				/************************CONNACT���ļ��**************************************************/
			//	���ֽڹ̶�Ϊ0x20  ʣ���ֽڹ̶�Ϊ0x02 �ɱ䱨ͷ����Ϊ ����ȷ�ϱ�־��0x00 �Լ����ӷ�����
		if((wifireceive->wifi_buffer[0]==0x20)&&(wifireceive->wifi_buffer[1]==0x02)&&(wifireceive->wifi_buffer[2]==0x00))    //����յ�4���ֽڣ��ҵ�һ���ֽ���0x20����ʾ��connect ack���ģ�����if
		{
			printf("�ѽ��շ�����CONNECT����\r\n");
			switch(wifireceive->wifi_buffer[3])	                                                     //�ж��յ��ĵ�4���ֽ�
			{					
				case 0x00 : printf("CONNECT���ĳɹ�\r\n");
										PINGREQ_FLAG=1;   //��ʼ����PINGREQ���ı�־
										PUBLISH_FLAG=1;   //��ʼ����PUBLISH���ı�־
										xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);
										vTaskDelay(3000);
										xSemaphoreGive(BinarySemaphore_MQTTsubscribe);
										vTaskDelay(3000);
										xQueueOverwrite(PUBLISH_Queue,&PUBLISH_FLAG);	
										
										/************SUBSCRIBE,publish*************/
							break;
				case 0x01 : printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");
				/***************E1��������*********************/		

						break;
				case 0x02 : printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n");
				/***************E2��������*********************/	

						break;
				case 0x03 : printf("�����Ѿܾ�������˲����ã�׼������\r\n");
				/***************E3��������*********************/	

						break;
				case 0x04 : printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");
				/***************E4��������*********************/	

						break;
				case 0x05 : printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");
				/***************E5��������*********************/	

						break;
				default   : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");
				/***************E6��������*********************/	

				    break;
		
			}				
		}//else if connack����
		
		/*************�յ�PINGRESP����******************************/
		//���ֽڹ̶�Ϊ0xD0 ʣ�೤�ȹ̶�Ϊ 0x00 ���޿ɱ䱨ͷ
		else if((wifireceive->wifi_buffer[0]==0xD0)&&(wifireceive->wifi_buffer[1]==0x00)) 
		{
		
		printf("�ѽ��շ�����PINGRESP����\r\n");
		
		}//else if PINGRESP����
		
		/*****************SUBACK����********************************/
		/*************�������subscribe����Ҫ��wifi_buffer[2]ʣ���ֽ��޸ģ�����һ����1�����������������ж��������ж�����*****/
		else if((wifireceive->wifi_buffer[0]==0x90)&&(wifireceive->wifi_buffer[1]==0x04)) 
		{
				printf("�ѽ��շ�����SUBACK����\r\n");
				if(wifireceive->wifi_buffer[2]==0x00&&wifireceive->wifi_buffer[3]==0x01)			//���յ����ı���1����ȷ��,���ı�ʶ��1
				{
					printf("�ѽ��շ�����SUBACK����,message id=1\r\n");
				switch(wifireceive->wifi_buffer[4])									//�ж��յ��ĵ�5���ֽ�
				{
					case 0x00 : 
								printf("����1���ĳɹ�,qos�ȼ�00\r\n");			//���������Ϣ
								break;
					case 0x01 :
								printf("����1���ĳɹ�,qos�ȼ�01\r\n");			//���������Ϣ
								break;
					case 0x02 :
								printf("����1���ĳɹ�,qos�ȼ�02\r\n");			//���������Ϣ
								break;
					default   : printf("����2����ʧ��\r\n");			//���������Ϣ 
								break;
				}//����1ȷ��
				switch(wifireceive->wifi_buffer[5])
				{
								case 0x00 : 
								printf("����2���ĳɹ�,qos�ȼ�00\r\n");			//���������Ϣ
								break;
					case 0x01 :
								printf("����2���ĳɹ�,qos�ȼ�01\r\n");			//���������Ϣ
								break;
					case 0x02 :
								printf("����2���ĳɹ�,qos�ȼ�02\r\n");			//���������Ϣ
								break;
					default   : printf("����2����ʧ��\r\n");			//���������Ϣ 
								break;	
				}//����2ȷ��
				
				
				
			}//���ı���1ȷ��
				
		}//else if SUBSCRIBE����
		
		/***************************������publish����************************/
		else if(wifireceive->wifi_buffer[0]==0x40)
		{

		printf("�ѽ��շ�����puback����\r\n");

		}//else if puback����

		else if((wifireceive->wifi_buffer[0]&0xF0)==0x30)
		{

		printf("�ѽ��շ�����PUBLISH����\r\n");
			/******���PUBLISH����*****/
		MQTTDeserialize_publish(&DeserializePublish_dup,&DeserializePublish_qos,
									&DeserializePublish_retained,&DeserializePublish_packetid,
									&DeserializePublish_topicName,&DeserializePublish_payload,
									&DeserializePublish_payloadlen,(unsigned char*)(wifireceive->wifi_buffer),
									(int)(wifireceive->wifi_lenth));	

									
									
									
									
									
									
									
									
								//	memcpy(topic,DeserializePublish_topicName.string,DeserializePublish_topicName.len);
									memcpy(data,DeserializePublish_payload,DeserializePublish_payloadlen);
		
									printf("DUP:%d\r\n",DeserializePublish_dup);
										printf("qps:%d\r\n",DeserializePublish_qos);
										printf("retained:%d\r\n",DeserializePublish_retained);
							
									
									
									
									
									
		for (i=0;i<DeserializePublish_topicName.len;i++)
		{
		printf("%c",DeserializePublish_topicName.string[i]);
	
		}
		printf("topic�������\r\n");
			
			
		for (i=0;i<DeserializePublish_payloadlen;i++)
		{
		printf("%c",data[i]);
	
		}
		printf("payload�������\r\n");
		
	
		for (i=0;i<wifireceive->wifi_lenth;i++)
		{
		printf("%02x",wifireceive->wifi_buffer[i]);
	
		}
			
		
		
			/*										
			if(DeserializePublish_topicName.string==topic1)
			{
				printf("����Ϊ��������\r\n");
			}//����ȷ��
			*/
		
			printf("�����ж����\r\n");
			
		};//else if publish����
		

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
