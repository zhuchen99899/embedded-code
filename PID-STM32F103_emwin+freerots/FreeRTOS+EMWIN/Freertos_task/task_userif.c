#include "task_userif.h"
#include "FreeRTOS.h"
#include "task.h"
#include "key.h"

//ͳ�����񣨿�������
void UserIf_task(void *p_arg)
{
	uint8_t ucKeyCode;
	uint8_t pcWriteBuffer[500];
		while(1)
		{
		ucKeyCode = KEY_Scan(1);

			switch (ucKeyCode)
			{
			/* K2 ������ ��ӡ����ִ����� */
			case 2:
				printf("=================================================\r\n");
				printf("������ ����״̬ ���ȼ� ʣ��ջ �������\r\n");
				vTaskList((char *)&pcWriteBuffer);
				printf("%s\r\n", pcWriteBuffer);
				printf("\r\n ������ ���м��� ʹ����\r\n");
				vTaskGetRunTimeStats((char *)&pcWriteBuffer);
				printf("%s\r\n", pcWriteBuffer);
				break;
				/* �����ļ�ֵ������ */
				default:
				break;
				}
			vTaskDelay(20);
			}
		
		
}
