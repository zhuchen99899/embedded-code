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


typedef  struct SETMSG
	{
		float Kp;
		float Ki;
		float Kd;
		
	}SETMSG;
extern SETMSG g_tMsg;





/***************�ź�������************/
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//��ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_WIFI_PIDSET;//WIFI_PIDSET��ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_WIFI_TEMSET;//WIFI_TEMSET���Ķ�ֵ�ź������
	/****��Ϣ���о��**************/
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
	
	//��Ϣ���о��

  extern QueueHandle_t Key_Queue;
	extern QueueHandle_t Set_Queue;
  extern QueueHandle_t Settem_Queue;
							
									
		//���붯̬�ڴ�ָ��
	char *topic1;
	char *payload1;
	
	
	/* ����JSON�ṹ��ָ��*/
	cJSON *json = NULL;
	cJSON *node = NULL;
	cJSON *node1 = NULL;
	cJSON *node2 = NULL;
	char *json_data = NULL;

	
	/*****JSON�����ݱ�������********/
	double JDATA_P;
	double JDATA_I;
	double JDATA_D;
	float JDATA_SETTEM;
	
/********************����****************************/


	/***************�ź�������************/
	BaseType_t err=pdFALSE;
	/****��Ϣ���в���**************/
	wifibuff *wifireceive;
	u8 PINGREQ_FLAG=0;
	u8 PUBLISH_FLAG=0;
	
	SETMSG *WIFI_PID;

	
	u8 key=0;
	
		WIFI_PID=&g_tMsg;//	PID�ṹ��ָ���ʼ��
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
				
				switch(wifireceive->wifi_buffer[6])
				{
								case 0x00 : 
								printf("����3���ĳɹ�,qos�ȼ�00\r\n");			//���������Ϣ
								break;
					case 0x01 :
								printf("����3���ĳɹ�,qos�ȼ�01\r\n");			//���������Ϣ
								break;
					case 0x02 :
								printf("����3���ĳɹ�,qos�ȼ�02\r\n");			//���������Ϣ
								break;
					default   : printf("����3����ʧ��\r\n");			//���������Ϣ 
								break;	
				}//����3ȷ��
				
				
				
				
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

				/*************����/�����ж�****************/						
									
					printf("DUP:%d\r\n",DeserializePublish_dup);
					printf("qps:%d\r\n",DeserializePublish_qos);
					printf("retained:%d\r\n",DeserializePublish_retained);
		
							
									
									
