/******************MQTT����**************/
#ifndef  __MQTT_CONFIG_H__
#define  __MQTT_CONFIG_H__


/****************************
CONNECT ���� ����汾ID  = "MQTC"
						 ����汾    = 0
***************************/
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


/********************
Subscribe ���ı�������
�����������qos�ȼ�����ɶԳ���
���ı�ʶ������>0
*********************/
#define subscribe_header_dup  0  //�̶�ͷ�����ֽڿ��Ʊ��ı�־dupλ subscribe���Ĺ̶�Ϊ0
#define subscribe_count       2 //���Ĺ���������

/*******���ı���1*******/
#define subscribe_packetid   1   // ���ı�ʶ��



//����1
#define subscrible1_topicFilters1   "STM32F103ZET6/KEY"  //����1
#define subscribe1_qos1        1   //����1���ĵȼ�



//����1ͨ�����
#define topic1_compare_monolayer      "STM32F103ZET6/+"    
#define topic1_compare_Universal      "STM32F103ZET6/#"



//����2
#define subscrible1_topicFilters2	 "STM32F103ZET6/PID"  //����2
#define subscribe1_qos2				1		 //����2���ĵȼ�




/********************
Publish ��������

*********************/

/********��������1*******/
#define publish1_header_dup  0  //�̶�ͷ�����ֽڿ��Ʊ��ı�־dupλ 
#define publish1_qos       1   //publish qos�ȼ� 
#define publish1_retained   0  //�Ƿ�����Ϣ
#define publish1_packetid   2  //���ı�ʶ�� message id 
#define publish1_topicname   "STM32F103ZET6/temperature"  //������





#endif


