#include "freertos.h"
#include "task.h"
#include "iconvdesk.h"
#include "task_emwin.h"
#include <EventGroupCreat.h>
#include "event_groups.h"

extern EventGroupHandle_t EventGroupHandler;	//�¼���־����

//���������������
void touch_task(void *pvParameters)
{
	while(1)
	{
		GUI_TOUCH_Exec();	
		xEventGroupSetBits(EventGroupHandler,EVENTBIT_1);
		vTaskDelay(10);		//��ʱ5ms
	}
}


//�������񴴽�


void emwindemo_task(void *pvParameters)
{
	

		
	
MainTask();
	
}



