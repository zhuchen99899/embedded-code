#include "includes.h"
/**********************************************
现有问题：
1.USART2串口空闲中断中必须加入printf函数输出，否则会卡死中断中，
怀疑       ①  xHigherPriorityTaskWoken 问题 或者信号量与队列同时使用问题
					 ②  中断中memcopy或者结构体指针赋值问题
					 
解决方法 ：①注释 printf 注释 队列，信号量 以及结构体指针传参 
					 ②taskENTER_CRITICAL()以及taskEXIT_CRITICAL()保护dma中断
					 ③调度锁vTaskSuspendAll  保护
					 
					 调度锁vTaskSuspendAll()禁止调度器，临界区taskENTER_CRITICAL()把freertos系统时基也关闭了
多次实验



已解决问题：
在CONNECT报文后 会同时启用pingreq,subscribe报文，以及publish报文，在发送过程中，可能存在ping或publish报文进入发送缓冲，
从而使正在发送的subscribe报文不完整导致CONNCET断开连接。
解决方法：①对DMA缓冲进行双缓冲区管理
					②DMA发送函数wifi_send中首先关闭DMA通道防止打断
					③对报文发送任务SUBSCRIBE，PINGREQ,PUBLISH 任务发送之后进行打印或者其他动作，防止在同一时刻（CONNECT报文发送之后）同时开启调度，导致DMA发送产生竞争
					④调整报文发送任务优先级 SUBSCRIBE
***************************************************/
/***********************************************************
						变量定义
************************************************************/


//消息队列句柄
QueueHandle_t Key_Queue;
QueueHandle_t Adc_Queue;
QueueHandle_t Set_Queue;
QueueHandle_t Settem_Queue;
QueueHandle_t Wifi_buffer_Queue;
QueueHandle_t PINGREQ_Queue;
QueueHandle_t PUBLISH_Queue;
//二值信号量句柄
SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2空闲中断二值信号量句柄
SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET报文二值信号量句柄
SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE报文二值信号量句柄
//开机内部flash读取相关
#define TEXT_MAXLEN 8	//数组长度
#define SIZE TEXT_MAXLEN		//数组长度

	static u8 datatempTem[TEXT_MAXLEN];
	static u8 datatempP[TEXT_MAXLEN];
	static u8 datatempI[TEXT_MAXLEN];
	static u8 datatempD[TEXT_MAXLEN];
//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
#define FLASH_SAVE_ADDR_TEM  0x0807FE60	
#define FLASH_SAVE_ADDR_P    0x0807FE50	
#define FLASH_SAVE_ADDR_I  	 0x0807FF60 
#define FLASH_SAVE_ADDR_D    0x0807FF50

 	//结构体保存变量 用于消息队列
typedef struct SETMSG
	{
		float Kp;
		float Ki;
		float Kd;
		
	}SETMSG;

SETMSG g_tMsg;

/***********************************************************
					任务函数声明
************************************************************/

