#include "freertos.h"
#include "queue.h"
#include "QueueCreate.h"


/***********************************************************
						 创建消息队列
************************************************************/

//按键消息队列的数量
#define KEYMSG_Q_NUM    1  		//按键消息队列的数量  
#define ADCMESSAGE_Q_NUM   4   	//发送数据的消息队列的数量 
//static QueueHandle_t Key_Queue;   		//按键值消息队列句柄
//static QueueHandle_t Message_Queue;	//信息队列句柄
//按键消息队列
void Key_QueueCreat(void)
{
	extern QueueHandle_t Key_Queue;
	Key_Queue =xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));
	
//	if (Key_Queue==0)
//	{
//	/*消息创建失败处理机制*/
//	printf("按键消息队列创建失败");
//	}
//	else 
//	{
//	printf("按键消息队列创建成功");
//	}
}


void ADC_QueueCreat(void)
{
	extern QueueHandle_t Adc_Queue;
	Adc_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(int));
//	if (Adc_Queue==0)
//	{
//	/*消息创建失败处理机制*/
//	printf("ADC消息队列创建失败");
//	}
//	else 
//	{
//	printf("ADC消息队列创建成功");
//	}
}






