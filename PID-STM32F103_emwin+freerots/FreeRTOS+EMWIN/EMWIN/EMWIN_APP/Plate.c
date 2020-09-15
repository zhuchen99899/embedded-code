#include <Plate.h>
#include <setdialog.h>
#include "ST_GUI_Addons.h"
#include "FreeRTOS.h"
#include "queue.h"



extern GUI_CONST_STORAGE GUI_BITMAP bmReturn;
extern GUI_CONST_STORAGE GUI_BITMAP bmSetting;
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0           (GUI_ID_USER + 0x00)
#define ID_PROGBAR_0          (GUI_ID_USER + 0x01)
#define ID_SPINBOX_0          (GUI_ID_USER + 0x03)
#define ID_SCROLLBAR_0        (GUI_ID_USER + 0x04)
#define ID_BUTTON_0           (GUI_ID_USER + 0x05)
#define ID_BUTTON_1           (GUI_ID_USER + 0x06)

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _PlateCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 800, 480, 0, 0x0, 0 },
  { PROGBAR_CreateIndirect, "Progbar", ID_PROGBAR_0, 257, 23, 268, 131, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 264, 209, 250, 120, 0, 0x0, 0 },
  { SCROLLBAR_CreateIndirect, "Scrollbar", ID_SCROLLBAR_0, 206, 383, 378, 47, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button0", ID_BUTTON_0, 0, 0, 100,100, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button1", ID_BUTTON_1,700, 0, 100, 100, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};





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
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbPlate(WM_MESSAGE * pMsg) {
  int NCode;
  int Id;
	WM_HWIN hItem;
	WM_HWIN hWin = pMsg->hWin;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
		
	case WM_INIT_DIALOG:
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "SET");
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonList);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "BACK");
		WM_SetHasTrans(hItem);
		WM_SetCallback(hItem, _cbButtonBack);
	
	
	
	
	break;
		
		
		
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_SPINBOX_0: 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_MOVED_OUT:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;// Notifications sent by 'Spinbox'
    case ID_SCROLLBAR_0: 
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:

        break;
      case WM_NOTIFICATION_RELEASED:

        break;
      case WM_NOTIFICATION_VALUE_CHANGED:

        break;

      }
      break;// Notifications sent by 'Scrollbar'

		case ID_BUTTON_0: 
			switch (NCode) {

			case WM_NOTIFICATION_CLICKED:
	
			

			break;
			case WM_NOTIFICATION_RELEASED:
				
			SETCreateSetwin();
			break;
		}
			break;// Notifications sent by 'Button'
		
		case ID_BUTTON_1: 
			switch (NCode) {

			case WM_NOTIFICATION_CLICKED:
	
			break;
			case WM_NOTIFICATION_RELEASED:
			GUI_EndDialog(hWin, 0);
			break;
		}
			break;// Notifications sent by 'Button'
					
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateWindow
*/

void App_Plate(WM_HWIN hWin) {



	GUI_CreateDialogBox(_PlateCreate,
		GUI_COUNTOF(_PlateCreate),
		&_cbPlate,
		hWin,
		0,
		0);
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/


