#include "DIALOG.h"

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
			if ((Id >= GUI_ID_USER) && (Id <= (GUI_ID_USER + GUI_COUNTOF(_aDialogNumPad) - 2))) {

						int Key;
				
				if(Id < GUI_ID_USER + 15)
				{			
				//触发消息的控件为数字按键以及'.'
						if (Id <= GUI_ID_USER + 10) {
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



				
		

		
			}
			
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





WM_HWIN CreateNumpad(void) {
	WM_HWIN hnumpad;

	hnumpad = GUI_CreateDialogBox(_aDialogNumPad, GUI_COUNTOF(_aDialogNumPad), _cbDialogNumPad, WM_HBKWIN, 0, 0);
	WM_SetStayOnTop(hnumpad, 1);
	return hnumpad;
}

// USER START (Optionally insert additional static code)
// USER END


