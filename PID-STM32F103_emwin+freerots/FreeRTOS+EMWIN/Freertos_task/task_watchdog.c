#include "task_watchdog.h"
#include "freertos.h"
#include "task.h"
#include <EventGroupCreat.h>
#include "event_groups.h"
#include "wdg.h"


extern EventGroupHandle_t EventGroupHandler;	//事件标志组句柄
/*******************独立看门狗任务**********************/
void WATCHDOG_task(void *pvParameters)
{
EventBits_t uxBits;
	const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS; /* 最大延迟100ms */
	
	/* 
	  开始执行启动任务主函数前使能独立看门狗。
	  设置LSI是128分频，下面函数参数范围0-0xFFF，分别代表最小6.4ms，最大26214.4ms。
	  下面设置的是26s，如果26s内没有喂狗，系统复位。
	*/
	
	vTaskDelay(35000);//开机优先等待网络连接
	InitIwdg(0xFFF);
	
	/* 打印系统开机状态，方便查看系统是否复位 */
	printf("=====================================================\r\n");
	printf("=看门狗开始监管任务\r\n");
	printf("=====================================================\r\n");

    while(1)
    {
		/* 等待所有任务发来事件标志 */
			//来源于 rec 任务 ，subscribe 任务， publish任务 emwin 显示/触摸任务，PWM任务，ADC任务 。
		uxBits = xEventGroupWaitBits(EventGroupHandler, /* 事件标志组句柄 */
							         EVENTBIT_ALL,       /* 等待TASK_BIT_ALL被设置 */
							         pdTRUE,             /* 退出前TASK_BIT_ALL被清除，这里是TASK_BIT_ALL都被设置才表示“退出”*/
							         pdTRUE,             /* 设置为pdTRUE表示等待TASK_BIT_ALL都被设置*/
							         xTicksToWait); 	 /* 等待延迟时间 */
		
		if((uxBits & EVENTBIT_ALL) == EVENTBIT_ALL)
		{
			IWDG_Feed();
			printf("系统正常运行，喂狗成功\r\n");
		}
	    else
		{
			/* 基本是每xTicksToWait进来一次 */
			/* 通过变量uxBits简单的可以在此处检测那个任务长期没有发来运行标志 */ //返回置1的事件标志组的值
			printf("等待所有事件标志uxBits:%X     ",uxBits);
		}
		vTaskDelay(3000);
    }


}

