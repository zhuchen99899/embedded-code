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
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);//GPIOA和ADC1 RCC时钟使能
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Init(GPIOA,&GPIO_InitStrue); //PA1口设置位ADC1模式
	
RCC_ADCCLKConfig(RCC_PCLK2_Div6); //ADC的时钟不能超过14M,PCLK2为72M，至少为6分频
	
	ADC_DeInit(ADC1); //复位ADC1
	
	ADC_InitStrue.ADC_ContinuousConvMode=DISABLE; //不使用连续转换模式
	ADC_InitStrue.ADC_DataAlign=ADC_DataAlign_Right; //数据右对齐模式。STM32是12位ADC，有16位寄存器，12位数据存储右对齐
	ADC_InitStrue.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//不使用外部触发ADC模式。(软件ADC模式）
	ADC_InitStrue.ADC_Mode=ADC_Mode_Independent;//独立ADC模式
	ADC_InitStrue.ADC_NbrOfChannel=1; //单通道ADC
	ADC_InitStrue.ADC_ScanConvMode=DISABLE; //不使用扫描模式
	
	ADC_Init(ADC1,&ADC_InitStrue);//ADC初始化
	
	ADC_Cmd(ADC1,ENABLE);//使能ADC1
	
	//ADC校准
	ADC_ResetCalibration(ADC1); //使能复位校准
	
	while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束
	
	ADC_StartCalibration(ADC1);//开启AD校准
	
	while(ADC_GetCalibrationStatus(ADC1));//等待AD校准结束
}
u16 Get_Adc (u8 ch)
{
	/*ch 有效性
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
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5); //规则通道配设：ADC名，第几个通道，rank=此通道在这个规则序列中是第几个转换（只有1个通道所以也只有1次转换），采样时间
	
ADC_SoftwareStartConvCmd(ADC1,ENABLE);//开启软件转换

while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);//等待转换结束

return ADC_GetConversionValue(ADC1); //获取转换值
}



/****************************************
获取ADC数值
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
软件滤波  去极限值平均滤波
并转换温度值
****************************************/
		//取14次数
		
float filtering(u16 adcx)
{
	int a,b,c,i,j;
	float getadcx[14];      //存储15个adc值
	float temp;
	float adctemp=0;
	float receive=0;
	float res;
	float ADC1_tem=0;
		for(j=0;j<14;j++)              //取14次ADC值 存储到getadcx数组
		{
		temp=(float)adcx*(3.0/4096); //STM32是12位的ADC ,2的12次方=4096
		getadcx[j]=temp;
		}                    

		//冒泡排序
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
		
		
		//去两个最大两个最小值后取平均数
		for(i=2;i<11;i++)    //算数滤波，去除两个最大值和两个最小值，然后求和
		{
		c+=getadcx[i];
		}
		c=c/10               ;        //取平均值
		
	
		receive=c;
		
		

		
/*电压转换为电阻*/
		res=((receive/10.0)/0.909)*1000.0;
		
		
		
/*查表转换为温度*/	
		ADC1_tem=(float)get_tem(res);	

		printf("%3.1f\r\n",ADC1_tem);
		
return ADC1_tem;
		
	}


/*********************************************
ADC任务
**********************************************/

void ADC_task(void *pvParameters)
{
	 BaseType_t err;
	extern QueueHandle_t Adc_Queue;
		float adcmsg=0;
	//消息队列参数

	u16 adcx=0;		
	float adctemp; //转换成STM32 12bitADC
	float res;
	while(1)
	{
			adcx=Get_Adc_Average(ADC_Channel_1,10);//从ADC通道获取数值
			adctemp=(float)adcx*(3.0/4096); //STM32是12位的ADC ,2的12次方=4096
			res=((adctemp/10.0)/0.909)*1000.0;//转换电阻
			adcmsg=(float)get_tem(res);  //查表转换为温度
			err=xQueueSend(Adc_Queue,&adcmsg,portMAX_DELAY);		
			if(Adc_Queue!=NULL)   	//消息adc_Queue发送成功
				{
				 printf("ADC消息发送成功\r\n");
				 printf("发送的消息是%f\r\n",adcmsg); 
					 vTaskDelay(500);  
				 }
					
				 else
				{
					printf("ADC消息发送失败 :%ld" ,err);
				}
				
				
				  vTaskDelay(10);                           //延时10ms，也就是10个时钟节拍	
	}

      

}


