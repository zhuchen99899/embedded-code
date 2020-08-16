#include "adc.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "tem.h"
#include "task_adc.h"


/*********************************************
ADC����
**********************************************/

void ADC_task(void *pvParameters)
{

	extern QueueHandle_t Adc_Queue;

	//��Ϣ���в���
		float adcmsg=0;
	u16 adcx=0;		
	float adctemp; //ת����STM32 12bitADC
	float res;
	while(1)
	{
			adcx=Get_Adc_Average(ADC_Channel_1,10);//��ADCͨ����ȡ��ֵ
			adctemp=(float)adcx*(3.0/4096); //STM32��12λ��ADC ,2��12�η�=4096
			res=((adctemp/10.0)/0.909)*1000.0;//ת������
			adcmsg=(float)get_tem(res);  //���ת��Ϊ�¶�
			xQueueOverwrite(Adc_Queue,&adcmsg);		

				
				  vTaskDelay(1000);                           //��ʱ10ms��Ҳ����10��ʱ�ӽ���	
	}

      

}


