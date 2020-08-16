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
extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;
extern GUI_CONST_STORAGE GUI_BITMAP bmSetting;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//��һ��multipage �ؼ�
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

/**********������ʾ�ؼ�****************/
//����ʹ��
#define ID_TEXT_15      (GUI_ID_USER + 28)
#define ID_TEXT_16      (GUI_ID_USER + 29)

//�ڶ���multipage �ؼ�
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

//������ؼ�
#define ID_Window_0         (GUI_ID_USER + 24)
#define ID_MULTIPAGE_0			(GUI_ID_USER + 25)
#define ID_BUTTON_0        (GUI_ID_USER + 26)
#define ID_BUTTON_1        (GUI_ID_USER + 27)





// USER START (Optionally insert additional defines)

// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

#define TEXT_MAXLEN   8   //EDIT�ؼ���󳤶�


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

	static U8      ShowText1=0 ; //edit SETTEM �༭�����ݸ��ı�־
	static U8      ShowText2=0 ; //edit PID     �༭�����ݸ��ı�־
	static U8      saveText1=0 ; //edit SETTEM �༭�����ݱ����־
	static U8      saveText2=0 ; //edit PID     �༭�����ݱ����־
	
	
	
	static u8 datatempTem[TEXT_MAXLEN];
	static u8 datatempP[TEXT_MAXLEN];
	static u8 datatempI[TEXT_MAXLEN];
	static u8 datatempD[TEXT_MAXLEN];
	
	//Ҫд�뵽STM32 FLASH���ַ�������

#define SIZE TEXT_MAXLEN		//���鳤��
/******************flash�洢settem\p\I\D ����********************/
//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
#define FLASH_SAVE_ADDR_TEM  0x0807FE60	
#define FLASH_SAVE_ADDR_P    0x0807FE50	
#define FLASH_SAVE_ADDR_I  	 0x0807FF60 
#define FLASH_SAVE_ADDR_D    0x0807FF50
/*********************************************************MULTIPAGEҳ��****************************************/

/**********************************************************
��һ��MULTIPAGEҳ��
���ܣ���ʾPID,�¶�����
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


/*********����********************/	
	WM_HWIN hWin = pMsg->hWin;
	WM_HTIMER hTimer;
	// USER START (Optionally insert additional variables)
	// USER END

	float adcmsg;
	char adcstring[20];
	extern QueueHandle_t Adc_Queue;


	
	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		//
		// Initialization of 'Framewin'
		//
/********************�¶��Լ������¶���ʾ******************************/
	
	
	GUI_SetFont(&GUI_Font32B_ASCII);

	/********��ȡflash�д洢���趨�¶�*******/
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_1),GUI_DARKMAGENTA);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_1), "Settem:");


	STMFLASH_Read(FLASH_SAVE_ADDR_TEM,(u16*)datatempTem,SIZE);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_0), (char *)datatempTem);
	
	
		/************��ȡʵʱ�¶�**************/
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_6),GUI_RED);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_6), "Temperature:");
	

	
	
/*******************P\I\D������ʾ**********************************/	
	GUI_SetFont(&GUI_Font32B_1);
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_8),GUI_DARKMAGENTA);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_8), "P:");
	
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_9),GUI_DARKMAGENTA);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_9), "I:");
	
	TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_10),GUI_DARKMAGENTA);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_10), "D:");



		/********��ȡflash�д洢��pid��ֵ*******/
		
	STMFLASH_Read(FLASH_SAVE_ADDR_P,(u16*)datatempP,SIZE);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_11), (char *)datatempP);

	
	STMFLASH_Read(FLASH_SAVE_ADDR_I,(u16*)datatempI,SIZE);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_12), (char *)datatempI);
	
	STMFLASH_Read(FLASH_SAVE_ADDR_D,(u16*)datatempD,SIZE);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_13), (char *)datatempD);




/******************�������ڶ�ʱ��***********************/
	hTimer= WM_CreateTimer(WM_GetClientWindow(hWin),1, 1000, 0);//������ʱ�� 1sˢ��
