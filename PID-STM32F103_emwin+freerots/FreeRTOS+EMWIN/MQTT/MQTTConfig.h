/******************MQTT����**************/
#ifndef  __MQTT_CONFIG_H__
#define  __MQTT_CONFIG_H__


/****************************
CONNECT ���� ����汾ID  = "MQTC"
						 ����汾    = 0
************************/
#define MQTT_CONNCET_ID    "MQTC"
#define MQTT_CONNCET_version  0						



#define MQTT_Version  4										 //MQTTЭ��汾4=3.1.1��3=3.1
#define MQTT_clientID   "dMQTTClient657"   //�ͻ���ID
#define keepAlive_Interval  60             //����ʱ��(s)����������ʱ����Ҫ�������ͨѶ���߷���������������Ͽ����ӣ�


/***cleansession******/
#define cleansession_FLAG   1     //ɾ���Ự��־

/*********************
Will ���� ����汾ID = "MQTW"
					����汾   =  0
**********************/
#define MQTT_WILL_ID        "MQTW"
#define MQTT_WILL_version    0

#define Will_Flag            1		 //�������ñ�־
#define will_qos						 0     //QOS�ȼ� qos1=1,qos2=2 
#define will_retained				 0		 //�������ñ�־
#define will_topicName      "lwt"		//�������⣺����������־�������
#define will_message				"dMQTTClient657 died" //������Ϣ������������־�������
#define MQTT_username   		"admin"								//�û�������Ϊ�������û���
#define MQTT_password				"oksn_123456"					//���룺��Ϊ����������











#endif


