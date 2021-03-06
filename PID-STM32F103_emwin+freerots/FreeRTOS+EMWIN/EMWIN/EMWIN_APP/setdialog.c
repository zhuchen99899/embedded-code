#include <setdialog.h>
#include <adcgui.h>
#include "DIALOG.h"
#include "string.h"
#include "WM.h"
#include "ST_GUI_Addons.h"
#include "stdio.h"
#include "stdlib.h"
#include "stmflash.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdlib.h>
#include <Numpad.h>
#include <stdio.h>
#include <semphr.h>

extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;
extern GUI_CONST_STORAGE GUI_BITMAP bmSetting;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//第一个multipage 控件
#define ID_Window_1         (GUI_ID_USER + 1)
#define ID_TEXT_0         (GUI_ID_USER + 2)
#define ID_TEXT_1         (GUI_ID_USER + 3)
#define ID_TEXT_6         (GUI_ID_USER + 4)
#define ID_TEXT_7         (GUI_ID_USER + 5)

#define ID_TEXT_8        (GUI_ID_USER + 6)
#define ID_TEXT_9         (GUI_ID_USER + 7)
#define ID_TEXT_10         (GUI_ID_USER + 8)

#define ID_TEXT_11         (GUI_ID_USER + 9)
#define ID_TEXT_12        (GUI_ID_USER + 10)
#define ID_TEXT_13       (GUI_ID_USER + 11)
#define ID_TEXT_14      (GUI_ID_USER + 12)

/**********测试显示控件****************/


//第二个multipage 控件
#define ID_Window_2         (GUI_ID_USER + 13)
#define ID_EDIT_0         (GUI_ID_USER + 14)
#define ID_EDIT_1         (GUI_ID_USER + 15)
#define ID_EDIT_2        (GUI_ID_USER + 16)
#define ID_EDIT_3        (GUI_ID_USER + 17)
#define ID_TEXT_2         (GUI_ID_USER + 18)
#define ID_TEXT_3        (GUI_ID_USER + 19)
#define ID_TEXT_4         (GUI_ID_USER + 20)
#define ID_TEXT_5         (GUI_ID_USER + 21)
#define ID_BUTTON_2         (GUI_ID_USER + 22)
#define ID_BUTTON_3         (GUI_ID_USER + 23)

//第三个multipage 控件
#define ID_Window_3         (GUI_ID_USER + 30)
#define ID_BUTTON_4      (GUI_ID_USER + 27)
#define ID_BUTTON_5      (GUI_ID_USER + 28)

//主界面控件
#define ID_Window_0         (GUI_ID_USER + 24)
#define ID_MULTIPAGE_0			(GUI_ID_USER + 25)
#define ID_BUTTON_1        (GUI_ID_USER + 26)



// USER START (Optionally insert additional defines)

// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

#define TEXT_MAXLEN   8   //EDIT控件最大长度


	static WM_HWIN hEditTem = 0;
	static WM_HWIN hEditP = 0;
	static WM_HWIN hEditI = 0;
	static WM_HWIN hEditD = 0;
	
	
  static char     aBuffer1[TEXT_MAXLEN];
	static char     aBufferP[TEXT_MAXLEN];
	static char     aBufferI[TEXT_MAXLEN];
	static char     aBufferD[TEXT_MAXLEN];
	
	static char     DisPlayTem[TEXT_MAXLEN];
	static char     DisPlayP[TEXT_MAXLEN];
	static char     DisPlayI[TEXT_MAXLEN];
	static char     DisPlayD[TEXT_MAXLEN];
	
	
	


	static U8      saveText1=0 ; //edit SETTEM 编辑框数据保存标志
	static U8      saveText2=0 ; //edit PID     编辑框数据保存标志
	
	static u8 datatempTem[TEXT_MAXLEN];
	static u8 datatempP[TEXT_MAXLEN];
	static u8 datatempI[TEXT_MAXLEN];
	static u8 datatempD[TEXT_MAXLEN];
	
	//要写入到STM32 FLASH的字符串数组

