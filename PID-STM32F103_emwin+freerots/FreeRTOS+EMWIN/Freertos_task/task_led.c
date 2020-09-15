#include "task_led.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "led.h"


/*******************LED����**********************/
void LED_task(void *pvParameters)
{
	u8 key=0;
  BaseType_t err;
	extern QueueHandle_t Key_Queue;
	
	while(1)
	{
		
		err=xQueueReceive(Key_Queue,&key,portMAX_DELAY);
      if(err!=NULL)   	//��Ϣ����Key_Queue�����ɹ�,���Ұ���������
			{
			//printf("the message is %d\r\n",key);
				//LED1=0ʱ��
				if (key==0)
				{
				LED1=1;
			
				}
				else if (key==1)
				{
				LED1=0;
				}
			}

        vTaskDelay(10);                           //��ʱ10ms��Ҳ����10��ʱ�ӽ���	

}




}


