#include <ButtonController.h>
#include "freertos.h"
#include "portmacro.h"
#include "queue.h"

/*********************************************************************************************************

										ButtonController程序

**********************************************************************************************************/
#define ID_FRAMEWIN_0   (GUI_ID_USER + 20)
#define ID_BUTTON_0   (GUI_ID_USER + 21)
#define ID_BUTTON_1   (GUI_ID_USER + 22)

#define ID_BUTTON_3   (GUI_ID_USER + 23)



/*ButtonController对话框*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateButton_Controller_1[] = {
	{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x64, 0 },
{ BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 174, 97, 200, 60, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 174, 203, 200, 60, 0, 0x0, 0 },
};

/*ButtonController初始化*/

/*ButtonController回调函数*/
static void _cbCallbackButton_Controller_1(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	WM_HWIN hWin = pMsg->hWin;
	int     NCode;
	int     Id;

	// USER START (Optionally insert additional variables)

	u8 key=0;
  extern QueueHandle_t Key_Queue;
	// USER END

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		//
		// Initialization of 'Framewin'
		//
		hItem = pMsg->hWin;
		FRAMEWIN_SetText(hItem, "TEST");
		FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
		//
		// Initialization of 'Button'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem, GUI_FONT_24B_ASCII);
		BUTTON_SetText(hItem, "LED1");
		//
		// Initialization of 'Button'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, GUI_FONT_24B_ASCII);
		BUTTON_SetText(hItem, "return");
		// USER START (Optionally insert additional code for further widget initialization)
		// USER END
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);

		
		NCode = pMsg->Data.v;
		switch (Id) {
		case ID_BUTTON_0: // Notifications sent by 'Button'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
        
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
					key=1;
					xQueueSend(Key_Queue,&key,10);
					key=0;
			
				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
		case ID_BUTTON_1: // Notifications sent by 'Button'
			switch (NCode) {
			case WM_NOTIFICATION_CLICKED:

	
				// USER START (Optionally insert code for reacting on notification message)

			
			
				// USER END
				break;
			case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				GUI_EndDialog(hWin, 0);

				// USER END
				break;
				// USER START (Optionally insert additional code for further notification handling)
				// USER END
			}
			break;
			// USER START (Optionally insert additional code for further Ids)
			// USER END
		}
		break;
		// USER START (Optionally insert additional message handling)
		// USER END
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}
/*ButtonControllerτ应用创建*/
void App_Button_Controller_1(WM_HWIN hWin)
{
	GUI_CreateDialogBox(_aDialogCreateButton_Controller_1,
		GUI_COUNTOF(_aDialogCreateButton_Controller_1),
		&_cbCallbackButton_Controller_1,
		hWin,
		0,
		0);
}