void start_task(void *pvParameters);		//Start任务
void touch_task(void *pvParameters);		//TOUCH任务
void UserIf_task(void *pvParameters);		//UserIf空闲任务
void emwindemo_task(void *pvParameters); 	//emwin任务
void LED_task(void *pvParameters);       // LED任务
void ADC_task(void *pvParameters);       // ADC任务
void PWM_task(void *pvParameters);       // PWM任务
void MQTT_Connect_task(void *pvParameters);       // MQTTconnect任务
void MQTT_rec_task(void *pvParameters);				// MQTT接收任务
void MQTT_Pingreq_task(void *pvParameters);				// MQTTPINGREQ任务
void MQTT_Subscribe_task(void *pvParameters);				// MQTTSUBSCRIBE任务
void MQTT_Publish_task(void *pvParameters);				// MQTTPUBLISH任务
/***********************************************************
						任务函数句柄
************************************************************/
//任务函数句柄
TaskHandle_t StartTask_Handler;			//Start任务
TaskHandle_t TouchTask_Handler;			//TOUCH任务
TaskHandle_t UserIfTask_Handler;		//UserIf空闲任务
TaskHandle_t EmwindemoTask_Handler;		//emwin任务
TaskHandle_t LedTask_Handler;		//LED任务
TaskHandle_t Adc_task_Handler;		//ADC任务
TaskHandle_t PWM_task_Handler;		//PWM任务
TaskHandle_t MQTT_Connect_task_Handler;		  // MQTTconnect任务
TaskHandle_t MQTT_Rec_task_Handler;		  // MQTT接收任务
TaskHandle_t MQTT_PINGREQ_task_Handler;		  // MQTTPINGREQ任务
TaskHandle_t MQTT_SUBSCRIBE_task_Handler;				// MQTTSUBSCRIBE任务
TaskHandle_t MQTT_Publish_task_Handler;				// MQTTPUBLISH任务
/***********************************************************
						任务堆栈大小
************************************************************/
#define START_STK_SIZE 			256  	//Start任务
#define TOUCH_STK_SIZE			128		//TOUCH任务
#define USERIF_STK_SIZE			512		//UserIf空闲任务
#define EMWINDEMO_STK_SIZE		512		//emwin任务
#define LED_STK_SIZE		128	//LED任务
#define ADC_STK_SIZE		512	//ADC任务
#define PWM_STK_SIZE		128	//PWM任务
#define MQTT_Connect_STK_SIZE		128	// MQTTconnect任务
#define MQTT_Rec_STK_SIZE		512	// MQTT接收任务
#define MQTT_PINGREQ_STK_SIZE		256	// MQTTPINGREQ任务
#define MQTT_SUBSCRIBE_STK_SIZE		128		// MQTTSUBSCRIBE任务
#define MQTT_Publish_STK_SIZE			512// MQTTPUBLISH任务
/***********************************************************
						 任务优先级(数值越小优先级越低)
************************************************************/
#define START_TASK_PRIO			4		//Start任务
#define TOUCH_TASK_PRIO			0	//TOUCH任务
#define USERIF_TASK_PRIO 		0	   //UserIf空闲任务
#define EMWINDEMO_TASK_PRIO		0		//emwin任务
#define LED_TASK_PRIO		1		//LED任务
#define ADC_TASK_PRIO		2		//ADC任务
#define PWM_TASK_PRIO		2		//PWM任务
#define MQTT_Connect_TASK_PRIO		3		// MQTTCONNCET任务
#define MQTT_Rec_TASK_PRIO		2		// MQTT接收任务
#define MQTT_PINGREQ_TASK_PRIO		2		// MQTTPINGREQ任务
#define MQTT_SUBSCRIBE_TASK_PRIO		3		// MQTTSUBSCRIBE任务
#define MQTT_Publish_TASK_PRIO			2	// MQTTPUBLISH任务
/***********************************************************
						 主函数入口
************************************************************/

int main(void)
{
	
/*
 在启动调度前，为了防止初始化 STM32 外设时有中断服务程序执行，这里禁止全局中断(除了 NMI 和 HardFault)。
 这样做的好处是：
 1. 防止执行的中断服务程序中有 FreeRTOS 的 API 函数。
 2. 保证系统正常启动，不受别的中断影响。
 3. 关于是否关闭全局中断，大家根据自己的实际情况设置即可。
 在移植文件 port.c 中的函数 prvStartFirstTask 中会重新开启全局中断。通过指令 cpsie i 开启，__set_PRIMASK(1)
 和 cpsie i 是等效的。
 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				//延时函数初始化	 
	uart_init(115200);					//初始化串口
	wifi_init(115200);					//初始化wifi
	Adc_Init();                //ADC初始化
	LED_Init();		  					//初始化LED
	KEY_Init();							//初始化按键
	TFTLCD_Init();						//LCD初始化	
	FSMC_SRAM_Init();					//初始化SRAM
	TP_Init();							//触摸屏初始化
	TIM3_PWM_Init(1439,0);//1439+1=1440,不分频，频率为72000000/1440=50khz
	TIM4_Int_Init(1799,0);//PWM初始化
	my_mem_init(SRAMIN);            	//初始化内部内存池
	my_mem_init(SRAMEX);				//初始化外部内存池
	
	

	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄                
    vTaskStartScheduler();          //开启任务调度
/*
 如果系统正常启动是不会运行到这里的，运行到这里极有可能是用于定时器任务或者空闲任务的
 heap 空间不足造成创建失败，此要加大 FreeRTOSConfig.h 文件中定义的 heap 大小：
 #define configTOTAL_HEAP_SIZE ( ( size_t ) ( 17 * 1024 ) )
*/
	while(1);			
}

