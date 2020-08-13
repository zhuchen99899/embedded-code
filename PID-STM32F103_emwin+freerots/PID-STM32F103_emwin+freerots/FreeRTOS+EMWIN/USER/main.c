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
						变量定义
************************************************************/


//消息队列全局变量
QueueHandle_t Key_Queue;
QueueHandle_t Adc_Queue;
QueueHandle_t Settem_Queue;
QueueHandle_t SetP_Queue;
QueueHandle_t SetI_Queue;
QueueHandle_t SetD_Queue;

//flash读取地址及变量

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

/***********************************************************
					任务函数声明
************************************************************/

void start_task(void *pvParameters);		//Start任务
void touch_task(void *pvParameters);		//TOUCH任务
void UserIf_task(void *pvParameters);		//接口消息任务
void emwindemo_task(void *pvParameters); 	//emwin任务
void LED_task(void *pvParameters);       // LED任务
void ADC_task(void *pvParameters);       // ADC任务
void PWM_task(void *pvParameters);       // PWM任务
/***********************************************************
						任务函数句柄
************************************************************/
//任务函数句柄
TaskHandle_t StartTask_Handler;			//Start任务
TaskHandle_t TouchTask_Handler;			//TOUCH任务
TaskHandle_t UserIfTask_Handler;		//接口消息任务
TaskHandle_t EmwindemoTask_Handler;		//emwin任务
TaskHandle_t LedTask_Handler;		//LED任务
TaskHandle_t Adc_task_Handler;		//ADC任务
TaskHandle_t PWM_task_Handler;		//PWM任务
/***********************************************************
						任务堆栈大小
************************************************************/
#define START_STK_SIZE 			256  	//Start任务
#define TOUCH_STK_SIZE			128		//TOUCH任务
#define USERIF_STK_SIZE			512		//接口消息任务
#define EMWINDEMO_STK_SIZE		512		//emwin任务
#define LED_STK_SIZE		128	//LED任务
#define ADC_STK_SIZE		512	//ADC任务
#define PWM_STK_SIZE		128	//PWM任务
/***********************************************************
						 任务优先级(数值越小优先级越低)
************************************************************/
#define START_TASK_PRIO			4		//Start任务
#define TOUCH_TASK_PRIO			1		//TOUCH任务
#define USERIF_TASK_PRIO 		1	    //接口消息任务
#define EMWINDEMO_TASK_PRIO		2		//emwin任务
#define LED_TASK_PRIO		1		//LED任务
#define ADC_TASK_PRIO		3		//ADC任务
#define PWM_TASK_PRIO		3		//PWM任务

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
	float Kp,Ki,Kd,settem;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);//开启CRC时钟

	GUI_Init();  					//STemWin初始化
	WM_SetCreateFlags(WM_CF_MEMDEV);//ewmin设置使存储设备创建控件与窗口
	
	
    taskENTER_CRITICAL();           //进入临界区
	
	/***********开机时读取flash中PID参数以及设置温度**************/
	STMFLASH_Read(FLASH_SAVE_ADDR_TEM,(u16*)datatempTem,SIZE);
	STMFLASH_Read(FLASH_SAVE_ADDR_P,(u16*)datatempP,SIZE);
	STMFLASH_Read(FLASH_SAVE_ADDR_I,(u16*)datatempI,SIZE);
	STMFLASH_Read(FLASH_SAVE_ADDR_D,(u16*)datatempD,SIZE);
	Kp=(float)atof((char *)datatempP);
  Ki=(float)atof((char *)datatempI);
  Kd=(float)atof((char *)datatempD);
  settem=(float)atof((char *)datatempTem);


	//创建消息队列
	Queue_Creat();
	
	//消息队列发送flash内存中的PID参数
		xQueueSend(SetP_Queue,&Kp,portMAX_DELAY);			
		xQueueSend(SetI_Queue,&Ki,portMAX_DELAY);		
		xQueueSend(SetD_Queue,&Kd,portMAX_DELAY);	
		xQueueSend(Settem_Queue,&settem,portMAX_DELAY);
	/**********************************任务创建******************************/
	
	//创建触摸任务
    xTaskCreate((TaskFunction_t )touch_task,             
                (const char*    )"touch_task",           
                (uint16_t       )TOUCH_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TOUCH_TASK_PRIO,        
                (TaskHandle_t*  )&TouchTask_Handler);   	
    //创建UserIf串口打印任务
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
		
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}





//统计任务
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

