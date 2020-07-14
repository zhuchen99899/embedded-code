#include "freertos.h"
#include "queue.h"
#include "QueueCreate.h"


/***********************************************************
						 ������Ϣ����
************************************************************/

//������Ϣ���е�����
#define KEYMSG_Q_NUM    1  		//������Ϣ���е�����  
#define ADCMESSAGE_Q_NUM   4   	//�������ݵ���Ϣ���е����� 
//static QueueHandle_t Key_Queue;   		//����ֵ��Ϣ���о��
//static QueueHandle_t Message_Queue;	//��Ϣ���о��
//������Ϣ����
void Key_QueueCreat(void)
{
	extern QueueHandle_t Key_Queue;
	Key_Queue =xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));
	
//	if (Key_Queue==0)
//	{
//	/*��Ϣ����ʧ�ܴ������*/
//	printf("������Ϣ���д���ʧ��");
//	}
//	else 
//	{
//	printf("������Ϣ���д����ɹ�");
//	}
}


void ADC_QueueCreat(void)
{
	extern QueueHandle_t Adc_Queue;
	Adc_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(int));
//	if (Adc_Queue==0)
//	{
//	/*��Ϣ����ʧ�ܴ������*/
//	printf("ADC��Ϣ���д���ʧ��");
//	}
//	else 
//	{
//	printf("ADC��Ϣ���д����ɹ�");
//	}
}