(void)hTimer;//��ֹ����

		break;//init




	case WM_PAINT:

	/**********************TEM�����¶���ʾ*********************************/
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
		
				/*************************PID������ʾ********************************/
		
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
		
		
	/*******************************����ɫ***************************************/	
		
		
	  	GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
							  0,			   /* ���Ͻ�Y λ�� */
							  800,	   	       /* ���½�X λ�� */
							  480,  	   	   /* ���½�Y λ�� */
							  GUI_WHITE,	   /* ���������Ҫ���Ƶ���ɫ */
							  GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */


			GUI_DrawGradientH(0,			   /* ���Ͻ�X λ�� */
							  170,			   /* ���Ͻ�Y λ�� */
							  800,	           /* ���½�X λ�� */
							  170,             /* ���½�Y λ�� */
							  GUI_RED,	       /* ���������Ҫ���Ƶ���ɫ */
							  GUI_YELLOW);     /* �������Ҳ�Ҫ���Ƶ���ɫ */
							  
		

		
		
		
		
		
		break;//wm_paint
	
		case WM_TIMER:
			
		WM_RestartTimer(pMsg->Data.v,1000); //���ڶ�ʱ����װ��

		/*************��ȡADC������Ϣ����*********************/
		xQueuePeek(Adc_Queue,&adcmsg,100);
		sprintf(adcstring,"%f",adcmsg);
		TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_7), adcstring);
		



		
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
�ڶ���MULTIPAGEҳ��
���ܣ��趨�¶ȣ�PID����
************************************************************/


/***************************************************************�ڶ�MULTIPAGEҳ��***********************************/

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

typedef  struct SETMSG
	{
		float Kp;
		float Ki;
		float Kd;
		
	}SETMSG;
