#include "stm32f10x.h"
#include "time.h"
#include "sys.h"
#include "led.h"
#include "pwm.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "pid.h"

void Timer3_Init(u16 arr,u16 psc)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
NVIC_InitTypeDef NVIC_Initstrue;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //通用定时器3RCC时钟使能
	
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;          //在预分频前的延时（不影响预分频系数），不常用
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数
	TIM_TimeBaseInitStrue.TIM_Period=arr;         //自动装载值
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc;            //时钟预分频系数

TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);//定时器3寄存器初始化

TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//定时器3中断设置：更新中断（有某个寄存器更新，溢出中断）

	
	NVIC_Initstrue.NVIC_IRQChannel= TIM3_IRQn;  //TIM3中断通道
	NVIC_Initstrue.NVIC_IRQChannelCmd=ENABLE;	//IRQ中断通道使能
	NVIC_Initstrue.NVIC_IRQChannelPreemptionPriority=0; //抢占优先级为2
	NVIC_Initstrue.NVIC_IRQChannelSubPriority=3;	//子优先级为2
	NVIC_Init(&NVIC_Initstrue); //初始化NVIC中断优先级寄存器
	
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	
}

void TIM3_IRQHandler(void)
{

if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{

		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}	


}


void TIM3_PWM_Init(u16 arr,u16 psc)
{

	GPIO_InitTypeDef GPIO_Structure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
	TIM_OCInitTypeDef TIM_OCInitStrue;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//使能GPIOB外设时钟和AFIO端口复用时钟
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能定时器3的时钟
	
	GPIO_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Structure.GPIO_Pin=GPIO_Pin_5;
	GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Structure);
	//PB5复用初始化

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	//定时器通道2部分重映射才能到PB5.没有重映射在PA7,完全重映射在PC7
	
		TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;          //在预分频前的延时（不影响预分频系数），不常用
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数
	TIM_TimeBaseInitStrue.TIM_Period=arr;         //自动装载值
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc;            //时钟预分频系数
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);//定时器3寄存器初始化
	
	TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM2;//通道2输出模式位PWM2
	TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;//通道2输出极性为高
	TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;//通道2输出使能
	
	TIM_OC2Init(TIM3,&TIM_OCInitStrue);//定时器3通道2初始化
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);//使能CCMRi预装载（CCMRi控制占空比，必须设置CCMRi寄存器的OCiPE位使能相应的预装载寄存器，也可以设置TIM3_CR1寄存器的ARPE位使能自动重装载的预装载寄存器(在向上计数模式或中央对称模式中)。）
	
	TIM_Cmd(TIM3,ENABLE);//使能定时器3
	
}


/***************************************
PWM任务
任务中进行温度修改，PID参数的改变
最终实现PWM改变
*********************************/

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
	float Settem;
	extern QueueHandle_t Settem_Queue;
	BaseType_t err_settem;
	//P设置消息队列参数
	float SetP;
	extern QueueHandle_t SetP_Queue;
	BaseType_t err_setp;
	//I设置消息队列参数
	float SetI;
	extern QueueHandle_t SetI_Queue;
BaseType_t err_seti;
	//D设置消息队列参数
	float SetD;
	extern QueueHandle_t SetD_Queue;
	BaseType_t err_setd;
	//ADC消息队列参数
	extern QueueHandle_t Adc_Queue;

	float adc1;
	
	
	while(1)
	{
		
		
		
		
		/************************************************
		消息队列传参Settem
		来源:EMWIN任务set界面，WIFI任务
		************************************************/

				xQueueReceive(Settem_Queue,&Settem,10);

		

						/************************************************
						消息队列传参Setp\SetI\SetD
						来源:EMWIN任务set界面，emwin初始化后flash读取，WIFI任务
						************************************************/
				
									xQueueReceive(SetP_Queue,&SetP,10);
									xQueueReceive(SetI_Queue,&SetI,10);
									xQueueReceive(SetD_Queue,&SetD,10);

						/************************************************
						消息队列传参adc
						来源:ADC任务
						************************************************/

							xQueueReceive(Adc_Queue,&adc1,portMAX_DELAY);
				
				
				/*************PID结构体全局赋值******************/
				pid.Kp=SetP;
				pid.Ki=SetI;
				pid.Kd=SetD;
				ADC1_tem=adc1;
				

				

		
		/*****************PWM改变*****************/
		led0pwmval=pid_realize(Settem);
			printf("PWM寄存器=%d \r\n,ADC1_tem=%f \r\n,kp=%f\r\n,ki=%f \r\n,kd=%f\r\n",led0pwmval,adc1,SetP,SetI,SetD);

		TIM_SetCompare2(TIM3,led0pwmval);//PWM输出

     vTaskDelay(500);                          

}

}



