#include "freertos.h"
#include "task.h"
#include "iconvdesk.h"
#include "task_emwin.h"


//���������������
void touch_task(void *pvParameters)
{
	while(1)
	{
		GUI_TOUCH_Exec();	
		vTaskDelay(10);		//��ʱ5ms
	}
}


//�������񴴽�


void emwindemo_task(void *pvParameters)
{
	

		
	
MainTask();
	
}



