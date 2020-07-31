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

extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;
extern GUI_CONST_STORAGE GUI_BITMAP bmSetting;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//第一个multipage 控件
#define ID_Window_1         (GUI_ID_USER + 16)
#define ID_TEXT_0         (GUI_ID_USER + 17)
#define ID_TEXT_1         (GUI_ID_USER + 18)
#define ID_TEXT_6         (GUI_ID_USER + 19)
#define ID_TEXT_7         (GUI_ID_USER + 20)

#define ID_TEXT_8        (GUI_ID_USER + 21)
#define ID_TEXT_9         (GUI_ID_USER + 22)
#define ID_TEXT_10         (GUI_ID_USER + 23)

#define ID_TEXT_11         (GUI_ID_USER + 24)
#define ID_TEXT_12        (GUI_ID_USER + 25)
#define ID_TEXT_13       (GUI_ID_USER + 26)
#define ID_TEXT_14      (GUI_ID_USER + 27)

//第二个multipage 控件
#define ID_Window_2         (GUI_ID_USER + 28)
#define ID_EDIT_0         (GUI_ID_USER + 29)
#define ID_EDIT_1         (GUI_ID_USER + 30)
#define ID_EDIT_2        (GUI_ID_USER + 31)
#define ID_EDIT_3        (GUI_ID_USER + 32)
#define ID_TEXT_2         (GUI_ID_USER + 33)
#define ID_TEXT_3        (GUI_ID_USER + 34)
#define ID_TEXT_4         (GUI_ID_USER + 35)
#define ID_TEXT_5         (GUI_ID_USER + 36)
#define ID_BUTTON_2         (GUI_ID_USER + 37)
#define ID_BUTTON_3         (GUI_ID_USER + 38)

//主界面控件
#define ID_Window_0         (GUI_ID_USER + 39)
#define ID_MULTIPAGE_0			(GUI_ID_USER + 40)
#define ID_BUTTON_0         (GUI_ID_USER + 41)
#define ID_BUTTON_1         (GUI_ID_USER + 42)





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

	static U8      ShowText1=0 ; //edit SETTEM 编辑框数据更改标志
	static U8      ShowText2=0 ; //edit PID     编辑框数据更改标志
	
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
#define FLASH_SAVE_ADDR_I  	 0x0807FF60 
#define FLASH_SAVE_ADDR_D    0x0807FF50
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
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_8, 50, 300, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_9, 300, 300, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_10, 550, 300, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_11, 80, 300, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_12, 330, 300, 150, 150, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TEXT", ID_TEXT_13, 580, 300, 150, 150, 0, 0x0, 0 },
};


static void _cbMULTIPAGE1(WM_MESSAGE * pMsg)
{



	WM_HWIN hWin = pMsg->hWin;

	// USER START (Optionally insert additional variables)
	// USER END
	
	//把edit文字缓冲区清空，防止未输入就按set后出现乱码。

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


	STMFLASH_Read(FLASH_SAVE_ADDR_TEM,(u16*)datatempTem,SIZE);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_0), (char *)datatempTem);
	
	
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



		/********获取flash中存储的pid数值*******/
		
	STMFLASH_Read(FLASH_SAVE_ADDR_P,(u16*)datatempP,SIZE);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_11), (char *)datatempP);
	
	STMFLASH_Read(FLASH_SAVE_ADDR_I,(u16*)datatempI,SIZE);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_12), (char *)datatempI);
	
	STMFLASH_Read(FLASH_SAVE_ADDR_D,(u16*)datatempD,SIZE);
	TEXT_SetText(WM_GetDialogItem(hWin, ID_TEXT_13), (char *)datatempD);


		break;//init




	case WM_PAINT:

	/**********************TEM设置温度显示*********************************/
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
		
				/*************************PID设置显示********************************/
		
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
							  
		

		
		
		
		
		
		break;
	

	

		
	case WM_NOTIFY_PARENT:


		break;
		
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
/*********************************************************************小键盘页面*********************************************************/

/*********************************************************************
*
*     创建数字键盘左右方向键位图
*
**********************************************************************
*/
static GUI_CONST_STORAGE GUI_COLOR _aColorsArrow[] = {
	0xFFFFFF, 0x000000
};

static GUI_CONST_STORAGE GUI_LOGPALETTE _PalArrow = {
	2,	/* number of entries */
	1, 	/* No transparency */
	&_aColorsArrow[0]
};

static GUI_CONST_STORAGE unsigned char _acArrowRight[] = {
	____XX__, ________,
	____XXXX, ________,
	XXXXXXXX, XX______,
	____XXXX, ________,
	____XX__, ________
};

static GUI_CONST_STORAGE unsigned char _acArrowLeft[] = {
	____XX__, ________,
	__XXXX__, ________,
	XXXXXXXX, XX______,
	__XXXX__, ________,
	____XX__, ________
};

