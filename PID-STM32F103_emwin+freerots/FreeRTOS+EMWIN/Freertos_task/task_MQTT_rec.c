#include "task_MQTT_rec.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
#include "MQTT.h"


/**********消息队列参数**************/

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





/***************信号量参数************/
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE报文二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_WIFI_PIDSET;//WIFI_PIDSET二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_WIFI_TEMSET;//WIFI_TEMSET报文二值信号量句柄
	/****消息队列句柄**************/
	extern QueueHandle_t Wifi_buffer_Queue;
	extern QueueHandle_t PINGREQ_Queue;
	extern QueueHandle_t PUBLISH_Queue;

/*************************WIFI接收任务函数******************/
void MQTT_rec_task(void *pvParameters)
{
	/**********************PUBLISH解包参数*********************/
		unsigned char DeserializePublish_dup;
		int DeserializePublish_qos;
		unsigned char DeserializePublish_retained;
		unsigned short DeserializePublish_packetid;
		MQTTString DeserializePublish_topicName;
		unsigned char* DeserializePublish_payload;
		int DeserializePublish_payloadlen;
	
	//消息队列句柄

  extern QueueHandle_t Key_Queue;
	extern QueueHandle_t Set_Queue;
  extern QueueHandle_t Settem_Queue;
							
									
		//申请动态内存指针
	char *topic1;
	char *payload1;
	
	
	/* 创建JSON结构体指针*/
	cJSON *json = NULL;
	cJSON *node = NULL;
	cJSON *node1 = NULL;
	cJSON *node2 = NULL;
	char *json_data = NULL;

	
	/*****JSON树数据变量保存********/
	double JDATA_P;
	double JDATA_I;
	double JDATA_D;
	float JDATA_SETTEM;
	
/********************变量****************************/


	/***************信号量参数************/
	BaseType_t err=pdFALSE;
	/****消息队列参数**************/
	wifibuff *wifireceive;
	u8 PINGREQ_FLAG=0;
	u8 PUBLISH_FLAG=0;
	
	SETMSG *WIFI_PID;

	
	u8 key=0;
	
		WIFI_PID=&g_tMsg;//	PID结构体指针初始化
/*******************阻塞等待DMA中断中传来的ISR信号量***************/
	while(1)
	{	
	err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//获取串口2空闲中断二值信号量
	if(err==pdTRUE)										//获取信号量成功
			{
				
xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
	/*printf("接收到的数据为 %02x\r\n ,长度为 %d\r\n",&wifireceive->wifi_buffer,wifireceive->wifi_lenth);*/
 
				/************************CONNACT报文检测**************************************************/
			//	首字节固定为0x20  剩余字节固定为0x02 可变报头部分为 连接确认标志：0x00 以及连接返回码
		if((wifireceive->wifi_buffer[0]==0x20)&&(wifireceive->wifi_buffer[1]==0x02)&&(wifireceive->wifi_buffer[2]==0x00))    //如果收到4个字节，且第一个字节是0x20，表示是connect ack报文，进入if
		{
			printf("已接收服务器CONNECT报文\r\n");
			switch(wifireceive->wifi_buffer[3])	                                                     //判断收到的第4个字节
			{					
				case 0x00 : printf("CONNECT报文成功\r\n");
										PINGREQ_FLAG=1;   //开始发送PINGREQ报文标志
										PUBLISH_FLAG=1;   //开始发送PUBLISH报文标志
										xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);
										vTaskDelay(3000);
										xSemaphoreGive(BinarySemaphore_MQTTsubscribe);
										vTaskDelay(3000);
										xQueueOverwrite(PUBLISH_Queue,&PUBLISH_FLAG);	
										
										/************SUBSCRIBE,publish*************/
							break;
				case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");
				/***************E1错误重启*********************/		

						break;
				case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n");
				/***************E2错误重启*********************/	

						break;
				case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");
				/***************E3错误重启*********************/	

						break;
				case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");
				/***************E4错误重启*********************/	

						break;
				case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");
				/***************E5错误重启*********************/	

						break;
				default   : printf("连接已拒绝，未知状态，准备重启\r\n");
				/***************E6错误重启*********************/	

				    break;
		
			}				
		}//else if connack报文
		
		/*************收到PINGRESP报文******************************/
		//首字节固定为0xD0 剩余长度固定为 0x00 ，无可变报头
		else if((wifireceive->wifi_buffer[0]==0xD0)&&(wifireceive->wifi_buffer[1]==0x00)) 
		{
		
		printf("已接收服务器PINGRESP报文\r\n");
		
		}//else if PINGRESP报文
		
		/*****************SUBACK报文********************************/
		/*************如果增加subscribe，需要对wifi_buffer[2]剩余字节修改，增加一个加1，并且在下面主题判断中增加判断主题*****/
		else if((wifireceive->wifi_buffer[0]==0x90)&&(wifireceive->wifi_buffer[1]==0x04)) 
		{
				printf("已接收服务器SUBACK报文\r\n");
				if(wifireceive->wifi_buffer[2]==0x00&&wifireceive->wifi_buffer[3]==0x01)			//接收到订阅报文1订阅确认,报文标识符1
				{
					printf("已接收服务器SUBACK报文,message id=1\r\n");
				switch(wifireceive->wifi_buffer[4])									//判断收到的第5个字节
				{
					case 0x00 : 
								printf("主题1订阅成功,qos等级00\r\n");			//串口输出信息
								break;
					case 0x01 :
								printf("主题1订阅成功,qos等级01\r\n");			//串口输出信息
								break;
					case 0x02 :
								printf("主题1订阅成功,qos等级02\r\n");			//串口输出信息
								break;
					default   : printf("主题2订阅失败\r\n");			//串口输出信息 
								break;
				}//主题1确认
				switch(wifireceive->wifi_buffer[5])
				{
								case 0x00 : 
								printf("主题2订阅成功,qos等级00\r\n");			//串口输出信息
								break;
					case 0x01 :
								printf("主题2订阅成功,qos等级01\r\n");			//串口输出信息
								break;
					case 0x02 :
								printf("主题2订阅成功,qos等级02\r\n");			//串口输出信息
								break;
					default   : printf("主题2订阅失败\r\n");			//串口输出信息 
								break;	
				}//主题2确认
				
				switch(wifireceive->wifi_buffer[6])
				{
								case 0x00 : 
								printf("主题3订阅成功,qos等级00\r\n");			//串口输出信息
								break;
					case 0x01 :
								printf("主题3订阅成功,qos等级01\r\n");			//串口输出信息
								break;
					case 0x02 :
								printf("主题3订阅成功,qos等级02\r\n");			//串口输出信息
								break;
					default   : printf("主题3订阅失败\r\n");			//串口输出信息 
								break;	
				}//主题3确认
				
				
				
				
			}//订阅报文1确认
				
		}//else if SUBSCRIBE报文
		
		/***************************服务器publish报文************************/
		else if(wifireceive->wifi_buffer[0]==0x40)
		{

		printf("已接收服务器puback报文\r\n");

		}//else if puback报文

		else if((wifireceive->wifi_buffer[0]&0xF0)==0x30)
		{

		printf("已接收服务器PUBLISH报文\r\n");
			/******解包PUBLISH报文*****/
		MQTTDeserialize_publish(&DeserializePublish_dup,&DeserializePublish_qos,
									&DeserializePublish_retained,&DeserializePublish_packetid,
									&DeserializePublish_topicName,&DeserializePublish_payload,
									&DeserializePublish_payloadlen,(unsigned char*)(wifireceive->wifi_buffer),
									(int)(wifireceive->wifi_lenth));	

				/*************主题/负载判断****************/						
									
					printf("DUP:%d\r\n",DeserializePublish_dup);
					printf("qps:%d\r\n",DeserializePublish_qos);
					printf("retained:%d\r\n",DeserializePublish_retained);
		
							
									
									
//申请动态内存保存主题和负载							
topic1=pvPortMalloc(DeserializePublish_topicName.len);
payload1=pvPortMalloc(DeserializePublish_payloadlen);
memcpy(payload1,DeserializePublish_payload,DeserializePublish_payloadlen);
memcpy(topic1,DeserializePublish_topicName.string,DeserializePublish_topicName.len);
//清零字符串最后一位，防止乱码
payload1[DeserializePublish_payloadlen]=0;
topic1[DeserializePublish_topicName.len]=0;		

									
									
							/***********按键主题*******************/
			if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters1)==0))
			{
				printf("主题为按键主题\r\n");							
				printf("主题为%s\r\n",topic1);	
				printf("负载为\r\n");
				
				
								/****************有效载荷Cjson解包******************/
			
			
									/**********	完整输出一个JSON对象结构************/
								json = cJSON_Parse(payload1);                         //从数据缓冲区解析JASON对象结构
								json_data = cJSON_Print(json);                    //将传入的JASON对象转化为JASON字符串
								printf("data: %s\n", json_data);                  //输出JASON结构
								vPortFree(json_data);                                  //释放JASON结构

									/****************JASON树中键值对解析***************/
									//根据键名再JASON中查找子节点，查找出LED0对象
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
													//从LED0中获取结果

												printf("value:%d\r\n",node->valueint);
														if((node->valueint)==1)
														{
														key=1;	
														xQueueSend(Key_Queue,&key,10);
														printf("LED0翻转");
														}

											}//查找LED0值
											
										

													
									}//识别JASON树中LED0对象
			
			
				
			}//按键主题确认
			
						/********************PID主题******************/
			else if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters2)==0))
			{
			
				printf("主题为PID主题\r\n");							
				printf("主题为%s\r\n",topic1);	
				printf("负载为\r\n");
				
						/****************有效载荷Cjson解包******************/
			
			
									/**********	完整输出一个JSON对象结构************/
								json = cJSON_Parse(payload1);                         //从数据缓冲区解析JASON对象结构
								json_data = cJSON_Print(json);                    //将传入的JASON对象转化为JASON字符串
								printf("data: %s\n", json_data);                  //输出JASON结构
								vPortFree(json_data);                                  //释放JASON结构

									/****************JASON树中键值对解析***************/
				
				          /*************找到P值**************/
									//根据键名再JASON中查找子节点，查找出P对象
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
													//从P对象提取值

												printf("value:%f\r\n",node->valuedouble);
												JDATA_P = (node->valuedouble);

											}//查找P对象内容类型
											
														
									}//识别到JASON树 -> P
			
									
									
									
									/*************找到I值**************/
									//根据键名再JASON中查找子节点，查找出I对象
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
													//从I对象提取值

												printf("value:%f\r\n",node1->valuedouble);
												JDATA_I = (node1->valuedouble);

											}//查找I对象内容类型
											
														
									}//识别到JASON树 -> I
									
									
									/*************找到D值**************/
									//根据键名再JASON中查找子节点，查找出D对象
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
													//从D对象提取值

												printf("value:%f\r\n",node2->valuedouble);
												JDATA_D = (node2->valuedouble);

											}//查找D对象内容类型
											
														
									}//识别到JASON树 -> D
									
									
								//消息队列传递PID数值						
									if ((node != NULL)&&(node1 != NULL)&&(node2 != NULL))	
							{	
			
									WIFI_PID->Kp=JDATA_P;
									WIFI_PID->Ki=JDATA_I;
									WIFI_PID->Kd=JDATA_D;
									xQueueOverwrite(Set_Queue,(void *)&WIFI_PID);				
						
							}
			
		
			}//PID主题确认
			
			else if((strcmp(topic1,topic1_compare_Universal)==0)||(strcmp(topic1,topic1_compare_monolayer)==0)||(strcmp(topic1,subscrible1_topicFilters3)==0))
			{
				printf("主题为设置温度主题\r\n");							
				printf("主题为%s\r\n",topic1);	
				printf("负载为\r\n");
				
				
								/****************有效载荷Cjson解包******************/
			
			
									/**********	完整输出一个JSON对象结构************/
								json = cJSON_Parse(payload1);                         //从数据缓冲区解析JASON对象结构
								json_data = cJSON_Print(json);                    //将传入的JASON对象转化为JASON字符串
								printf("data: %s\n", json_data);                  //输出JASON结构
								vPortFree(json_data);                                  //释放JASON结构

									/****************JASON树中键值对解析***************/
									//根据键名再JASON中查找子节点，查找出SETTEM对象
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
													//从SETTEM中获取结果

												printf("value:%f\r\n",node->valuedouble);

												JDATA_SETTEM=(float)(node->valuedouble);
												
												xQueueOverwrite(Settem_Queue,&JDATA_SETTEM);	

											}//查找SETTEM值
											
										

													
									}//识别JASON树中SETTEM对象
			
			
				
			}//settem主题确认
			
			
			else
			{
			printf("已订阅的未知主题，请查看设置\r\n");
			}
			//主题确认
		
			
			//释放JASON树
			cJSON_Delete(json);
			
			//释放解包所申请的空间 
	vPortFree(payload1);	
	vPortFree(topic1);	
  printf(" 接收任务堆剩余量= %d\r\n", xPortGetFreeHeapSize());

			
		};//else if publish报文
		

	}//WIFI串口接收信号量
			
			/************串口未收到任何服务器返回(信号量持续阻塞)***********************/
			
		//else{
			/************************
			建立Freertos软件定时器 周期定时 ，时间为MQTTconfig.h中设值拿keepAlive_Interval 
			定时器回调函数即超时处理。
			**************/
			
			
			
			//	}
			
			
	
   vTaskDelay(30);                           //延时3000ms
	}

}
