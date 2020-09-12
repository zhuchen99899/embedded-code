#include <EventGroupCreat.h>
#include <freertos.h>
#include "event_groups.h"


extern EventGroupHandle_t EventGroupHandler;	//事件标志组句柄

void EventGroupCreat(void)
{

	/* 创建事件标志组 */
	EventGroupHandler = xEventGroupCreate();
	
	if(EventGroupHandler == NULL)
    {
       printf("事件标志组创建失败");
    }

	else
	{
	
	printf("事件标志组创建成功");
	}


	}