static GUI_CONST_STORAGE GUI_BITMAP _bmArrowRight = {
	10,            /* XSize */
	5,             /* YSize */
	2,             /* BytesPerLine */
	1,             /* BitsPerPixel */
	_acArrowRight, /* Pointer to picture data (indices) */
	&_PalArrow     /* Pointer to palette */
};

static GUI_CONST_STORAGE GUI_BITMAP _bmArrowLeft = {
	10,            /* XSize */
	5,             /* YSize */
	2,             /* BytesPerLine */
	1,             /* BitsPerPixel */
	_acArrowLeft,  /* Pointer to picture data (indices) */
	&_PalArrow     /* Pointer to palette */
};
//
// Array of keys
//


/******************************************************************
*
*功能按键消息定义
*
全局变量
*******************************************************************/
static int _aKey[] = { GUI_KEY_BACKSPACE, GUI_KEY_TAB, GUI_KEY_LEFT, GUI_KEY_RIGHT };

/*********************************************************************
*
*    数字键盘对话框创建
*
【注】1.左右方向键文字由位图绘制
**********************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogNumPad[] = {
	//
	//  Function        Text             Id         Px   Py   Dx   Dy
	//
{ WINDOW_CreateIndirect, 0  ,          0         ,   0,   0, 168, 320, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "7",    GUI_ID_USER + 7,  10,  10,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "8",    GUI_ID_USER + 8,  60,  10,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "9",    GUI_ID_USER + 9, 110,  10,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "4",    GUI_ID_USER + 4,  10,  60,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "5",    GUI_ID_USER + 5, 110,  60,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "6",    GUI_ID_USER + 6,  60,  60,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "1",    GUI_ID_USER + 1,  10, 110,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "2",    GUI_ID_USER + 2,  60, 110,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "3",    GUI_ID_USER + 3, 110, 110,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "0",    GUI_ID_USER + 0,  10, 160,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, ".",    GUI_ID_USER + 10,  60, 160,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Bak",  GUI_ID_USER + 11, 110, 160,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Tab",  GUI_ID_USER + 12,  10, 210,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, 0,      GUI_ID_USER + 13,  60, 210,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, 0,      GUI_ID_USER + 14, 110, 210,  45,  45, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "OK",   GUI_ID_USER + 15, 10, 260,  145,  45, 0, 0x0, 0 },

// USER START (Optionally insert additional widgets)
// USER END
};




/*********************************************************************
*
*       数字键盘对话框回调函数
*
**********************************************************************
*/
static void _cbDialogNumPad(WM_MESSAGE * pMsg) {
	WM_HWIN  hItem;
	WM_HWIN  hDlg;
	int      NCode;
	int      Pressed; //按键状态
	unsigned Id;
	unsigned i;

	Pressed = 0; //按键状态初值
	hDlg = pMsg->hWin;//获取对话框中其他控件句柄

					  //消息通知
	switch (pMsg->MsgId) {
		//窗口重绘消息
	case WM_PAINT:
		//键盘背景为亮灰色
		GUI_SetBkColor(GUI_LIGHTGRAY);
		GUI_Clear();
		break;
		//窗口初始化消息
	case WM_INIT_DIALOG:

		//设置所有按钮控件禁止聚焦（重要！同时刻所有控件只有一个控件可以聚焦，不禁止按钮聚焦，按下按钮后，按钮自动聚焦，此时编辑框聚焦失效）
		for (i = 0; i < GUI_COUNTOF(_aDialogNumPad) - 1; i++) {
			hItem = WM_GetDialogItem(hDlg, GUI_ID_USER + i);
			BUTTON_SetFocussable(hItem, 0);

			switch (i) {
			case 13://左键按钮位图设置
				BUTTON_SetBitmapEx(hItem, 0, &_bmArrowLeft, 20, 20);  /* Set bitmap for arrow left button (unpressed) */
				BUTTON_SetBitmapEx(hItem, 1, &_bmArrowLeft, 20, 20);  /* Set bitmap for arrow left button (pressed) */
				break;
			case 14://右键按钮位图设置
				BUTTON_SetBitmapEx(hItem, 0, &_bmArrowRight, 20, 20); /* Set bitmap for arrow right button (unpressed) */
				BUTTON_SetBitmapEx(hItem, 1, &_bmArrowRight, 20, 20); /* Set bitmap for arrow right button (pressed) */
				break;
			}
		}
		//按钮控件句柄值返回，防止出错
		//WM_GetDialogItem(hDlg, GUI_ID_USER + 12);
		
		break;

		//窗口子控件消息
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc); /* Id of widget */ //控件id
		NCode = pMsg->Data.v;  /* Notification code */ //通知码
	
		switch (NCode) {
			//子控件被点击
		case WM_NOTIFICATION_CLICKED:
			
			Pressed = 1;
			//【注】此位置没有break，点击后继续执行释放消息判断
			//子控件被释放
		case WM_NOTIFICATION_RELEASED:
			//若触发消息的控件ID为定义的全部14个按键中的一个
			if ((Id >= GUI_ID_USER) && (Id <= (GUI_ID_USER + GUI_COUNTOF(_aDialogNumPad) - 1))) {
			int Key;
				
				
				if(Id < GUI_ID_USER + 15){
						
				//触发消息的控件为数字按键以及'.'
						if (Id < GUI_ID_USER + 11) {
							char acBuffer[10];
							BUTTON_GetText(pMsg->hWinSrc, acBuffer, sizeof(acBuffer)); //获取按键文本，并存储acBuffer缓冲区中
							Key = acBuffer[0];//存储键位值
							}
						else 
							{
							Key = _aKey[Id - GUI_ID_USER - 11];     //获取此按键对应键盘消息，即全局变量_aKey中的四个值 
							}
							
							
						/*发送消息给当前聚焦的窗口，即创建键盘的编辑框控件*/
							GUI_SendKeyMsg(Key, Pressed);                                /* Send a key message to the focussed window */
				
						
						
					}



				
		

		
			}//
			
			break;
		}
		
		
		
		switch(Id)
		{
			case GUI_ID_USER + 15:
				switch(NCode)
				{
				case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
					


				case WM_NOTIFICATION_RELEASED:
					GUI_EndDialog(pMsg->hWin, 0);
				break;
				
				}
		
			break;
		}			
		
		
		
		
		
	break;
	default: 
		WM_DefaultProc(pMsg);
		break;
	}
}




