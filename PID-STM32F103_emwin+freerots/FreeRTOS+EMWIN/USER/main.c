#include "includes.h"

/***********************************************************
						��������
************************************************************/


//��Ϣ���о��
QueueHandle_t Key_Queue;
QueueHandle_t Adc_Queue;
QueueHandle_t Set_Queue;
QueueHandle_t Settem_Queue;
QueueHandle_t Wifi_buffer_Queue;
//��ֵ�ź������
SemaphoreHandle_t BinarySemaphore;	//��ֵ�ź������

//�����ڲ�flash��ȡ���
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

 	//�ṹ�屣����� ������Ϣ����
typedef struct SETMSG
	{
		float Kp;
		float Ki;
		float Kd;
		
	}SETMSG;

SETMSG g_tMsg;

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
void MQTT_send_task(void *pvParameters);       // MQTT��������
void MQTT_rec_task(void *pvParameters);				// MQTT��������
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
TaskHandle_t MQTT_Send_task_Handler;		  // MQTT��������
TaskHandle_t MQTT_Rec_task_Handler;		  // MQTT��������
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
#define MQTT_Send_STK_SIZE		512	// MQTT��������
#define MQTT_Rec_STK_SIZE		512	// MQTT��������
/***********************************************************
						 �������ȼ�(��ֵԽС���ȼ�Խ��)
************************************************************/
#define START_TASK_PRIO			4		//Start����
#define TOUCH_TASK_PRIO			0	//TOUCH����
#define USERIF_TASK_PRIO 		0	    //�ӿ���Ϣ����
#define EMWINDEMO_TASK_PRIO		0		//emwin����
#define LED_TASK_PRIO		1		//LED����
#define ADC_TASK_PRIO		2		//ADC����
#define PWM_TASK_PRIO		2		//PWM����
#define MQTT_Send_TASK_PRIO		2		// MQTT��������
#define MQTT_Rec_TASK_PRIO		2		// MQTT��������
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
	wifi_init(115200);					//��ʼ��wifi
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
SETMSG *Flashdata;
float settem;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//����CRCʱ��

	GUI_Init();  					//STemWin��ʼ��
	WM_SetCreateFlags(WM_CF_MEMDEV);//ewmin����ʹ�洢�豸�����ؼ��봰��
	
	
    taskENTER_CRITICAL();           //�����ٽ���
	
	/***********����ʱ��ȡflash��PID�����Լ������¶�**************/
	Flashdata= &g_tMsg; //�ṹ��ָ���ʼ��
	Flashdata->Kp=0;
	Flashdata->Ki=0;
	Flashdata->Kd=0;
	STMFLASH_Read(FLASH_SAVE_ADDR_TEM,(u16*)datatempTem,SIZE);
	STMFLASH_Read(FLASH_SAVE_ADDR_P,(u16*)datatempP,SIZE);
	STMFLASH_Read(FLASH_SAVE_ADDR_I,(u16*)datatempI,SIZE);
	STMFLASH_Read(FLASH_SAVE_ADDR_D,(u16*)datatempD,SIZE);
	Flashdata->Kp=(float)atof((char *)datatempP);
  Flashdata->Ki=(float)atof((char *)datatempI);
  Flashdata->Kd=(float)atof((char *)datatempD);
  settem=(float)atof((char *)datatempTem);


	//������Ϣ����
	Queue_Creat();
		//��ֵ�źŴ���
	BinarySemaphore=xSemaphoreCreateBinary();	
	
	//��Ϣ���з���flash�ڴ��е����ò���
		xQueueOverwrite(Set_Queue,(void *)&Flashdata);			
		xQueueOverwrite(Settem_Queue,&settem);			

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
		
		//����MQTT��������			
		   xTaskCreate((TaskFunction_t )MQTT_send_task,             
                (const char*    )"MQTT_Send_Task",           
                (uint16_t       )MQTT_Send_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_Send_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_Send_task_Handler);
				
			//����MQTT��������			
		   xTaskCreate((TaskFunction_t )MQTT_rec_task,             
                (const char*    )"MQTT_Rec_Task",           
                (uint16_t       )MQTT_Rec_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_Rec_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_Rec_task_Handler);
				
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}