#define SIZE TEXT_MAXLEN		//数组长度
/******************flash存储settem\p\I\D 数据********************/
//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
#define FLASH_SAVE_ADDR_TEM  0x0807FE60	
#define FLASH_SAVE_ADDR_P    0x0807FE50	
#define FLASH_SAVE_ADDR_I  	 0x0807FE70 
#define FLASH_SAVE_ADDR_D    0x0807FF60

/************消息队列句柄************/
extern QueueHandle_t Set_Queue;
extern QueueHandle_t Settem_Queue;
extern QueueHandle_t PWM_Algorithm_Queue;


/*******************信号量句柄**************/
extern SemaphoreHandle_t BinarySemaphore_WIFI_PIDSET;//MQTT WIFI_PIDSET报文二值信号量句柄
extern SemaphoreHandle_t BinarySemaphore_WIFI_TEMSET;//MQTT WIFI_TEMSET报文二值信号量句柄


BaseType_t err_settem=pdFALSE;
BaseType_t err_setpid=pdFALSE;


//pid设置结构体
typedef  struct SETMSG
	{
		float Kp;
		float Ki;
		float Kd;
		
	}SETMSG;

extern SETMSG g_tMsg;

/*********************************************************MULTIPAGE页面****************************************/

/**********************************************************
第一个MULTIPAGE页面
功能：显示PID,温度数据
************************************************************/
	
static const GUI_WIDGET_CREATE_INFO _aDialogCreateWindowPage1[] = {
	{ WINDOW_CreateIndirect, "Window", ID_Window_1, 0, 0, 800, 480, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_0,  50, 70, 150, 150, 0, 0x0, 0},
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_1, 5, 70, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_6, 0, 120, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "0", ID_TEXT_7, 100, 120, 150, 150, 0, 0x0, 0 },
//	{ TEXT_CreateIndirect, "0", ID_TEXT_14, 300, 100, 150, 150, 0, 0x0, 0 },
	//{ TEXT_CreateIndirect, "duty", ID_TEXT_16, 580, 70, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_8, 50, 300, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_9, 300, 300, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_10, 550, 300, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_11, 80, 300, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_12, 330, 300, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_13, 580, 300, 150, 150, 0, 0x0, 0 },
};


static void _cbMULTIPAGE1(WM_MESSAGE * pMsg)
{


static char    WIFI_DisPlayTem[TEXT_MAXLEN];
static char     WIFI_P[TEXT_MAXLEN];
static char     WIFI_I[TEXT_MAXLEN];
static char     WIFI_D[TEXT_MAXLEN];
	
	float SetTem;
  SETMSG *PIDSET;//结构体指针初始化
	float adcmsg;
	char adcstring[20];
	
	/************队列句柄******************************/
	extern QueueHandle_t Adc_Queue;
	


/*********窗口********************/	
	WM_HWIN hWin = pMsg->hWin;
	WM_HTIMER hTimer;
	// USER START (Optionally insert additional variables)
	// USER END

	
	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		//
		// Initialization of 'Framewin'
		//
/********************温度以及设置温度显示******************************/
	
	
	GUI_SetFont(&GUI_Font32B_ASCII);

	/********获取flash中存储的设定温度*******/
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_1),GUI_DARKMAGENTA);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_1), "Settem:");


	
		/************获取实时温度**************/
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_6),GUI_RED);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_6), "Temperature:");
	

	
	
/*******************P\I\D设置显示**********************************/	
	GUI_SetFont(&GUI_Font32B_1);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_8),GUI_DARKMAGENTA);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_8), "P:");
	
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_9),GUI_DARKMAGENTA);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_9), "I:");
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_10),GUI_DARKMAGENTA);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_10), "D:");

TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_0),  "000.000");
TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_11), "000.000");
TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_12), "000.000");
TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_13), "000.000");


/******************创建窗口定时器***********************/
	hTimer= WM_CreateTimer(WM_GetClientWindow(hWin),1, 1000, 0);//创建定时器 1s刷新
