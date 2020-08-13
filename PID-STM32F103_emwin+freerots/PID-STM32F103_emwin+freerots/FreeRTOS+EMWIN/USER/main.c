#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "adc.h"
#include "timer.h"
#include "ILI93xx.h"
#include "key.h"
#include "timer.h"
#include "sram.h"
#include "24cxx.h"
#include "touch.h"
#include "malloc.h"
#include "w25qxx.h"
#include "pwm.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "limits.h"
#include "GUI.h"
#include "WM.h"
#include "GUIDEMO.h"
#include "queue.h"
#include "QueueCreate.h"
#include "stmflash.h"

/***********************************************************
						��������
************************************************************/


//��Ϣ����ȫ�ֱ���
QueueHandle_t Key_Queue;
QueueHandle_t Adc_Queue;
QueueHandle_t Settem_Queue;
QueueHandle_t SetP_Queue;
QueueHandle_t SetI_Queue;
QueueHandle_t SetD_Queue;

//flash��ȡ��ַ������

#define TEXT_MAXLEN 8	//���鳤��
#define SIZE TEXT_MAXLEN		//���鳤��
	static u8 datatempTem[TEXT_MAXLEN];
	static u8 datatempP[TEXT_MAXLEN];
	static u8 datatempI[TEXT_MAXLEN];
	static u8 datatempD[TEXT_MAXLEN];
//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
#define FLASH_SAVE_ADDR_TEM  0x0807FE60	
#define FLASH_SAVE_ADDR_P    0x0807FE50	
#define FLASH_SAVE_ADDR_I  	 0x0807FF60 
#define FLASH_SAVE_ADDR_D    0x0807FF50

/***********************************************************
					����������
************************************************************/

void start_task(void *pvParameters);		//Start����
void touch_task(void *pvParameters);		//TOUCH����
void UserIf_task(void *pvParameters);		//�ӿ���Ϣ����
void emwindemo_task(void *pvParameters); 	//emwin����
void LED_task(void *pvParameters);       // LED����
void ADC_task(void *pvParameters);       // ADC����
void PWM_task(void *pvParameters);       // PWM����
/***********************************************************
						���������
************************************************************/
//���������
TaskHandle_t StartTask_Handler;			//Start����
TaskHandle_t TouchTask_Handler;			//TOUCH����
TaskHandle_t UserIfTask_Handler;		//�ӿ���Ϣ����
TaskHandle_t EmwindemoTask_Handler;		//emwin����
TaskHandle_t LedTask_Handler;		//LED����
TaskHandle_t Adc_task_Handler;		//ADC����
TaskHandle_t PWM_task_Handler;		//PWM����
/***********************************************************
						�����ջ��С
************************************************************/
#define START_STK_SIZE 			256  	//Start����
#define TOUCH_STK_SIZE			128		//TOUCH����
#define USERIF_STK_SIZE			512		//�ӿ���Ϣ����
#define EMWINDEMO_STK_SIZE		512		//emwin����
#define LED_STK_SIZE		128	//LED����
#define ADC_STK_SIZE		512	//ADC����
#define PWM_STK_SIZE		128	//PWM����
/***********************************************************
						 �������ȼ�(��ֵԽС���ȼ�Խ��)
************************************************************/
#define START_TASK_PRIO			4		//Start����
#define TOUCH_TASK_PRIO			1		//TOUCH����
#define USERIF_TASK_PRIO 		1	    //�ӿ���Ϣ����
#define EMWINDEMO_TASK_PRIO		2		//emwin����
#define LED_TASK_PRIO		1		//LED����
#define ADC_TASK_PRIO		3		//ADC����
#define PWM_TASK_PRIO		3		//PWM����

/***********************************************************
						 ���������
************************************************************/

int main(void)
{
	
/*
 ����������ǰ��Ϊ�˷�ֹ��ʼ�� STM32 ����ʱ���жϷ������ִ�У������ֹȫ���ж�(���� NMI �� HardFault)��
 �������ĺô��ǣ�
 1. ��ִֹ�е��жϷ���������� FreeRTOS �� API ������
 2. ��֤ϵͳ�������������ܱ���ж�Ӱ�졣
 3. �����Ƿ�ر�ȫ���жϣ���Ҹ����Լ���ʵ��������ü��ɡ�
 ����ֲ�ļ� port.c �еĺ��� prvStartFirstTask �л����¿���ȫ���жϡ�ͨ��ָ�� cpsie i ������__set_PRIMASK(1)
 �� cpsie i �ǵ�Ч�ġ�
 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	 
	uart_init(115200);					//��ʼ������
	Adc_Init();                //ADC��ʼ��
	LED_Init();		  					//��ʼ��LED
	KEY_Init();							//��ʼ������
	TFTLCD_Init();						//LCD��ʼ��	
	FSMC_SRAM_Init();					//��ʼ��SRAM
	TP_Init();							//��������ʼ��
	TIM3_PWM_Init(1439,0);//1439+1=1440,����Ƶ��Ƶ��Ϊ72000000/1440=50khz
	TIM4_Int_Init(1799,0);//PWM��ʼ��
	my_mem_init(SRAMIN);            	//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);				//��ʼ���ⲿ�ڴ��
	
	

	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������                
    vTaskStartScheduler();          //�����������
/*
 ���ϵͳ���������ǲ������е�����ģ����е����Ｋ�п��������ڶ�ʱ��������߿��������
 heap �ռ䲻����ɴ���ʧ�ܣ���Ҫ�Ӵ� FreeRTOSConfig.h �ļ��ж���� heap ��С��
 #define configTOTAL_HEAP_SIZE ( ( size_t ) ( 17 * 1024 ) )
*/
	while(1);			
}

