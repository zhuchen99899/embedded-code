#include "task_MQTT_rec.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"

/*	WIFI接收调试输出
for (i=0;i<4;i++)
	{
	printf("%02x",(wifireceive->wifi_buffer)[i]);
	
	}
				*/

/**********消息队列参数**************/

typedef struct wifibuff
{
u32 wifi_lenth;
u8 wifi_buffer[256];
}wifibuff;

/***************信号量参数************/
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//二值信号量句柄
	/****消息队列参数**************/
	extern QueueHandle_t Wifi_buffer_Queue;
	extern QueueHandle_t PINGREQ_Queue;

void MQTT_rec_task(void *pvParameters)
{
	//int i;

	/***************信号量参数************/
	BaseType_t err=pdFALSE;
	/****消息队列参数**************/
	wifibuff *wifireceive;
	u8 PINGREQ_FLAG;
	while(1)
	{	
	err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//获取串口2空闲中断二值信号量
	if(err==pdTRUE)										//获取信号量成功
			{
				
xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
	/*printf("接收到的数据为 %02x\r\n ,长度为 %d\r\n",&wifireceive->wifi_buffer,wifireceive->wifi_lenth);*/
 
				/************************CONNACT报文检测**************************************************/
				
		if((wifireceive->wifi_buffer[0]==0x20)&&(wifireceive->wifi_buffer[1]==0x02)&&(wifireceive->wifi_buffer[2]==0x00))    //如果收到4个字节，且第一个字节是0x20，表示是connect ack报文，进入if
		{
			switch(wifireceive->wifi_buffer[3])	                                                     //判断收到的第4个字节
			{					
				case 0x00 : printf("CONNECT报文成功\r\n");
										PINGREQ_FLAG=1;   //开始发送PINGREQ报文标志
										xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);
										/************SUBSCRIBE,publish*************/
							break;
				case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");
				/***************E1错误重启*********************/		
						PINGREQ_FLAG=0;   //关闭发送PINGREQ报文标志
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);		
						break;
				case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n");
				/***************E2错误重启*********************/	
						PINGREQ_FLAG=0;   //关闭发送PINGREQ报文标志
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);		
						break;
				case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");
				/***************E3错误重启*********************/	
						PINGREQ_FLAG=0;   //关闭发送PINGREQ报文标志
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);		
						break;
				case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");
				/***************E4错误重启*********************/	
						PINGREQ_FLAG=0;   //关闭发送PINGREQ报文标志
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);		
						break;
				case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");
				/***************E5错误重启*********************/	
						PINGREQ_FLAG=0;   //关闭发送PINGREQ报文标志
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);	
						break;
				default   : printf("连接已拒绝，未知状态，准备重启\r\n");
				/***************E6错误重启*********************/	
						PINGREQ_FLAG=0;   //关闭发送PINGREQ报文标志
						xQueueOverwrite(PINGREQ_Queue,&PINGREQ_FLAG);	
				    break;
		
			}				
		}
		
		/*************其他报文******************************/
		else if((wifireceive->wifi_buffer[0]==0xD0)&&(wifireceive->wifi_buffer[1]==0x00)) 
		{
		
		printf("已接收服务器PINGRESP报文\r\n");
		
		}

		
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
