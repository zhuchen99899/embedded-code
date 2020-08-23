/******************MQTT配设**************/
#ifndef  __MQTT_CONFIG_H__
#define  __MQTT_CONFIG_H__


/****************************
CONNECT 配设 代码版本ID  = "MQTC"
						 代码版本    = 0
************************/
#define MQTT_CONNCET_ID    "MQTC"
#define MQTT_CONNCET_version  0						



#define MQTT_Version  4										 //MQTT协议版本4=3.1.1，3=3.1
#define MQTT_clientID   "dMQTTClient657"   //客户端ID
#define keepAlive_Interval  60             //保持时间(s)，在这个这个时间内要与服务器通讯或者发送心跳包，否则断开连接，


/***cleansession******/
#define cleansession_FLAG   1     //删除会话标志

/*********************
Will 配设 代码版本ID = "MQTW"
					代码版本   =  0
**********************/
#define MQTT_WILL_ID        "MQTW"
#define MQTT_WILL_version    0

#define Will_Flag            1		 //遗嘱启用标志
#define will_qos						 0     //QOS等级 qos1=1,qos2=2 
#define will_retained				 0		 //保存启用标志
#define will_topicName      "lwt"		//遗嘱主题：设立遗嘱标志后必须有
#define will_message				"dMQTTClient657 died" //遗嘱消息：设立遗嘱标志后必须有
#define MQTT_username   		"admin"								//用户名：若为空则无用户名
#define MQTT_password				"oksn_123456"					//密码：若为空则无密码











#endif


