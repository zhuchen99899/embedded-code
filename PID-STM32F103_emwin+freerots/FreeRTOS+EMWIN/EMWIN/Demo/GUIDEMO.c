#include "freertos.h"
#include "task.h"
#include "iconvdesk.h"
#include "GUIDEMO.h"


//触摸任务的任务函数
void touch_task(void *pvParameters)
{
	while(1)
	{
		GUI_TOUCH_Exec();	
		vTaskDelay(5);		//延时5ms
	}
}


//触摸任务创建


void emwindemo_task(void *pvParameters)
{
	

		
	
MainTask();
	
}



