#include "freertos.h"
#include "queue.h"
#include "QueueCreate.h"


/***********************************************************
						 ������Ϣ����
************************************************************/

//������Ϣ���е�����
#define KEYMSG_Q_NUM    1  		//������Ϣ���е�����  
#define ADCMESSAGE_Q_NUM   4   	//�������ݵ���Ϣ���е����� 
#define SETTEMMESSAGE_Q_NUM   4   	//�������ݵ���Ϣ���е����� 
#define SETPMESSAGE_Q_NUM   4   	//�������ݵ���Ϣ���е����� 
#define SETIMESSAGE_Q_NUM   4   	//�������ݵ���Ϣ���е����� 
#define SETDMESSAGE_Q_NUM   4   	//�������ݵ���Ϣ���е����� 
//static QueueHandle_t Key_Queue;   		//����ֵ��Ϣ���о��
//static QueueHandle_t Adc_Queue;	//ADC��Ϣ���о��
//static QueueHandle_t Settem_Queue;	//�¶�����Ϣ���о��
//static QueueHandle_t SetP_Queue;   		//p������Ϣ���о��
//static QueueHandle_t SetI_Queue;	//I������Ϣ���о��
//static QueueHandle_t SetD_Queue;	//D������Ϣ���о��
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

void Settem_QueueCreat(void)
{
	extern QueueHandle_t Settem_Queue;
	Settem_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(int));
	if (Settem_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("�¶�������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("�¶�������Ϣ���д����ɹ�");
	}
}

void SetP_QueueCreat(void)
{
	extern QueueHandle_t SetP_Queue;
	SetP_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(int));
	if (SetP_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("p������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("p������Ϣ���д����ɹ�");
	}
}

void SetI_QueueCreat(void)
{
	extern QueueHandle_t SetI_Queue;
	SetI_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(int));
	if (SetI_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("i������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("i������Ϣ���д����ɹ�");
	}
}

void SetD_QueueCreat(void)
{
	extern QueueHandle_t SetD_Queue;
	SetD_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(int));
	if (SetD_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("d������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("d������Ϣ���д����ɹ�");
	}
}


