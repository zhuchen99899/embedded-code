#include "freertos.h"
#include "queue.h"
#include "QueueCreate.h"


/***********************************************************
						 ������Ϣ����
************************************************************/

//������Ϣ���е�����
#define KEYMSG_Q_NUM    1  		//������Ϣ���е�����  
#define ADCMESSAGE_Q_NUM   1  	//ADC��Ϣ���е����� 
#define SETMESSAGE_Q_NUM   1   	//�����¶���Ϣ���е����� 
#define SETTEMMESSAGE_Q_NUM   1   	//����PID��Ϣ���е����� 
#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi�������ݵ���Ϣ���е����� 
#define PINGREQ_Q_NUM   1   	//PINGREQ����Ϣ���е�����  

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
	



//������Ϣ����
void Key_QueueCreat(void)
{
	extern QueueHandle_t Key_Queue;
	Key_Queue =xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));
	
	if (Key_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("������Ϣ���д����ɹ�");
	}
}


void ADC_QueueCreat(void)
{
	extern QueueHandle_t Adc_Queue;
	Adc_Queue = xQueueCreate(ADCMESSAGE_Q_NUM,sizeof(float));
	if (Adc_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("ADC��Ϣ���д���ʧ��");
	}
	else 
	{
	printf("ADC��Ϣ���д����ɹ�");
	}
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
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(struct WIFIMSG *));
	
	if (Wifi_buffer_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("MQTT������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("MQTT������Ϣ���д����ɹ�");
	}
}

void PINGREQ_QueueCreat(void)
{
	extern QueueHandle_t PINGREQ_Queue;
	PINGREQ_Queue =xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));
	
	if (PINGREQ_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("PINGREQ��Ϣ���д���ʧ��");
	}
	else 
	{
	printf("PINGREQ��Ϣ���д����ɹ�");
	}
}

void Queue_Creat(void)
{
	/******��Ϣ���д���**********/
Key_QueueCreat();
ADC_QueueCreat();
Set_QueueCreat();
Settem_QueueCreat();
WIFI_buffer_QueueCreat();
PINGREQ_QueueCreat();


}