(void)hTimer;//防止警告

		break;//init




	case WM_PAINT:

	
	/*****************多任务时使用消息队列解决显示问题，不使用全局变量showtest来使
	多个页面间进行通讯
	因为可能会有别的地方传入数据 。列入来源于上位机或网络数据需要显示
	*****************************************************************/
	
	/**********************TEM设置温度显示*********************************/
/*	

	
	if (ShowText1)
		{
				GUI_SetFont(&GUI_Font16B_ASCII);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_0), aBuffer1);
				GUI_Clear();
				ShowText1 = 0;
		}
		
		
		else 
		{
			
			
		}
		*/
				/*************************PID设置显示********************************/
		/*
		if (ShowText2)
		{
				GUI_SetFont(&GUI_Font16B_ASCII);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_11), aBufferP);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_12), aBufferI);
				TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_13), aBufferD);
				GUI_Clear();
				ShowText2 = 0;
		}
		else
		{
		
		}

   */
		
		
	/*******************************渐变色***************************************/	
		
		
	  	GUI_DrawGradientV(0,			   /* 左上角X 位置 */
							  0,			   /* 左上角Y 位置 */
							  800,	   	       /* 右下角X 位置 */
							  480,  	   	   /* 右下角Y 位置 */
							  GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
							  GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */


			GUI_DrawGradientH(0,			   /* 左上角X 位置 */
							  170,			   /* 左上角Y 位置 */
							  800,	           /* 右下角X 位置 */
							  170,             /* 右下角Y 位置 */
							  GUI_RED,	       /* 矩形最左侧要绘制的颜色 */
							  GUI_YELLOW);     /* 矩形最右侧要绘制的颜色 */
							  	
		
		break;//wm_paint
	
		case WM_TIMER:
			
		WM_RestartTimer(pMsg->Data.v,1000); //窗口定时器重装填

		/*************获取ADC任务消息队列*********************/
		
		xQueuePeek(Adc_Queue,&adcmsg,100);
		sprintf(adcstring,"%f",adcmsg);
		TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_7), adcstring);
		
		
		/*********************网络设置显示************************/

	
			xQueuePeek(Settem_Queue,&SetTem,100);
			sprintf(WIFI_DisPlayTem,"%f",SetTem);
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_0), WIFI_DisPlayTem);
	



			xQueuePeek(Set_Queue,&PIDSET,100);
				

/***************注意这里使用snprintf来转换float数据为字符串
原因：使用sprintf 时，转换结果中不仅有PIDSET->P数据还会有 I,D
数据，猜测可能时将结构体首地址放入进行转换导致？ （需查看sprintf实现）
snprintf可以限制转换的字符串长度，截去多余部分*******************/
//上述bug未来待解决
			snprintf(WIFI_P,8,"%f",(*PIDSET).Kp);
			snprintf(WIFI_I,8,"%f",(*PIDSET).Ki);
			snprintf(WIFI_D,8,"%f",(*PIDSET).Kd);


			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_11), WIFI_P);
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_12), WIFI_I);
			TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_13), WIFI_D);



		
		break;//wm_timer
			
	case WM_NOTIFY_PARENT:


		break;//wm_notify_parent
		
		// USER START (Optionally insert additional message handling)
		// USER END

default:
WM_DefaultProc(pMsg);
break;
	}

	
	
};





WM_HWIN CreateWindowPage1(void)
{
WM_HWIN hWin;
hWin= GUI_CreateDialogBox(_aDialogCreateWindowPage1,GUI_COUNTOF(_aDialogCreateWindowPage1),_cbMULTIPAGE1,WM_HBKWIN,0,0);
	return hWin;

};



/**********************************************************
第二个MULTIPAGE页面
功能：设定温度，PID参数
************************************************************/


/***************************************************************第二MULTIPAGE页面***********************************/

