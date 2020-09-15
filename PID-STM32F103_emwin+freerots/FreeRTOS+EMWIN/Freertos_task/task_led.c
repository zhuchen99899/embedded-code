#include "task_led.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "led.h"


/*******************LED任务**********************/
void LED_task(void *pvParameters)
{
	u8 key=0;
  BaseType_t err;
	extern QueueHandle_t Key_Queue;
	
	while(1)
	{
		
		err=xQueueReceive(Key_Queue,&key,portMAX_DELAY);
      if(err!=NULL)   	//消息队列Key_Queue创建成功,并且按键被按下
			{
			//printf("the message is %d\r\n",key);
				//LED1=0时开
				if (key==0)
				{
				LED1=1;
			
				}
				else if (key==1)
				{
				LED1=0;
				}
			}

        vTaskDelay(10);                           //延时10ms，也就是10个时钟节拍	

}




}


