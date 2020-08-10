#include "freertos.h"
#include "queue.h"
#include "QueueCreate.h"


/***********************************************************
						 创建消息队列
************************************************************/

//按键消息队列的数量
#define KEYMSG_Q_NUM    1  		//按键消息队列的数量  
#define ADCMESSAGE_Q_NUM   4   	//发送数据的消息队列的数量 
#define SETTEMMESSAGE_Q_NUM   4   	//发送数据的消息队列的数量 
#define SETPMESSAGE_Q_NUM   4   	//发送数据的消息队列的数量 
#define SETIMESSAGE_Q_NUM   4   	//发送数据的消息队列的数量 
#define SETDMESSAGE_Q_NUM   4   	//发送数据的消息队列的数量 
//static QueueHandle_t Key_Queue;   		//按键值消息队列句柄
//static QueueHandle_t Adc_Queue;	//ADC消息队列句柄
//static QueueHandle_t Settem_Queue;	//温度设置息队列句柄
//static QueueHandle_t SetP_Queue;   		//p设置消息队列句柄
//static QueueHandle_t SetI_Queue;	//I设置消息队列句柄
//static QueueHandle_t SetD_Queue;	//D设置消息队列句柄
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

void Settem_QueueCreat(void)
{
	extern QueueHandle_t Settem_Queue;
	Settem_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(int));
	if (Settem_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("温度设置消息队列创建失败");
	}
	else 
	{
	printf("温度设置消息队列创建成功");
	}
}

void SetP_QueueCreat(void)
{
	extern QueueHandle_t SetP_Queue;
	SetP_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(int));
	if (SetP_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("p设置消息队列创建失败");
	}
	else 
	{
	printf("p设置消息队列创建成功");
	}
}

void SetI_QueueCreat(void)
{
	extern QueueHandle_t SetI_Queue;
	SetI_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(int));
	if (SetI_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("i设置消息队列创建失败");
	}
	else 
	{
	printf("i设置消息队列创建成功");
	}
}

void SetD_QueueCreat(void)
{
	extern QueueHandle_t SetD_Queue;
	SetD_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(int));
	if (SetD_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("d设置消息队列创建失败");
	}
	else 
	{
	printf("d设置消息队列创建成功");
	}
}


