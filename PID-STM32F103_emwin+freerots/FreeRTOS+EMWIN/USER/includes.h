/***********ͷ�ļ������ļ�*****************/
#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/*****************������**********************/
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include "limits.h"


/****************system************************/

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
/*****************BSP����***********************/
#include "led.h"
#include "adc.h"
#include "timer.h"
#include "ILI93xx.h"
#include "key.h"
#include "sram.h"
#include "24cxx.h"
#include "touch.h"
#include "malloc.h"
#include "w25qxx.h"
#include "pwm.h"
#include "wifi.h"
#include "stmflash.h"
/***************Freertos���**********************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "QueueCreate.h"
#include "SemaphoreCreat.h"
#include "semphr.h"
/***************GUI���*****************************/
#include "GUI.h"
#include "WM.h"

/*************Freertos����**********************/
#include "task_led.h"
#include "task_pwm.h"
#include "task_adc.h"
#include "task_emwin.h"
#include "task_MQTT_connect.h"
#include "task_MQTT_pingreq.h"
#include "task_MQTT_subscribe.h"
#include "task_MQTT_publish.h"
#include "task_MQTT_rec.h"
#include "task_userif.h"
#endif


