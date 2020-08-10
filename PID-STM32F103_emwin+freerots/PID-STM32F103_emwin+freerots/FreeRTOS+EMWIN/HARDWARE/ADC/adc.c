#include "adc.h"
#include "tem.h"
#include "stm32f10x.h"
#include "delay.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"
void Adc_Init(void)
{
GPIO_InitTypeDef GPIO_InitStrue;
	ADC_InitTypeDef ADC_InitStrue;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);//GPIOA��ADC1 RCCʱ��ʹ��
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Init(GPIOA,&GPIO_InitStrue); //PA1������λADC1ģʽ
	
RCC_ADCCLKConfig(RCC_PCLK2_Div6); //ADC��ʱ�Ӳ��ܳ���14M,PCLK2Ϊ72M������Ϊ6��Ƶ
	
	ADC_DeInit(ADC1); //��λADC1
	
	ADC_InitStrue.ADC_ContinuousConvMode=DISABLE; //��ʹ������ת��ģʽ
	ADC_InitStrue.ADC_DataAlign=ADC_DataAlign_Right; //�����Ҷ���ģʽ��STM32��12λADC����16λ�Ĵ�����12λ���ݴ洢�Ҷ���
	ADC_InitStrue.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//��ʹ���ⲿ����ADCģʽ��(���ADCģʽ��
	ADC_InitStrue.ADC_Mode=ADC_Mode_Independent;//����ADCģʽ
	ADC_InitStrue.ADC_NbrOfChannel=1; //��ͨ��ADC
	ADC_InitStrue.ADC_ScanConvMode=DISABLE; //��ʹ��ɨ��ģʽ
	
	ADC_Init(ADC1,&ADC_InitStrue);//ADC��ʼ��
	
	ADC_Cmd(ADC1,ENABLE);//ʹ��ADC1
	
	//ADCУ׼
	ADC_ResetCalibration(ADC1); //ʹ�ܸ�λУ׼
	
	while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);//����ADУ׼
	
	while(ADC_GetCalibrationStatus(ADC1));//�ȴ�ADУ׼����
}
u16 Get_Adc (u8 ch)
{
	/*ch ��Ч��
 ADC_Channel_0                              
 ADC_Channel_1                            
 ADC_Channel_2                           
 ADC_Channel_3                            
 ADC_Channel_4                          
 ADC_Channel_5                         
 ADC_Channel_6                           
 ADC_Channel_7                              
 ADC_Channel_8                         
 ADC_Channel_9                               
 ADC_Channel_10                              
 ADC_Channel_11                            
 ADC_Channel_12                              
 ADC_Channel_13                              
 ADC_Channel_14                             
 ADC_Channel_15                             
 ADC_Channel_16                            
 ADC_Channel_17     
	*/
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5); //����ͨ�����裺ADC�����ڼ���ͨ����rank=��ͨ������������������ǵڼ���ת����ֻ��1��ͨ������Ҳֻ��1��ת����������ʱ��
	
ADC_SoftwareStartConvCmd(ADC1,ENABLE);//�������ת��

while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//�ȴ�ת������

return ADC_GetConversionValue(ADC1); //��ȡת��ֵ
}



/****************************************
��ȡADC��ֵ
*****************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{

u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
	temp_val+=Get_Adc(ch);
	// vTaskDelay(5);    
	}

return temp_val/times;
}


/***************************************
����˲�  ȥ����ֵƽ���˲�
��ת���¶�ֵ
****************************************/
		//ȡ14����
		
float filtering(u16 adcx)
{
	int a,b,c,i,j;
	float getadcx[14];      //�洢15��adcֵ
	float temp;
	float adctemp=0;
	float receive=0;
	float res;
	float ADC1_tem=0;
		for(j=0;j<14;j++)              //ȡ14��ADCֵ �洢��getadcx����
		{
		temp=(float)adcx*(3.0/4096); //STM32��12λ��ADC ,2��12�η�=4096
		getadcx[j]=temp;
		}                    

		//ð������
		for(a=0;a<14;a++)       
		{
     		for(b=0;b<13;b++)
			{
				
				if(getadcx[b]>getadcx[b+1])
				{

			adctemp=getadcx[b];
			getadcx[b+1]=getadcx[b];
			getadcx[b]=adctemp;

				}
			}

		}
		
		
		//ȥ�������������Сֵ��ȡƽ����
		for(i=2;i<11;i++)    //�����˲���ȥ���������ֵ��������Сֵ��Ȼ�����
		{
		c+=getadcx[i];
		}
		c=c/10               ;        //ȡƽ��ֵ
		
	
		receive=c;
		
		

		
/*��ѹת��Ϊ����*/
		res=((receive/10.0)/0.909)*1000.0;
		
		
		
/*���ת��Ϊ�¶�*/	
		ADC1_tem=(float)get_tem(res);	

		printf("%3.1f\r\n",ADC1_tem);
		
return ADC1_tem;
		
	}


/*********************************************
ADC����
**********************************************/

void ADC_task(void *pvParameters)
{
	 BaseType_t err;
	extern QueueHandle_t Adc_Queue;
		float adcmsg=0;
	//��Ϣ���в���

	u16 adcx=0;		
	float adctemp; //ת����STM32 12bitADC
	float res;
	while(1)
	{
			adcx=Get_Adc_Average(ADC_Channel_1,10);//��ADCͨ����ȡ��ֵ
			adctemp=(float)adcx*(3.0/4096); //STM32��12λ��ADC ,2��12�η�=4096
			res=((adctemp/10.0)/0.909)*1000.0;//ת������
			adcmsg=(float)get_tem(res);  //���ת��Ϊ�¶�
			err=xQueueSend(Adc_Queue,&adcmsg,portMAX_DELAY);		
			if(Adc_Queue!=NULL)   	//��Ϣadc_Queue���ͳɹ�
				{
				 printf("ADC��Ϣ���ͳɹ�\r\n");
				 printf("���͵���Ϣ��%f\r\n",adcmsg); 
					 vTaskDelay(500);  
				 }
					
				 else
				{
					printf("ADC��Ϣ����ʧ�� :%ld" ,err);
				}
				
				
				  vTaskDelay(10);                           //��ʱ10ms��Ҳ����10��ʱ�ӽ���	
	}

      

}