WM_HWIN CreateNumpad(void);
WM_HWIN CreateNumpad(void) {
	WM_HWIN hnumpad;

	hnumpad = GUI_CreateDialogBox(_aDialogNumPad, GUI_COUNTOF(_aDialogNumPad), _cbDialogNumPad, WM_HBKWIN, 0, 0);
	WM_SetStayOnTop(hnumpad, 1);
	return hnumpad;
}

// USER START (Optionally insert additional static code)
// USER END

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

	int     NCode;
	int     Id;



	// USER START (Optionally insert additional variables)
	int intbufferA;
	// USER END

	//把edit文字缓冲区清空，防止未输入就按set后出现乱码。

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
							  GUI_LIGHTBLUE);  /* 矩形最右侧要绘制的颜色 */

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
			EDIT_GetText(hEditTem, aBuffer1, TEXT_MAXLEN);
			intbufferA=atoi(aBuffer1);
			
				if (intbufferA>350)
				{
					
					GUI_MessageBox("set_tempreture is overflow!! please change the value","Warning",GUI_MESSAGEBOX_CF_MODAL);
					strcpy(aBuffer1,"350");
					strcpy(DisPlayTem,aBuffer1);
					ShowText1=1;
					
				}
				else
				{
					GUI_MessageBox("the value is changed","Set Tem successful",GUI_MESSAGEBOX_CF_MODAL);
					strcpy(DisPlayTem,aBuffer1);
					ShowText1=1;
				}
							


			break;
		}
		break;//CASE:ID_BUTTON_2
				

		
		case ID_BUTTON_3: // Notifications sent by 'Button'
		switch (NCode) {

			case WM_NOTIFICATION_CLICKED:

			case WM_NOTIFICATION_RELEASED:
				
				EDIT_GetText(hEditP, aBufferP, TEXT_MAXLEN);
				EDIT_GetText(hEditI, aBufferI, TEXT_MAXLEN);
				EDIT_GetText(hEditD, aBufferD, TEXT_MAXLEN);
		
				
				GUI_MessageBox("the value is changed","Set PID successful",GUI_MESSAGEBOX_CF_MODAL);

				strcpy(DisPlayP,aBufferP);
			
				strcpy(DisPlayI,aBufferI);
			
				strcpy(DisPlayD,aBufferD);
			
				ShowText2=1;
			
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

/*
*********************************************************************************************************
*	函 数 名: _cbButtonList
*	功能说明: 按钮回调函数
*	形    参: pMsg  消息指针
*	返 回 值: 无
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
主界面创建
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

			
			/************退出前保存 PID 以及设置值TEM******************/
		
			//内部flash保存温度设置

				
		STMFLASH_Write(FLASH_SAVE_ADDR_TEM,(u16*)DisPlayTem,SIZE);
			
			//内部flash保存pid
		STMFLASH_Write(FLASH_SAVE_ADDR_P,(u16*)DisPlayP,SIZE);
			
		STMFLASH_Write(FLASH_SAVE_ADDR_I,(u16*)DisPlayI,SIZE);
			
		STMFLASH_Write(FLASH_SAVE_ADDR_D,(u16*)DisPlayD,SIZE);
			

	
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









