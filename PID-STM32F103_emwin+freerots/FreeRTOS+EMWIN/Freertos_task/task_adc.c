#include "adc.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "tem.h"
#include "task_adc.h"


/*********************************************
ADC任务
**********************************************/

void ADC_task(void *pvParameters)
{

	extern QueueHandle_t Adc_Queue;

	//消息队列参数
		float adcmsg=0;
	u16 adcx=0;		
	float adctemp; //转换成STM32 12bitADC
	float res;
	while(1)
	{
			adcx=Get_Adc_Average(ADC_Channel_1,10);//从ADC通道获取数值
			adctemp=(float)adcx*(3.0/4096); //STM32是12位的ADC ,2的12次方=4096
			res=((adctemp/10.0)/0.909)*1000.0;//转换电阻
			adcmsg=(float)get_tem(res);  //查表转换为温度
			xQueueOverwrite(Adc_Queue,&adcmsg);		

				
				  vTaskDelay(1000);                           //延时10ms，也就是10个时钟节拍	
	}

      

}


