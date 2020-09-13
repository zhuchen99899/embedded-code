#include "task_FreertosTimer_Callback.h"
#include "freertos.h"
#include "timers.h"
#include "semphr.h"
#include "queue.h"


/********Freertos软件定时器组句柄******************/
//周期性软件定时器
extern TimerHandle_t 	AutoReloadTimer_For_MqttConnectErr_Handle;			
extern TimerHandle_t   AutoReloadTimer_For_MqttPingreqErr_Handle;
extern TimerHandle_t   AutoReloadTimer_For_MqttSubscribeErr_Handle;

/***************二值信号量句柄****************/
extern SemaphoreHandle_t BinarySemaphore_MQTTconnect;//MQTT CONNCET报文二值信号量句柄
extern SemaphoreHandle_t BinarySemaphore_MQTTsubscribe;//MQTT SUBSCRIBE报文二值信号量句柄
//周期定时器1回调，发送CONNECT报文后开启定时器，5S内没有接收CONNACK报文 在定时器回调中再次释放CONNECT任务信号量，并关闭定时器
void AutoReloadCallback_MqttConnectErr(TimerHandle_t xTimer)
{

	
	
	xSemaphoreGive(BinarySemaphore_MQTTconnect);//发送MQTT Connect报文信号
	printf("检测时间内CONNACK报文未应答，定时器回调中重新发送 CONNECT信号\r\n");
	printf("关闭CONNECT报文应答检测定时器\r\n");
	xTimerStop(AutoReloadTimer_For_MqttConnectErr_Handle,0); 	//关闭周期定时器
	
	
}


//周期定时器2，发送pingreq报文后开启，3min内没有接收到pingreq报文传来关闭定时器指令，在回调函数中再次释放CONNECT任务信号量，再关闭定时器
void AutoReloadCallback_MqttPingreqErr(TimerHandle_t xTimer)
{

	xSemaphoreGive(BinarySemaphore_MQTTconnect);//发送MQTT Connect报文信号
	printf("检测时间内PINGREQ报文未应答，定时器回调中重新发送 CONNECT信号\r\n");
	printf("关闭PINGREQ报文应答检测定时器\r\n");
	xTimerStop(AutoReloadTimer_For_MqttPingreqErr_Handle,0); 	//关闭周期定时器
	
	
	
}


//周期定时器3，发送subscribe报文后开启，5s内没有接收到suback报文，在定时器回调中再次释放subscribe信号量，重复三次计数flag,
	//如果还没有suback报文(在回调中判断flag),提前将flag置0，并再次释放CONNECT信号量，并关闭定时器			
void AutoReloadCallback_MqttSubscribeErr(TimerHandle_t xTimer)
{
static u8 suback_errflag=0;
	
	suback_errflag++;
	
	if(suback_errflag==3)
	{
	printf("检测时间内subscribe报文未应答3次，定时器回调中重新发送Connect报文信号量\r\n");
	xSemaphoreGive(BinarySemaphore_MQTTconnect);//发送MQTT Connect报文信号
	suback_errflag=0;
	printf("关闭SUBSCRIBE报文应答检测定时器\r\n");
	xTimerStop(AutoReloadTimer_For_MqttSubscribeErr_Handle,0); 	//关闭周期定时器
	
	}
	else
	{
		
	suback_errflag++;
		
	printf("检测时间内subscribe报文未应答，重新发送subscribe报文信号\r\n");
	xSemaphoreGive(BinarySemaphore_MQTTsubscribe);//发送MQTT SUBSCRIBE报文信号	
		printf("关闭SUBSCRIBE报文应答检测定时器\r\n");
	xTimerStop(AutoReloadTimer_For_MqttSubscribeErr_Handle,0); 	//关闭周期定时器
		
	}
	
}