static const GUI_WIDGET_CREATE_INFO _aDialogCreateWindowPage2[] = {
	{ WINDOW_CreateIndirect, "Window", ID_Window_2, 0, 0, 800, 480, 0, 0x0, 0 },
{ EDIT_CreateIndirect, "Edit", ID_EDIT_0, 200, 70, 107, 28, 0, 0x64, 0 },
{ TEXT_CreateIndirect, "Tempreture", ID_TEXT_2, 50, 70, 100, 37, 0, 0x0, 0 },
{ EDIT_CreateIndirect, "Edit", ID_EDIT_1, 100, 241, 107, 28, 0, 0x64, 0 },
{ TEXT_CreateIndirect, "P:", ID_TEXT_3, 50, 241, 50, 40, 0, 0x0, 0 },
{ EDIT_CreateIndirect, "Edit", ID_EDIT_2, 300, 241, 107, 28, 0, 0x64, 0 },
{ TEXT_CreateIndirect, "I:", ID_TEXT_3, 250, 241, 40, 37, 0, 0x0, 0 },
{ EDIT_CreateIndirect, "Edit", ID_EDIT_3, 500, 241, 107, 28, 0, 0x64, 0 },
{ TEXT_CreateIndirect, "D:", ID_TEXT_3, 450, 241, 40, 37, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "SET", ID_BUTTON_2, 407, 70, 97, 34, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "SET", ID_BUTTON_3, 700, 241, 97, 34, 0, 0x0, 0 },
};


	
static void _cbMULTIPAGE2(WM_MESSAGE * pMsg)
{


	float SetTem;
  SETMSG *Setdata;
//消息队列参数

	int     NCode;
	int     Id;

	// USER START (Optionally insert additional variables)
	int intbufferA;
	Setdata=&g_tMsg;//	结构体指针初始化
	// USER END


	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		//
		// Initialization of 'Framewin'
		//
	STMFLASH_Read(FLASH_SAVE_ADDR_TEM,(u16*)datatempTem,SIZE);
  EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0),(char *)datatempTem);
		
	STMFLASH_Read(FLASH_SAVE_ADDR_P,(u16*)datatempP,SIZE);
	EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1),(char *)datatempP);

	STMFLASH_Read(FLASH_SAVE_ADDR_I,(u16*)datatempI,SIZE);
	EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2),(char *)datatempI);
	
	STMFLASH_Read(FLASH_SAVE_ADDR_D,(u16*)datatempD,SIZE);
	EDIT_SetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_3),(char *)datatempD);


		break;

	case WM_PAINT:
			GUI_DrawGradientV(0,			   /* 左上角X 位置 */
							  0,			   /* 左上角Y 位置 */
							  800,	   	       /* 右下角X 位置 */
							  480,  	   	   /* 右下角Y 位置 */
							  GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
							  GUI_LIGHTCYAN);  /* 矩形最右侧要绘制的颜色 */

				GUI_DrawGradientH(0,			   /* 左上角X 位置 */
							  170,			   /* 左上角Y 位置 */
							  800,	           /* 右下角X 位置 */
							  170,             /* 右下角Y 位置 */
							  GUI_RED,	       /* 矩形最左侧要绘制的颜色 */
							  GUI_YELLOW);     /* 矩形最右侧要绘制的颜色 */
							  
		break;

	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
	
	
			switch (Id) {
	case ID_EDIT_0: // Notifications sent by 'Edit'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				CreateNumpad();
				hEditTem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
				WM_SetFocus(hEditTem);
				hEditTem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
				WM_SetFocus(hEditTem);

				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				/*************重要*********************/
				//hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);     //获取edit控件的句柄
				//在edit控件更改内容消息中不能使用获取API,EDIT_GetText。可以在此处赋值句柄，窗口重绘时赋值缓冲区。

				if (hEditTem == 0) {
					hEditTem = pMsg->hWinSrc;
				}


				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			
			break;//	case ID_EDIT_0

			case ID_EDIT_1: // Notifications sent by 'Edit'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				CreateNumpad();
				hEditP = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
				WM_SetFocus(hEditP);
				hEditP = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
				WM_SetFocus(hEditP);

				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				/*************重要*********************/
				//hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);     //获取edit控件的句柄
				//在edit控件更改内容消息中不能使用获取API,EDIT_GetText。可以在此处赋值句柄，窗口重绘时赋值缓冲区。

				if (hEditP == 0) {
					hEditP = pMsg->hWinSrc;
				}


				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			
			break;	//case ID_EDIT_1
			
			
			
			case ID_EDIT_2: // Notifications sent by 'Edit'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				CreateNumpad();
				hEditI = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
				WM_SetFocus(hEditI);
				hEditI = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
				WM_SetFocus(hEditI);

				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				/*************重要*********************/
				//hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);     //获取edit控件的句柄
				//在edit控件更改内容消息中不能使用获取API,EDIT_GetText。可以在此处赋值句柄，窗口重绘时赋值缓冲区。

				if (hEditI == 0) {
					hEditI = pMsg->hWinSrc;
				}


				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			
			break;	//case ID_EDIT_2
			
			case ID_EDIT_3: // Notifications sent by 'Edit'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				CreateNumpad();
				hEditD = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
				WM_SetFocus(hEditD);
				hEditD = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
				WM_SetFocus(hEditD);

				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:

				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				/*************重要*********************/
				//hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);     //获取edit控件的句柄
				//在edit控件更改内容消息中不能使用获取API,EDIT_GetText。可以在此处赋值句柄，窗口重绘时赋值缓冲区。

				if (hEditD == 0) {
					hEditD = pMsg->hWinSrc;
				}


				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			
			break;	//case ID_EDIT_3
			
			case ID_BUTTON_2: // Notifications sent by 'Button'
			switch (NCode) {

			case WM_NOTIFICATION_CLICKED:

			case WM_NOTIFICATION_RELEASED:
			EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0),aBuffer1,TEXT_MAXLEN);
	
			intbufferA=atoi(aBuffer1);
			
				if (intbufferA>350)
				{
					
					GUI_MessageBox("set_tempreture is overflow!! please change the value","Warning",GUI_MESSAGEBOX_CF_MODAL);
					strcpy(aBuffer1,"350");
					strcpy(DisPlayTem,aBuffer1);
					saveText1=1;
				}

				else
				{
					GUI_MessageBox("the value is changed","Set Tem successful",GUI_MESSAGEBOX_CF_MODAL);
					strcpy(DisPlayTem,aBuffer1);
					saveText1=1;
				}
		/************温度设置消息队列发送***************/
	  SetTem=(float)atof(DisPlayTem);
		
		xQueueOverwrite(Settem_Queue,&SetTem);		


			break;
		}
		break;//CASE:ID_BUTTON_2
				

		
		case ID_BUTTON_3: // Notifications sent by 'Button'
		switch (NCode) {
				
			case WM_NOTIFICATION_CLICKED:

			case WM_NOTIFICATION_RELEASED:
			 EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_1),aBufferP,TEXT_MAXLEN);
			 EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_2),aBufferI,TEXT_MAXLEN);
			 EDIT_GetText(WM_GetDialogItem(pMsg->hWin, ID_EDIT_3),aBufferD,TEXT_MAXLEN);
			
				/*
				EDIT_GetText(hEditI, aBufferI, TEXT_MAXLEN);
				EDIT_GetText(hEditD, aBufferD, TEXT_MAXLEN);
		*/
				
				GUI_MessageBox("the value is changed","Set PID successful",GUI_MESSAGEBOX_CF_MODAL);

				strcpy(DisPlayP,aBufferP);
			
				strcpy(DisPlayI,aBufferI);
			
				strcpy(DisPlayD,aBufferD);
		
			  saveText2=1;
		/************PID设置消息队列发送***************/

	  Setdata->Kp=(float)atof(DisPlayP);
		Setdata->Ki=(float)atof(DisPlayI);
		Setdata->Kd=(float)atof(DisPlayD);
			
		xQueueOverwrite(Set_Queue,(void *) &Setdata);			


		
			 break;
		}
		break;//CASE:ID_BUTTON_3
				

		
		


	}
	break;//case WM_NOTIFY_PARENT
