#include "task_userif.h"
#include "FreeRTOS.h"
#include "task.h"
#include "key.h"

//统计任务（空闲任务）
void UserIf_task(void *p_arg)
{
	uint8_t ucKeyCode;
	uint8_t pcWriteBuffer[500];
		while(1)
		{
		ucKeyCode = KEY_Scan(1);

			switch (ucKeyCode)
			{
			/* K2 键按下 打印任务执行情况 */
			case 2:
				printf("=================================================\r\n");
				printf("任务名 任务状态 优先级 剩余栈 任务序号\r\n");
				vTaskList((char *)&pcWriteBuffer);
				printf("%s\r\n", pcWriteBuffer);
				printf("\r\n 任务名 运行计数 使用率\r\n");
				vTaskGetRunTimeStats((char *)&pcWriteBuffer);
				printf("%s\r\n", pcWriteBuffer);
				break;
				/* 其他的键值不处理 */
				default:
				break;
				}
			vTaskDelay(20);
			}
		
		
}
