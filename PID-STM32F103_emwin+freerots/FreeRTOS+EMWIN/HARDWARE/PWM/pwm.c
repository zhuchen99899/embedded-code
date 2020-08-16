#include "stm32f10x.h"
#include "time.h"
#include "sys.h"
#include "pwm.h"


void Timer3_Init(u16 arr,u16 psc)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
NVIC_InitTypeDef NVIC_Initstrue;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //ͨ�ö�ʱ��3RCCʱ��ʹ��
	
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;          //��Ԥ��Ƶǰ����ʱ����Ӱ��Ԥ��Ƶϵ������������
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;   //���ϼ���
	TIM_TimeBaseInitStrue.TIM_Period=arr;         //�Զ�װ��ֵ
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc;            //ʱ��Ԥ��Ƶϵ��

TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);//��ʱ��3�Ĵ�����ʼ��

TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��ʱ��3�ж����ã������жϣ���ĳ���Ĵ������£�����жϣ�

	
	NVIC_Initstrue.NVIC_IRQChannel= TIM3_IRQn;  //TIM3�ж�ͨ��
	NVIC_Initstrue.NVIC_IRQChannelCmd=ENABLE;	//IRQ�ж�ͨ��ʹ��
	NVIC_Initstrue.NVIC_IRQChannelPreemptionPriority=0; //��ռ���ȼ�Ϊ2
	NVIC_Initstrue.NVIC_IRQChannelSubPriority=3;	//�����ȼ�Ϊ2
	NVIC_Init(&NVIC_Initstrue); //��ʼ��NVIC�ж����ȼ��Ĵ���
	
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	
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
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//ʹ��GPIOB����ʱ�Ӻ�AFIO�˿ڸ���ʱ��
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ�ܶ�ʱ��3��ʱ��
	
	GPIO_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Structure.GPIO_Pin=GPIO_Pin_5;
	GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Structure);
	//PB5���ó�ʼ��

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	//��ʱ��ͨ��2������ӳ����ܵ�PB5.û����ӳ����PA7,��ȫ��ӳ����PC7
	
		TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;          //��Ԥ��Ƶǰ����ʱ����Ӱ��Ԥ��Ƶϵ������������
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;   //���ϼ���
	TIM_TimeBaseInitStrue.TIM_Period=arr;         //�Զ�װ��ֵ
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc;            //ʱ��Ԥ��Ƶϵ��
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);//��ʱ��3�Ĵ�����ʼ��
	
	TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM2;//ͨ��2���ģʽλPWM2
	TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;//ͨ��2�������Ϊ��
	TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;//ͨ��2���ʹ��
	
	TIM_OC2Init(TIM3,&TIM_OCInitStrue);//��ʱ��3ͨ��2��ʼ��
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);//ʹ��CCMRiԤװ�أ�CCMRi����ռ�ձȣ���������CCMRi�Ĵ�����OCiPEλʹ����Ӧ��Ԥװ�ؼĴ�����Ҳ��������TIM3_CR1�Ĵ�����ARPEλʹ���Զ���װ�ص�Ԥװ�ؼĴ���(�����ϼ���ģʽ������Գ�ģʽ��)����
	
	TIM_Cmd(TIM3,ENABLE);//ʹ�ܶ�ʱ��3
	
}



