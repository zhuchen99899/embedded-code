#ifndef EVENTGROUPCREAT_H
#define EVENTGROUPCREAT_H

// config   事件标志组设置

#define EVENTBIT_0	(1<<0)				//事件位  //emwin
#define EVENTBIT_1	(1<<1)									//touch
#define EVENTBIT_2	(1<<2)									//pwm
#define EVENTBIT_3	(1<<3)									//adc
#define EVENTBIT_4	(1<<4)									//publish
#define EVENTBIT_5	(1<<5)									//rec
//#define EVENTBIT_6	(1<<6)									
//#define EVENTBIT_7	(1<<7)
#define EVENTBIT_ALL	(EVENTBIT_0|EVENTBIT_1|EVENTBIT_2|EVENTBIT_3|EVENTBIT_4|EVENTBIT_5)


	




void EventGroupCreat(void);






#endif

