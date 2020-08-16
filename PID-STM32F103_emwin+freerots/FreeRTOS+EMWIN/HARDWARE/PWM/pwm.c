#include "stm32f10x.h"
#include "time.h"
#include "sys.h"
#include "pwm.h"


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