extern SETMSG g_tMsg;

	
static void _cbMULTIPAGE2(WM_MESSAGE * pMsg)
{

	float SetTem;
  SETMSG *Setdata;

//	Setdata->Kp=0;
//	Setdata->Ki=0;
//	Setdata->Kd=0;
//	
extern QueueHandle_t Set_Queue;
extern QueueHandle_t Settem_Queue;
//��Ϣ���в���

	int     NCode;
	int     Id;

	// USER START (Optionally insert additional variables)
	int intbufferA;
	Setdata=&g_tMsg;//	�ṹ��ָ���ʼ��
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
			GUI_DrawGradientV(0,			   /* ���Ͻ�X λ�� */
							  0,			   /* ���Ͻ�Y λ�� */
							  800,	   	       /* ���½�X λ�� */
							  480,  	   	   /* ���½�Y λ�� */
							  GUI_WHITE,	   /* ���������Ҫ���Ƶ���ɫ */
							  GUI_LIGHTBLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */

				GUI_DrawGradientH(0,			   /* ���Ͻ�X λ�� */
							  170,			   /* ���Ͻ�Y λ�� */
							  800,	           /* ���½�X λ�� */
							  170,             /* ���½�Y λ�� */
							  GUI_RED,	       /* ���������Ҫ���Ƶ���ɫ */
							  GUI_YELLOW);     /* �������Ҳ�Ҫ���Ƶ���ɫ */
							  
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
				/*************��Ҫ*********************/
				//hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);     //��ȡedit�ؼ��ľ��
				//��edit�ؼ�����������Ϣ�в���ʹ�û�ȡAPI,EDIT_GetText�������ڴ˴���ֵ����������ػ�ʱ��ֵ��������

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
				/*************��Ҫ*********************/
				//hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);     //��ȡedit�ؼ��ľ��
				//��edit�ؼ�����������Ϣ�в���ʹ�û�ȡAPI,EDIT_GetText�������ڴ˴���ֵ����������ػ�ʱ��ֵ��������

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
				/*************��Ҫ*********************/
				//hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);     //��ȡedit�ؼ��ľ��
				//��edit�ؼ�����������Ϣ�в���ʹ�û�ȡAPI,EDIT_GetText�������ڴ˴���ֵ����������ػ�ʱ��ֵ��������

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
				/*************��Ҫ*********************/
				//hEdit = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);     //��ȡedit�ؼ��ľ��
				//��edit�ؼ�����������Ϣ�в���ʹ�û�ȡAPI,EDIT_GetText�������ڴ˴���ֵ����������ػ�ʱ��ֵ��������

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
					ShowText1=1;
					saveText1=1;
				}

				else
				{
					GUI_MessageBox("the value is changed","Set Tem successful",GUI_MESSAGEBOX_CF_MODAL);
					strcpy(DisPlayTem,aBuffer1);
					ShowText1=1;
					saveText1=1;
				}
		/************�¶�������Ϣ���з���***************/
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
		
				ShowText2=1;
			  saveText2=1;
				/************PID������Ϣ���з���***************/
	

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


/********************************************************��ҳ��*****************************************************************/

/*********************************************************************
�����Զ���
**********************************************************************/
/*
*********************************************************************************************************
*	�� �� ��: _cbButtonBack
*	����˵��: ��ť�ص�����
*	��    ��: pMsg  ��Ϣָ��
*	�� �� ֵ: ��
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

/*
*********************************************************************************************************
*	�� �� ��: _cbButtonList
*	����˵��: ��ť�ص�����
*	��    ��: pMsg  ��Ϣָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbButtonList(WM_MESSAGE * pMsg) 
{
	WM_HWIN  hWin;

	hWin  = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			if (BUTTON_IsPressed(hWin)) 
			{
				GUI_SetColor(GUI_DARKGRAY);
				GUI_AA_FillCircle(0, 0, 72);
				GUI_SetBkColor(GUI_DARKGRAY);
				GUI_SetColor(GUI_WHITE); 
				
				GUI_DrawBitmap(&bmSetting, 0, 5);
			} 
			else 
			{
				GUI_SetColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_AA_FillCircle(0, 0, 72);
				GUI_SetBkColor(GUI_STCOLOR_LIGHTBLUE);
				GUI_SetColor(GUI_WHITE);  

				GUI_DrawBitmap(&bmSetting, 0, 5);
			}
			break;
			
		default:
			BUTTON_Callback(pMsg);
	}
}


/*********************************************************************
�����洴��
**********************************************************************/



static const GUI_WIDGET_CREATE_INFO _aDialogCreateSettem[] = {
{ WINDOW_CreateIndirect, "Window", ID_Window_0, 0, 0, 800, 480, 0, 0x64, 0 },
{ MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE_0, 0, 100, 800, 480, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Button0", ID_BUTTON_0, 0, 0, 100,100, 0, 0x0, 0 },
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

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "GRAPH");
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonList);
	
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
		
		MULTIPAGE_SelectPage(hItem,0);

		break;

	case WM_PAINT:


		
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;

		switch (Id) {

		case ID_BUTTON_0: // Notifications sent by 'Button'
			switch (NCode) {

			case WM_NOTIFICATION_CLICKED:
	
				ADCCreateFramewin();

			break;
			case WM_NOTIFICATION_RELEASED:
			break;
		}
			break;
		
		case ID_BUTTON_1: // Notifications sent by 'Button'
			switch (NCode) {

			case WM_NOTIFICATION_CLICKED:
	
				break;
			case WM_NOTIFICATION_RELEASED:

			
			/************�˳�ǰ���� PID �Լ�����ֵTEM******************/
		
			//�ڲ�flash�����¶�����
if (saveText1==1)
{
				
		STMFLASH_Write(FLASH_SAVE_ADDR_TEM,(u16*)DisPlayTem,SIZE);
	saveText1=0;
}	

if (saveText2==1)
{
			//�ڲ�flash����pid
		STMFLASH_Write(FLASH_SAVE_ADDR_P,(u16*)DisPlayP,SIZE);
			
		STMFLASH_Write(FLASH_SAVE_ADDR_I,(u16*)DisPlayI,SIZE);
			
		STMFLASH_Write(FLASH_SAVE_ADDR_D,(u16*)DisPlayD,SIZE);
	saveText2=0;
}		

	
			/*******�رմ���**********/
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
WM_HWIN SETCreateFramewin(void);

WM_HWIN SETCreateFramewin(void) {
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









