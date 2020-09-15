#include "task_pwm.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "pid.h"
#include "pwm.h"
#include <EventGroupCreat.h>
#include "event_groups.h"



/****************�¼���־����*************************/
extern EventGroupHandle_t EventGroupHandler;	//�¼���־����
extern QueueHandle_t PWM_Algorithm_Queue;   //PWM�㷨��־���
extern QueueHandle_t Settem_Queue;
extern QueueHandle_t Set_Queue;
extern QueueHandle_t Adc_Queue;

/***************************************
PWM����
�����н����¶��޸ģ�PID�����ĸı�
����ʵ��PWM�ı�
*********************************/
typedef struct SETMSG
	{
		float Kp;
		float Ki;
		float Kd;
		
	}SETMSG;


struct pid {
	
	
	float SetTem;    //�趨���¶�
	float ActualTem;  //ʵ�ʴﵽ���¶�
	float err;           //ƫ��ֵ
	float err_last;			//��һ�ε�ƫ��ֵ
	float Kp,Ki,Kd;            //PID����
	float voltage;				//PWMռ�ձȸı�ֵ��PID�㷨�����ֵ
	float integral;            //������
}pid;


float ADC1_tem;
//ȫ�ֱ�������

	
void PWM_task(void *pvParameters)
{
	u16 led0pwmval;
	/***��Ϣ���в�������***/
	//�¶�������Ϣ���в���
	//float settemdisplay;
	float Settem;
	//ADC��Ϣ���в���
	float adc1;
	
	 u8 PWM_Algorithm_FLAG=0;
	
	SETMSG *PIDMSG;
	
	while(1)
	{
		
		
			xQueuePeek(PWM_Algorithm_Queue,&PWM_Algorithm_FLAG,portMAX_DELAY);

		
		if(PWM_Algorithm_FLAG)
		{	
		
			//ʹ��ģ���㷨
			
		}
		else
		{
			/************************************************
			��Ϣ���д���Settem
			��Դ:EMWIN����set���棬WIFI���� ������flash��ȡ
			************************************************/

			xQueuePeek(Set_Queue,(void *)&PIDMSG,portMAX_DELAY);
			xQueuePeek(Settem_Queue,&Settem,portMAX_DELAY);
		

			/************************************************
			��Ϣ���д���adc
			��Դ:ADC����
			************************************************/

			xQueuePeek(Adc_Queue,&adc1,portMAX_DELAY);
				
				
			/*************PID�ṹ��ȫ�ָ�ֵ******************/
			pid.Kp=PIDMSG->Kp;
			pid.Ki=PIDMSG->Ki;
			pid.Kd=PIDMSG->Kd;
			ADC1_tem=adc1;
				
				
				
		}//PID�㷨ѡ��
		
		/*****************PWM�ı�*****************/
		led0pwmval=pid_realize(Settem);
		
		//settemdisplay=Settem;
		//printf("PWM�Ĵ���=%d \r\n,ADC1_tem=%f \r\n,kp=%f\r\n,ki=%f \r\n,kd=%f\r\n,settem=%f \r\n",led0pwmval,adc1,pid.Kp,pid.Ki,pid.Kd,settemdisplay);

		TIM_SetCompare2(TIM3,led0pwmval);//PWM���



	xEventGroupSetBits(EventGroupHandler,EVENTBIT_2);

     vTaskDelay(1000);                          

 }

}

