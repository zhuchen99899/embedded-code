#include "task_pwm.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "pid.h"
#include "pwm.h"
#include <EventGroupCreat.h>
#include "event_groups.h"



/****************事件标志组句柄*************************/
extern EventGroupHandle_t EventGroupHandler;	//事件标志组句柄
extern QueueHandle_t PWM_Algorithm_Queue;   //PWM算法标志句柄
extern QueueHandle_t Settem_Queue;
extern QueueHandle_t Set_Queue;
extern QueueHandle_t Adc_Queue;

/***************************************
PWM任务
任务中进行温度修改，PID参数的改变
最终实现PWM改变
*********************************/
typedef struct SETMSG
	{
		float Kp;
		float Ki;
		float Kd;
		
	}SETMSG;


struct pid {
	
	
	float SetTem;    //设定的温度
	float ActualTem;  //实际达到的温度
	float err;           //偏差值
	float err_last;			//上一次的偏差值
	float Kp,Ki,Kd;            //PID参数
	float voltage;				//PWM占空比改变值，PID算法的输出值
	float integral;            //积分项
}pid;


float ADC1_tem;
//全局变量定义

	
void PWM_task(void *pvParameters)
{
	u16 led0pwmval;
	/***消息队列参数定义***/
	//温度设置消息队列参数
	//float settemdisplay;
	float Settem;
	//ADC消息队列参数
	float adc1;
	
	 u8 PWM_Algorithm_FLAG=0;
	
	SETMSG *PIDMSG;
	
	while(1)
	{
		
		
			xQueuePeek(PWM_Algorithm_Queue,&PWM_Algorithm_FLAG,portMAX_DELAY);

		
		if(PWM_Algorithm_FLAG)
		{	
		
			//使用模糊算法
			
		}
		else
		{
			/************************************************
			消息队列传参Settem
			来源:EMWIN任务set界面，WIFI任务 开机后flash读取
			************************************************/

			xQueuePeek(Set_Queue,(void *)&PIDMSG,portMAX_DELAY);
			xQueuePeek(Settem_Queue,&Settem,portMAX_DELAY);
		

			/************************************************
			消息队列传参adc
			来源:ADC任务
			************************************************/

			xQueuePeek(Adc_Queue,&adc1,portMAX_DELAY);
				
				
			/*************PID结构体全局赋值******************/
			pid.Kp=PIDMSG->Kp;
			pid.Ki=PIDMSG->Ki;
			pid.Kd=PIDMSG->Kd;
			ADC1_tem=adc1;
				
				
				
		}//PID算法选择
		
		/*****************PWM改变*****************/
		led0pwmval=pid_realize(Settem);
		
		//settemdisplay=Settem;
		//printf("PWM寄存器=%d \r\n,ADC1_tem=%f \r\n,kp=%f\r\n,ki=%f \r\n,kd=%f\r\n,settem=%f \r\n",led0pwmval,adc1,pid.Kp,pid.Ki,pid.Kd,settemdisplay);

		TIM_SetCompare2(TIM3,led0pwmval);//PWM输出



	xEventGroupSetBits(EventGroupHandler,EVENTBIT_2);

     vTaskDelay(1000);                          

 }

}

