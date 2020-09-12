#include "freertos.h"
#include "task.h"
#include "iconvdesk.h"
#include "task_emwin.h"
#include <EventGroupCreat.h>
#include "event_groups.h"

extern EventGroupHandle_t EventGroupHandler;	//事件标志组句柄

//触摸任务的任务函数
void touch_task(void *pvParameters)
{
	while(1)
	{
		GUI_TOUCH_Exec();	
		xEventGroupSetBits(EventGroupHandler,EVENTBIT_1);
		vTaskDelay(10);		//延时5ms
	}
}


//触摸任务创建


void emwindemo_task(void *pvParameters)
{
	

		
	
MainTask();
	
}