//���붯̬�ڴ汣������͸���							
topic1=pvPortMalloc(DeserializePublish_topicName.len);
payload1=pvPortMalloc(DeserializePublish_payloadlen);
memcpy(payload1,DeserializePublish_payload,DeserializePublish_payloadlen);
memcpy(topic1,DeserializePublish_topicName.string,DeserializePublish_topicName.len);
//�����ַ������һλ����ֹ����
payload1[DeserializePublish_payloadlen]=0;
topic1[DeserializePublish_topicName.len]=0;		

									
									
							/***********��������*******************/
			if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters1)==0))
			{
				printf("����Ϊ��������\r\n");							
				printf("����Ϊ%s\r\n",topic1);	
				printf("����Ϊ\r\n");
				
				
								/****************��Ч�غ�Cjson���******************/
			
			
									/**********	�������һ��JSON����ṹ************/
								json = cJSON_Parse(payload1);                         //�����ݻ���������JASON����ṹ
								json_data = cJSON_Print(json);                    //�������JASON����ת��ΪJASON�ַ���
								printf("data: %s\n", json_data);                  //���JASON�ṹ
								vPortFree(json_data);                                  //�ͷ�JASON�ṹ

									/****************JASON���м�ֵ�Խ���***************/
									//���ݼ�����JASON�в����ӽڵ㣬���ҳ�LED0����
									node = cJSON_GetObjectItem(json, "LED0");
									if (node == NULL)
									{
									printf("LED0: no\r\n");
										
									}	
									else
									{
									printf("LED0: ok\r\n");
										 if( node->type == cJSON_Number )

											{
													//��LED0�л�ȡ���

												printf("value:%d\r\n",node->valueint);
														if((node->valueint)==1)
														{
														key=1;	
														xQueueSend(Key_Queue,&key,10);
														printf("LED0��ת");
														}

											}//����LED0ֵ
											
										

													
									}//ʶ��JASON����LED0����
			
			
				
			}//��������ȷ��
			
						/********************PID����******************/
			else if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters2)==0))
			{
			
				printf("����ΪPID����\r\n");							
				printf("����Ϊ%s\r\n",topic1);	
				printf("����Ϊ\r\n");
				
						/****************��Ч�غ�Cjson���******************/
			
			
									/**********	�������һ��JSON����ṹ************/
								json = cJSON_Parse(payload1);                         //�����ݻ���������JASON����ṹ
								json_data = cJSON_Print(json);                    //�������JASON����ת��ΪJASON�ַ���
								printf("data: %s\n", json_data);                  //���JASON�ṹ
								vPortFree(json_data);                                  //�ͷ�JASON�ṹ

									/****************JASON���м�ֵ�Խ���***************/
				
				          /*************�ҵ�Pֵ**************/
									//���ݼ�����JASON�в����ӽڵ㣬���ҳ�P����
									node = cJSON_GetObjectItem(json, "P");
									if (node == NULL)
									{
									printf("p: no\r\n");
										
									}	
									else
									{
									printf("P: ok\r\n");
										 if( node->type == cJSON_Number )

											{
													//��P������ȡֵ

												printf("value:%f\r\n",node->valuedouble);
												JDATA_P = (node->valuedouble);

											}//����P������������
											
														
									}//ʶ��JASON�� -> P
			
									
									
									
									/*************�ҵ�Iֵ**************/
									//���ݼ�����JASON�в����ӽڵ㣬���ҳ�I����
									node1 = cJSON_GetObjectItem(json, "I");
									if (node == NULL)
									{
									printf("I: no\r\n");
										
									}	
									else
									{
									printf("I: ok\r\n");
										 if( node->type == cJSON_Number )

											{
													//��I������ȡֵ

												printf("value:%f\r\n",node1->valuedouble);
												JDATA_I = (node1->valuedouble);

											}//����I������������
											
														
									}//ʶ��JASON�� -> I
									
									
									/*************�ҵ�Dֵ**************/
									//���ݼ�����JASON�в����ӽڵ㣬���ҳ�D����
									node2 = cJSON_GetObjectItem(json, "D");
									if (node == NULL)
									{
									printf("D: no\r\n");
										
									}	
									else
									{
									printf("D: ok\r\n");
										 if( node2->type == cJSON_Number )

											{
													//��D������ȡֵ

												printf("value:%f\r\n",node2->valuedouble);
												JDATA_D = (node2->valuedouble);

											}//����D������������
											
														
									}//ʶ��JASON�� -> D
									
									
								//��Ϣ���д���PID��ֵ						
									if ((node != NULL)&&(node1 != NULL)&&(node2 != NULL))	
							{	
			
									WIFI_PID->Kp=JDATA_P;
									WIFI_PID->Ki=JDATA_I;
									WIFI_PID->Kd=JDATA_D;
									xQueueOverwrite(Set_Queue,(void *)&WIFI_PID);				
						
							}
			
		
			}//PID����ȷ��
			
			else if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters3)==0))
			{
				printf("����Ϊ�����¶�����\r\n");							
				printf("����Ϊ%s\r\n",topic1);	
				printf("����Ϊ\r\n");
				
				
								/****************��Ч�غ�Cjson���******************/
			
			
									/**********	�������һ��JSON����ṹ************/
								json = cJSON_Parse(payload1);                         //�����ݻ���������JASON����ṹ
								json_data = cJSON_Print(json);                    //�������JASON����ת��ΪJASON�ַ���
								printf("data: %s\n", json_data);                  //���JASON�ṹ
								vPortFree(json_data);                                  //�ͷ�JASON�ṹ

									/****************JASON���м�ֵ�Խ���***************/
									//���ݼ�����JASON�в����ӽڵ㣬���ҳ�SETTEM����
									node = cJSON_GetObjectItem(json, "SETTEM");
									if (node == NULL)
									{
									printf("SETTEM: no\r\n");
										
									}	
									else
									{
									printf("SETTEM: ok\r\n");
										 if( node->type == cJSON_Number )

											{
													//��SETTEM�л�ȡ���

												printf("value:%f\r\n",node->valuedouble);

												JDATA_SETTEM=(float)(node->valuedouble);
												
												xQueueOverwrite(Settem_Queue,&JDATA_SETTEM);	

											}//����SETTEMֵ
											
										

													
									}//ʶ��JASON����SETTEM����
			
			
				
			}//settem����ȷ��
			
			
			else
			{
			printf("�Ѷ��ĵ�δ֪���⣬��鿴����\r\n");
			}
			//����ȷ��
		
			
			//�ͷ�JASON��
			cJSON_Delete(json);
			
			//�ͷŽ��������Ŀռ� 
	vPortFree(payload1);	
	vPortFree(topic1);	
  printf(" ���������ʣ����= %d\r\n", xPortGetFreeHeapSize());

			
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
