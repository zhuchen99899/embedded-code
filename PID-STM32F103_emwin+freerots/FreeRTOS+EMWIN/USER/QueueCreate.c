#include "freertos.h"
#include "queue.h"
#include "QueueCreate.h"


/***********************************************************
						 创建消息队列
************************************************************/

//按键消息队列的数量
#define KEYMSG_Q_NUM    1  		//按键消息队列的数量  
#define ADCMESSAGE_Q_NUM   1  	//ADC消息队列的数量 
#define SETMESSAGE_Q_NUM   1   	//设置温度消息队列的数量 
#define SETTEMMESSAGE_Q_NUM   1   	//设置PID消息队列的数量 
#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi接收数据的消息队列的数量 
#define PINGREQ_Q_NUM   1   	//PINGREQ的消息队列的数量  
#define PUBLISH_Q_NUM   1			//PUBLISH的消息队列的数量  
#define PWM_Algorithm_NUM  1			//PUBLISH的消息队列的数量  
typedef struct SETMSG
	{
		float Kp;
		float Ki;
		float Kd;
		
		
	}SETMSG;



typedef struct WIFIMSG
{

char  address[256];
u32 length;

} WIFIMSG;
	



//按键消息队列
void Key_QueueCreat(void)
{
	extern QueueHandle_t Key_Queue;
	Key_Queue =xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));
	
	if (Key_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("按键消息队列创建失败");
	}
	else 
	{
	printf("按键消息队列创建成功");
	}
}


void ADC_QueueCreat(void)
{
	extern QueueHandle_t Adc_Queue;
	Adc_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(float));
	if (Adc_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("ADC消息队列创建失败");
	}
	else 
	{
	printf("ADC消息队列创建成功");
	}
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
	Settem_Queue = xQueueCreate(SETTEMMESSAGE_Q_NUM,sizeof(float));
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
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(struct WIFIMSG *));
	
	if (Wifi_buffer_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("MQTT接收消息队列创建失败");
	}
	else 
	{
	printf("MQTT接收消息队列创建成功");
	}
}

void PINGREQ_QueueCreat(void)
{
	extern QueueHandle_t PINGREQ_Queue;
	PINGREQ_Queue =xQueueCreate(PINGREQ_Q_NUM,sizeof(uint8_t));
	
	if (PINGREQ_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("PINGREQ消息队列创建失败");
	}
	else 
	{
	printf("PINGREQ消息队列创建成功");
	}
}


void PUBLISH_QueueCreat(void)
{
	extern QueueHandle_t PUBLISH_Queue;
	PUBLISH_Queue =xQueueCreate(PUBLISH_Q_NUM,sizeof(uint8_t));
	
	if (PUBLISH_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("PUBLISH消息队列创建失败");
	}
	else 
	{
	printf("PUBLISH消息队列创建成功");
	}
}



//PWM算法选择消息队列
void PWM_Algorithm_QueueCreat(void)
{
	extern QueueHandle_t PWM_Algorithm_Queue;
	PWM_Algorithm_Queue =xQueueCreate(PWM_Algorithm_NUM,sizeof(uint8_t));
	
	if (PWM_Algorithm_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("PWM算法选择消息队列创建失败");
	}
	else 
	{
	printf("PWM算法选择消息队列创建成功");
	}
}



void Queue_Creat(void)
{
	/******消息队列创建**********/
Key_QueueCreat();
ADC_QueueCreat();
Set_QueueCreat();
Settem_QueueCreat();
WIFI_buffer_QueueCreat();
PINGREQ_QueueCreat();
PUBLISH_QueueCreat();
PWM_Algorithm_QueueCreat();
}