/***********************************************************
						 ������
************************************************************/

//��ʼ����������
void start_task(void *pvParameters)
{
	float Kp,Ki,Kd,settem;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//����CRCʱ��

	GUI_Init();  					//STemWin��ʼ��
	WM_SetCreateFlags(WM_CF_MEMDEV);//ewmin����ʹ�洢�豸�����ؼ��봰��
	
	
    taskENTER_CRITICAL();           //�����ٽ���
	
	/***********����ʱ��ȡflash��PID�����Լ������¶�**************/
	STMFLASH_Read(FLASH_SAVE_ADDR_TEM,(u16*)datatempTem,SIZE);
	STMFLASH_Read(FLASH_SAVE_ADDR_P,(u16*)datatempP,SIZE);
	STMFLASH_Read(FLASH_SAVE_ADDR_I,(u16*)datatempI,SIZE);
	STMFLASH_Read(FLASH_SAVE_ADDR_D,(u16*)datatempD,SIZE);
	Kp=(float)atof((char *)datatempP);
  Ki=(float)atof((char *)datatempI);
  Kd=(float)atof((char *)datatempD);
  settem=(float)atof((char *)datatempTem);


	//������Ϣ����
	Queue_Creat();
	
	//��Ϣ���з���flash�ڴ��е�PID����
		xQueueSend(SetP_Queue,&Kp,portMAX_DELAY);			
		xQueueSend(SetI_Queue,&Ki,portMAX_DELAY);		
		xQueueSend(SetD_Queue,&Kd,portMAX_DELAY);	
		xQueueSend(Settem_Queue,&settem,portMAX_DELAY);
	/**********************************���񴴽�******************************/
	
	//������������
    xTaskCreate((TaskFunction_t )touch_task,             
                (const char*    )"touch_task",           
                (uint16_t       )TOUCH_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TOUCH_TASK_PRIO,        
                (TaskHandle_t*  )&TouchTask_Handler);   	
    //����UserIf���ڴ�ӡ����
    xTaskCreate((TaskFunction_t )UserIf_task,             
                (const char*    )"UserIf",           
                (uint16_t       )USERIF_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )USERIF_TASK_PRIO,        
                (TaskHandle_t*  )&UserIfTask_Handler);  
    //����EMWIN Demo����
    xTaskCreate((TaskFunction_t )emwindemo_task,             
                (const char*    )"emwindemo_task",           
                (uint16_t       )EMWINDEMO_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EMWINDEMO_TASK_PRIO,        
                (TaskHandle_t*  )&EmwindemoTask_Handler);
		//����LED����
								
								
		    xTaskCreate((TaskFunction_t )LED_task,             
                (const char*    )"LED_task",           
                (uint16_t       )LED_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LED_TASK_PRIO,//
                (TaskHandle_t*  )&LedTask_Handler);
								
		//����ADC����			
		    xTaskCreate((TaskFunction_t )ADC_task,             
                (const char*    )"ADC_task",           
                (uint16_t       )ADC_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )ADC_TASK_PRIO,//
                (TaskHandle_t*  )&Adc_task_Handler);
		
								
		//����PWM����			
		   xTaskCreate((TaskFunction_t )PWM_task,             
                (const char*    )"PWM_task",           
                (uint16_t       )PWM_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )PWM_TASK_PRIO,//
                (TaskHandle_t*  )&PWM_task_Handler);
		
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}





//ͳ������
void UserIf_task(void *p_arg)
{
	uint8_t ucKeyCode;
	uint8_t pcWriteBuffer[500];
		while(1)
		{
		ucKeyCode = KEY_Scan(1);

			switch (ucKeyCode)
			{
			/* K2 ������ ��ӡ����ִ����� */
			case 2:
				printf("=================================================\r\n");
				printf("������ ����״̬ ���ȼ� ʣ��ջ �������\r\n");
				vTaskList((char *)&pcWriteBuffer);
				printf("%s\r\n", pcWriteBuffer);
				printf("\r\n ������ ���м��� ʹ����\r\n");
				vTaskGetRunTimeStats((char *)&pcWriteBuffer);
				printf("%s\r\n", pcWriteBuffer);
				break;
				/* �����ļ�ֵ������ */
				default:
				break;
				}
			vTaskDelay(20);
			}
		
		
}

