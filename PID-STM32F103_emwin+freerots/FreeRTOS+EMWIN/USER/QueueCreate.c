#include "freertos.h"
#include "queue.h"
#include "QueueCreate.h"


/***********************************************************
						 创建消息队列
************************************************************/

//按键消息队列的数量
#define KEYMSG_Q_NUM    1  		//按键消息队列的数量  
#define ADCMESSAGE_Q_NUM   1  	//发送数据的消息队列的数量 
#define SETMESSAGE_Q_NUM   1   	//发送数据的消息队列的数量 
#define SETTEMMESSAGE_Q_NUM   1   	//发送数据的消息队列的数量 
#define WIFIMESSAGE_buffer_Q_NUM   1   	//发送数据的消息队列的数量 
#define WIFIMESSAGE_lenth_Q_NUM   1   	//发送数据的消息队列的数量 



//static QueueHandle_t Key_Queue;   		//按键值消息队列句柄
//static QueueHandle_t Adc_Queue;	//ADC消息队列句柄
//static QueueHandle_t Settem_Queue;	//温度设置息队列句柄
//static QueueHandle_t SetP_Queue;   		//p设置消息队列句柄
//static QueueHandle_t SetI_Queue;	//I设置消息队列句柄
//static QueueHandle_t SetD_Queue;	//D设置消息队列句柄

	//结构体保存变量 用于消息队列
//extern	struct SETMSG
//	{
//		float Kp,Ki,Kd;
//		
//	}SETMSG;




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
	Adc_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(float));
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

void Set_QueueCreat(void)
{
	extern QueueHandle_t Set_Queue;
	Set_Queue = xQueueCreate(SETMESSAGE_Q_NUM,sizeof(struct SETMSG *));
	if (Set_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("设置PID消息队列创建失败");
	}
	else 
	{
	printf("设置PID消息队列创建成功");
	}
}

void Settem_QueueCreat(void)
{
	extern QueueHandle_t Settem_Queue;
	Settem_Queue = xQueueCreate(SETTEMMESSAGE_Q_NUM,sizeof(int));
	if (Settem_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("设置温度消息队列创建失败");
	}
	else 
	{
	printf("设置温度消息队列创建成功");
	}
}


void WIFI_buffer_QueueCreat(void)
{
	extern QueueHandle_t Wifi_buffer_Queue;
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(uint8_t));
	
//	if (Key_Queue==0)
//	{
//	/*消息创建失败处理机制*/
//	printf("WIFI消息队列创建失败");
//	}
//	else 
//	{
//	printf("WIFI消息队列创建成功");
//	}
}

void WIFI_lenth_QueueCreat(void)
{
	extern QueueHandle_t Wifi_lenth_Queue;
	Wifi_lenth_Queue =xQueueCreate(WIFIMESSAGE_lenth_Q_NUM,sizeof(uint8_t));
	
//	if (Key_Queue==0)
//	{
//	/*消息创建失败处理机制*/
//	printf("WIFI消息队列创建失败");
//	}
//	else 
//	{
//	printf("WIFI消息队列创建成功");
//	}
}

void Queue_Creat(void)
{
	/******消息队列创建**********/
Key_QueueCreat();
ADC_QueueCreat();
Set_QueueCreat();
Settem_QueueCreat();
WIFI_buffer_QueueCreat();
WIFI_lenth_QueueCreat();
}



