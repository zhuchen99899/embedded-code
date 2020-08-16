#include "freertos.h"
#include "queue.h"
#include "QueueCreate.h"


/***********************************************************
						 ������Ϣ����
************************************************************/

//������Ϣ���е�����
#define KEYMSG_Q_NUM    1  		//������Ϣ���е�����  
#define ADCMESSAGE_Q_NUM   1  	//�������ݵ���Ϣ���е����� 
#define SETMESSAGE_Q_NUM   1   	//�������ݵ���Ϣ���е����� 
#define SETTEMMESSAGE_Q_NUM   1   	//�������ݵ���Ϣ���е����� 
#define WIFIMESSAGE_buffer_Q_NUM   1   	//�������ݵ���Ϣ���е����� 
#define WIFIMESSAGE_lenth_Q_NUM   1   	//�������ݵ���Ϣ���е����� 



//static QueueHandle_t Key_Queue;   		//����ֵ��Ϣ���о��
//static QueueHandle_t Adc_Queue;	//ADC��Ϣ���о��
//static QueueHandle_t Settem_Queue;	//�¶�����Ϣ���о��
//static QueueHandle_t SetP_Queue;   		//p������Ϣ���о��
//static QueueHandle_t SetI_Queue;	//I������Ϣ���о��
//static QueueHandle_t SetD_Queue;	//D������Ϣ���о��

	//�ṹ�屣����� ������Ϣ����
//extern	struct SETMSG
//	{
//		float Kp,Ki,Kd;
//		
//	}SETMSG;




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
	Adc_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(float));
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

void Set_QueueCreat(void)
{
	extern QueueHandle_t Set_Queue;
	Set_Queue = xQueueCreate(SETMESSAGE_Q_NUM,sizeof(struct SETMSG *));
	if (Set_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("����PID��Ϣ���д���ʧ��");
	}
	else 
	{
	printf("����PID��Ϣ���д����ɹ�");
	}
}

void Settem_QueueCreat(void)
{
	extern QueueHandle_t Settem_Queue;
	Settem_Queue = xQueueCreate(SETTEMMESSAGE_Q_NUM,sizeof(int));
	if (Settem_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("�����¶���Ϣ���д���ʧ��");
	}
	else 
	{
	printf("�����¶���Ϣ���д����ɹ�");
	}
}


void WIFI_buffer_QueueCreat(void)
{
	extern QueueHandle_t Wifi_buffer_Queue;
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(uint8_t));
	
//	if (Key_Queue==0)
//	{
//	/*��Ϣ����ʧ�ܴ������*/
//	printf("WIFI��Ϣ���д���ʧ��");
//	}
//	else 
//	{
//	printf("WIFI��Ϣ���д����ɹ�");
//	}
}

void WIFI_lenth_QueueCreat(void)
{
	extern QueueHandle_t Wifi_lenth_Queue;
	Wifi_lenth_Queue =xQueueCreate(WIFIMESSAGE_lenth_Q_NUM,sizeof(uint8_t));
	
//	if (Key_Queue==0)
//	{
//	/*��Ϣ����ʧ�ܴ������*/
//	printf("WIFI��Ϣ���д���ʧ��");
//	}
//	else 
//	{
//	printf("WIFI��Ϣ���д����ɹ�");
//	}
}

void Queue_Creat(void)
{
	/******��Ϣ���д���**********/
Key_QueueCreat();
ADC_QueueCreat();
Set_QueueCreat();
Settem_QueueCreat();
WIFI_buffer_QueueCreat();
WIFI_lenth_QueueCreat();
}



