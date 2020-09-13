#include "task_watchdog.h"
#include "freertos.h"
#include "task.h"
#include <EventGroupCreat.h>
#include "event_groups.h"
#include "wdg.h"


extern EventGroupHandle_t EventGroupHandler;	//�¼���־����
/*******************�������Ź�����**********************/
void WATCHDOG_task(void *pvParameters)
{
EventBits_t uxBits;
	const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS; /* ����ӳ�100ms */
	
	/* 
	  ��ʼִ����������������ǰʹ�ܶ������Ź���
	  ����LSI��128��Ƶ�����溯��������Χ0-0xFFF���ֱ������С6.4ms�����26214.4ms��
	  �������õ���26s�����26s��û��ι����ϵͳ��λ��
	*/
	
	vTaskDelay(35000);//�������ȵȴ���������
	InitIwdg(0xFFF);
	
	/* ��ӡϵͳ����״̬������鿴ϵͳ�Ƿ�λ */
	printf("=====================================================\r\n");
	printf("=���Ź���ʼ�������\r\n");
	printf("=====================================================\r\n");

    while(1)
    {
		/* �ȴ������������¼���־ */
			//��Դ�� rec ���� ��subscribe ���� publish���� emwin ��ʾ/��������PWM����ADC���� ��
		uxBits = xEventGroupWaitBits(EventGroupHandler, /* �¼���־���� */
							         EVENTBIT_ALL,       /* �ȴ�TASK_BIT_ALL������ */
							         pdTRUE,             /* �˳�ǰTASK_BIT_ALL�������������TASK_BIT_ALL�������òű�ʾ���˳���*/
							         pdTRUE,             /* ����ΪpdTRUE��ʾ�ȴ�TASK_BIT_ALL��������*/
							         xTicksToWait); 	 /* �ȴ��ӳ�ʱ�� */
		
		if((uxBits & EVENTBIT_ALL) == EVENTBIT_ALL)
		{
			IWDG_Feed();
			printf("ϵͳ�������У�ι���ɹ�\r\n");
		}
	    else
		{
			/* ������ÿxTicksToWait����һ�� */
			/* ͨ������uxBits�򵥵Ŀ����ڴ˴�����Ǹ�������û�з������б�־ */ //������1���¼���־���ֵ
			printf("�ȴ������¼���־uxBits:%X     ",uxBits);
		}
		vTaskDelay(3000);
    }


}

