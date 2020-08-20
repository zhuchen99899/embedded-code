#include "task_MQTT_send.h"

#include "freertos.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"
#include "string.h"
#include "wifi.h"
/*

void hexdump(unsigned char *buf, unsigned int buf_len) {

	int i, j, mod = buf_len % 16;

	int n = 16 - mod;

	for (i = 0; i < buf_len; i++)

	{

		if (i % 16 == 0 && i != 0)

		{

			printf("\n");

		}

		printf("%02X ", buf[i]);

		if ((i + 1) % 16 == 0)

		{

			printf("\t");

			for (j = i - 15; j <= i; j++)

			{

				if (j == i - 8)

					printf(" ");

				if (buf[j] >= 32 && buf[j] < 127)

					printf("%c", buf[j]);

				else

					printf(".");

			}

		}

	}

	for (i = 0; i < n; i++)

		printf("   ");

	printf("\t");

	for (i = buf_len - mod; i < buf_len; i++)

	{

		if (i == buf_len - mod + 8)

			printf(" ");

		if (buf[i] >= 32 && buf[i] < 127)

			printf("%c", buf[i]);

		else

			printf(".");

	}

	printf("\n");

}

*/

extern QueueHandle_t Adc_Queue;
/********************WIFI发送任务*****************/
 u8 temp_buff[100];
 int i;
 u8 *j;
void MQTT_send_task(void *pvParameters)
{
float adcmsg;
char adcmsgstring[10] ;

	/****消息队列参数**************/
//	extern QueueHandle_t Wifi_buffer_Queue;
//	extern QueueHandle_t Wifi_lenth_Queue;
//		int length;
//	u8 showbuffer[256];
	

temp_buff[0]=0x10;
temp_buff[1]=0x48;		
temp_buff[2]=0x00;
temp_buff[3]=0x04;
temp_buff[4]=0x4D;
temp_buff[5]=0x51;
temp_buff[6]=0x54;
temp_buff[7]=	0x54;
temp_buff[8]=0x04;
temp_buff[9]=0xC6;
temp_buff[10]=0x00;
temp_buff[11]=0x28;
temp_buff[12]=0x00;
temp_buff[13]=0x0E;
memcpy(&temp_buff[14],"dMQTTClient657",14);
temp_buff[28]=0x00;
temp_buff[29]=0x03;
memcpy(&temp_buff[30],"lwt",3);
temp_buff[33]=0x00;
temp_buff[34]=0x13;
memcpy(&temp_buff[35],"dMQTTClient657 died",19);
temp_buff[54]=0x00;
temp_buff[55]=0x05;
memcpy(&temp_buff[56],"admin",5);
temp_buff[61]=0x00;
temp_buff[62]=0x0B;
memcpy(&temp_buff[63],"oksn_123456",11);

vTaskDelay(20000);
	while(1)
{	
	//xQueuePeek(Adc_Queue,&adcmsg,portMAX_DELAY);

// sprintf((char*)adcmsgstring,"%f",adcmsg);
		//printf("WIFI任务正在运行\r\n");

	//WIFI_send((u8*)adcmsgstring,sizeof(adcmsgstring));


	WIFI_send(temp_buff,74);


	/*
for (i=0;i<76;i++)
	{
	printf("%02x",temp_buff[i]);
	
	}
*/
//hexdump(temp_buff,44);
	/*
	for(i=0,i<44,i++)
	{
printf(temp_buff[]);
{;	
*/	

	
      vTaskDelay(15000);                           //延时3000ms

}

}