default:
WM_DefaultProc(pMsg);
break;
	
 }

}



WM_HWIN CreateWindowPage2(void)
{
	WM_HWIN hWin;
hWin= GUI_CreateDialogBox(_aDialogCreateWindowPage2,GUI_COUNTOF(_aDialogCreateWindowPage2),_cbMULTIPAGE2,WM_HBKWIN,0,0);
	return hWin;

}






/**********************************************************
第三个MULTIPAGE页面
功能：改变PID模式 ：1.模糊算法      2.PID参数模式
************************************************************/


/***************************************************************第三MULTIPAGE页面***********************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateWindowPage3[] = {
{ WINDOW_CreateIndirect, "Window", ID_Window_3, 0, 0, 800, 480, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Typical", ID_BUTTON_4, 340, 70, 97, 34, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Fuzzy", ID_BUTTON_5, 340, 250, 97, 34, 0, 0x0, 0 },
};


static void _cbMULTIPAGE3(WM_MESSAGE * pMsg) {
	//WM_HWIN hItem;
	int     NCode;
	int     Id;
		u8	PWM_Algorithm_flag=0;



	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		//
		// Initialization of 'Framewin'
		//

		break;

	case WM_PAINT:
			GUI_DrawGradientV(0,			   /* 左上角X 位置 */
							  0,			   /* 左上角Y 位置 */
							  800,	   	       /* 右下角X 位置 */
							  480,  	   	   /* 右下角Y 位置 */
							  GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
							  GUI_LIGHTMAGENTA);  /* 矩形最右侧要绘制的颜色 */

				GUI_DrawGradientH(0,			   /* 左上角X 位置 */
							  170,			   /* 左上角Y 位置 */
							  800,	           /* 右下角X 位置 */
							  170,             /* 右下角Y 位置 */
							  GUI_RED,	       /* 矩形最左侧要绘制的颜色 */
							  GUI_YELLOW);     /* 矩形最右侧要绘制的颜色 */

		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;

		switch (Id) {

		case ID_BUTTON_4: // Notifications sent by 'Button'
			switch (NCode) {

			case WM_NOTIFICATION_CLICKED:
	

			break;
			case WM_NOTIFICATION_RELEASED:
				PWM_Algorithm_flag=0;
				xQueueOverwrite(PWM_Algorithm_Queue,&PWM_Algorithm_flag);	
				GUI_MessageBox("Set typical PID MODE successful","Set MODE successful",GUI_MESSAGEBOX_CF_MODAL);
			
			//发送启用PID参数算法给消息队列
			break;
			
		}
			break;
		
		case ID_BUTTON_5: // Notifications sent by 'Button'
			switch (NCode) {

			case WM_NOTIFICATION_CLICKED:
	
				break;
			case WM_NOTIFICATION_RELEASED:
				PWM_Algorithm_flag=1;
			xQueueOverwrite(PWM_Algorithm_Queue,&PWM_Algorithm_flag);
			GUI_MessageBox("Set Fuzzy PID MODE successful","Set MODE successful",GUI_MESSAGEBOX_CF_MODAL);
			//发送启用模糊pid算法标志给消息队列
			break;
			// USER START (Optionally insert additional code for further Ids)
			// USER END
		}
		
		break;


	}//控件
		break;
		default:
		WM_DefaultProc(pMsg);
		break;
 }//message ID

}//cbmultipage3



