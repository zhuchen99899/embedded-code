#include "task_FreertosTimer_Callback.h"
#include "freertos.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"


/********Freertos�����ʱ������******************/
//�����������ʱ��
extern TimerHandle_t 	AutoReloadTimer_For_MqttConnectErr_Handle;			
extern TimerHandle_t   AutoReloadTimer_For_MqttPingreqErr_Handle;
extern TimerHandle_t   AutoReloadTimer_For_MqttSubscribeErr_Handle;

/***************��ֵ�ź������****************/
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET���Ķ�ֵ�ź������
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE���Ķ�ֵ�ź������
//���ڶ�ʱ��1�ص�������CONNECT���ĺ�����ʱ����5S��û�н���CONNACK���� �ڶ�ʱ���ص����ٴ��ͷ�CONNECT�����ź��������رն�ʱ��
void AutoReloadCallback_MqttConnectErr(TimerHandle_t xTimer)
{

	
	
	xSemaphoreGive(BinarySemaphore_MQTTconnect);//����MQTT Connect�����ź�
	printf("���ʱ����CONNACK����δӦ�𣬶�ʱ���ص������·��� CONNECT�ź�\r\n");
	printf("�ر�CONNECT����Ӧ���ⶨʱ��\r\n");
	xTimerStop(AutoReloadTimer_For_MqttConnectErr_Handle,0); 	//�ر����ڶ�ʱ��
	
	
}


//���ڶ�ʱ��2������pingreq���ĺ�����3min��û�н��յ�pingreq���Ĵ����رն�ʱ��ָ��ڻص��������ٴ��ͷ�CONNECT�����ź������ٹرն�ʱ��
void AutoReloadCallback_MqttPingreqErr(TimerHandle_t xTimer)
{

	xSemaphoreGive(BinarySemaphore_MQTTconnect);//����MQTT Connect�����ź�
	printf("���ʱ����PINGREQ����δӦ�𣬶�ʱ���ص������·��� CONNECT�ź�\r\n");
	printf("�ر�PINGREQ����Ӧ���ⶨʱ��\r\n");
	xTimerStop(AutoReloadTimer_For_MqttPingreqErr_Handle,0); 	//�ر����ڶ�ʱ��
	
	
	
}


//���ڶ�ʱ��3������subscribe���ĺ�����5s��û�н��յ�suback���ģ��ڶ�ʱ���ص����ٴ��ͷ�subscribe�ź������ظ����μ���flag,
	//�����û��suback����(�ڻص����ж�flag),��ǰ��flag��0�����ٴ��ͷ�CONNECT�ź��������رն�ʱ��			
void AutoReloadCallback_MqttSubscribeErr(TimerHandle_t xTimer)
{
static u8 suback_errflag=0;
	
	suback_errflag++;
	
	if(suback_errflag==3)
	{
	printf("���ʱ����subscribe����δӦ��3�Σ���ʱ���ص������·���Connect�����ź���\r\n");
	xSemaphoreGive(BinarySemaphore_MQTTconnect);//����MQTT Connect�����ź�
	suback_errflag=0;
	printf("�ر�SUBSCRIBE����Ӧ���ⶨʱ��\r\n");
	xTimerStop(AutoReloadTimer_For_MqttSubscribeErr_Handle,0); 	//�ر����ڶ�ʱ��
	
	}
	else
	{
		
	suback_errflag++;
		
	printf("���ʱ����subscribe����δӦ�����·���subscribe�����ź�\r\n");
	xSemaphoreGive(BinarySemaphore_MQTTsubscribe);//����MQTT SUBSCRIBE�����ź�	
		printf("�ر�SUBSCRIBE����Ӧ���ⶨʱ��\r\n");
	xTimerStop(AutoReloadTimer_For_MqttSubscribeErr_Handle,0); 	//�ر����ڶ�ʱ��
		
	}
	
}