/***********************************************************
						 任务函数
************************************************************/

//开始任务任务函数
void start_task(void *pvParameters)
{
SETMSG *Flashdata;
float settem;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//开启CRC时钟

	GUI_Init();  					//STemWin初始化
	WM_SetCreateFlags(WM_CF_MEMDEV);//ewmin设置使存储设备创建控件与窗口
	
	
    taskENTER_CRITICAL();           //进入临界区
	
	/***********开机时读取flash中PID参数以及设置温度**************/
	Flashdata= &g_tMsg; //结构体指针初始化
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


	//创建消息队列
	Queue_Creat();
		//信号量创建
SempaphoreCreate();
	
	//消息队列发送flash内存中的设置参数
		xQueueOverwrite(Set_Queue,(void *)&Flashdata);			
		xQueueOverwrite(Settem_Queue,&settem);			

//发送MQTT任务信号量
	xSemaphoreGive(BinarySemaphore_MQTTconnect);//发送MQTT Connack报文信号
	/**********************************任务创建******************************/
	CJSON_INIT();
	//创建触摸任务
    xTaskCreate((TaskFunction_t )touch_task,             
                (const char*    )"touch_task",           
                (uint16_t       )TOUCH_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TOUCH_TASK_PRIO,        
                (TaskHandle_t*  )&TouchTask_Handler);   	
    //创建UserIf空闲任务
    xTaskCreate((TaskFunction_t )UserIf_task,             
                (const char*    )"UserIf",           
                (uint16_t       )USERIF_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )USERIF_TASK_PRIO,        
                (TaskHandle_t*  )&UserIfTask_Handler);  
    //创建EMWIN Demo任务
    xTaskCreate((TaskFunction_t )emwindemo_task,             
                (const char*    )"emwindemo_task",           
                (uint16_t       )EMWINDEMO_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EMWINDEMO_TASK_PRIO,        
                (TaskHandle_t*  )&EmwindemoTask_Handler);
		//创建LED任务
								
								
		    xTaskCreate((TaskFunction_t )LED_task,             
                (const char*    )"LED_task",           
                (uint16_t       )LED_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LED_TASK_PRIO,//
                (TaskHandle_t*  )&LedTask_Handler);
								
		//创建ADC任务			
		    xTaskCreate((TaskFunction_t )ADC_task,             
                (const char*    )"ADC_task",           
                (uint16_t       )ADC_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )ADC_TASK_PRIO,//
                (TaskHandle_t*  )&Adc_task_Handler);
		
								
		//创建PWM任务			
		   xTaskCreate((TaskFunction_t )PWM_task,             
                (const char*    )"PWM_task",           
                (uint16_t       )PWM_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )PWM_TASK_PRIO,//
                (TaskHandle_t*  )&PWM_task_Handler);
		
		//创建MQTT发送任务			
		   xTaskCreate((TaskFunction_t )MQTT_Connect_task,             
                (const char*    )"MQTT_Connect_Task",           
                (uint16_t       )MQTT_Connect_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_Connect_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_Connect_task_Handler);
				
			//创建MQTT接收任务			
		   xTaskCreate((TaskFunction_t )MQTT_rec_task,             
                (const char*    )"MQTT_Rec_Task",           
                (uint16_t       )MQTT_Rec_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_Rec_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_Rec_task_Handler);
			 //创建MQTT PINGREQ任务			
		   xTaskCreate((TaskFunction_t )MQTT_Pingreq_task,             
                (const char*    )"MQTT_PINGREQ_Task",           
                (uint16_t       )MQTT_PINGREQ_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_PINGREQ_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_PINGREQ_task_Handler);
				
				//创建MQTT SUBSCRIBE任务			
		   xTaskCreate((TaskFunction_t )MQTT_Subscribe_task,             
                (const char*    )"MQTT_SUBSCRIBE_Task",           
                (uint16_t       )MQTT_SUBSCRIBE_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_SUBSCRIBE_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_SUBSCRIBE_task_Handler);
				
				//创建MQTT PUBLISH任务			
			 xTaskCreate((TaskFunction_t )MQTT_Publish_task,             
                (const char*    )"MQTT_PUBLISH_Task",           
                (uint16_t       )MQTT_Publish_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MQTT_Publish_TASK_PRIO,//
                (TaskHandle_t*  )&MQTT_Publish_task_Handler);
				
														
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}