WM_HWIN CreateWindowPage3(void)
{
	WM_HWIN hWin;
hWin= GUI_CreateDialogBox(_aDialogCreateWindowPage3,GUI_COUNTOF(_aDialogCreateWindowPage3),_cbMULTIPAGE3,WM_HBKWIN,0,0);
	return hWin;

}




/********************************************************主页面*****************************************************************/

/*********************************************************************
按键自定义
**********************************************************************/
/*
*********************************************************************************************************
*	函 数 名: _cbButtonBack
*	功能说明: 按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbButtonBack(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;

	hWin  = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(100, 0, 72);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				GUI_DrawBitmap(&bmReturn, 40, 5);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(100, 0, 72);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  
				GUI_DrawBitmap(&bmReturn, 40, 5);				
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}



/*********************************************************************
主界面创建
**********************************************************************/



static const GUI_WIDGET_CREATE_INFO _aDialogCreateSettem[] = {
{ WINDOW_CreateIndirect, "Window", ID_Window_0, 0, 0, 800, 480, 0, 0x64, 0 },
{ MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE_0, 0, 100, 800, 480, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Button1", ID_BUTTON_1,700, 0, 100, 100, 0, 0x0, 0 },
// USER START (Optionally insert additional widgets)
// USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/



/*********************************************************************
*
*       _cbDialog
*/
static void _cbCallbackSettem(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	WM_HWIN hWinPage;
	int     NCode;
	int     Id;
	WM_HWIN hWin = pMsg->hWin;

	// USER START (Optionally insert additional variables)
	// USER END



	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		//
		// Initialization of 'Framewin'
		//


	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "BACK");
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonBack);
	
	
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);
		MULTIPAGE_SetFont(hItem,GUI_FONT_16_ASCII);
	
	  hWinPage= CreateWindowPage1();
		MULTIPAGE_AddPage(hItem,hWinPage,"DisPlay");
		
		hWinPage=CreateWindowPage2();
		MULTIPAGE_AddPage(hItem,hWinPage,"SET");
		
		hWinPage=CreateWindowPage3();
		MULTIPAGE_AddPage(hItem,hWinPage,"MODE");
		
		MULTIPAGE_SelectPage(hItem,0);

		break;

	case WM_PAINT:


		
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;

		switch (Id) {

	
		
		case ID_BUTTON_1: // Notifications sent by 'Button'
			switch (NCode) {

			case WM_NOTIFICATION_CLICKED:
	
				break;
			case WM_NOTIFICATION_RELEASED:

			
			/************退出前保存 PID 以及设置值TEM******************/
		
			//内部flash保存温度设置
if (saveText1==1)
{
				
		STMFLASH_Write(FLASH_SAVE_ADDR_TEM,(u16*)DisPlayTem,SIZE);
	saveText1=0;
}	

if (saveText2==1)
{
			//内部flash保存pid
		STMFLASH_Write(FLASH_SAVE_ADDR_P,(u16*)DisPlayP,SIZE);
			
		STMFLASH_Write(FLASH_SAVE_ADDR_I,(u16*)DisPlayI,SIZE);
			
		STMFLASH_Write(FLASH_SAVE_ADDR_D,(u16*)DisPlayD,SIZE);
		saveText2=0;
}		

	
			/*******关闭窗口**********/
			GUI_EndDialog(hWin, 0);
			
			break;
			// USER START (Optionally insert additional code for further Ids)
			// USER END
		}
		
		break;
		case ID_MULTIPAGE_0: 
			switch (NCode) {

			case WM_NOTIFICATION_CLICKED:
	
				break;
			case WM_NOTIFICATION_RELEASED:
	
				
			break;
			// USER START (Optionally insert additional code for further Ids)
			// USER END
		}
		
		break;
		// USER START (Optionally insert additional message handling)
		// USER END

	}
		break;
		default:
		WM_DefaultProc(pMsg);
		break;
}

}

/*********************************************************************
*
*       CreateFramewin
*/


WM_HWIN SETCreateSetwin(void) {
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreateSettem, GUI_COUNTOF(_aDialogCreateSettem), _cbCallbackSettem, WM_HBKWIN, 0, 0);
	return hWin;
}



// USER START (Optionally insert additional public code)



void App_Settem(WM_HWIN hWin)
{
	GUI_CreateDialogBox(_aDialogCreateSettem,
		GUI_COUNTOF(_aDialogCreateSettem),
		&_cbCallbackSettem,
		hWin,
		0,
		0);
}
// USER END

/*************************** End of file ****************************/









