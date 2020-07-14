#include <setdialog.h>
#include <adcgui.h>
#include "DIALOG.h"
#include "string.h"
#include "WM.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_Window_0         (GUI_ID_USER + 16)
#define ID_TEXT_0         (GUI_ID_USER + 18)
#define ID_BUTTON_0         (GUI_ID_USER + 19)
#define ID_BUTTON_1         (GUI_ID_USER + 20)
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
static const GUI_WIDGET_CREATE_INFO _aDialogCreateSettem[] = {
{ WINDOW_CreateIndirect, "Window", ID_Window_0, 0, 0, 800, 480, 0, 0x64, 0 },
{ TEXT_CreateIndirect, "", ID_TEXT_0, 250, 241, 188, 37, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Button0", ID_BUTTON_0, 703, 0, 97,34, 0, 0x0, 0 },
{ BUTTON_CreateIndirect, "Button1", ID_BUTTON_1, 0, 0, 97, 34, 0, 0x0, 0 },
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
	int     NCode;
	int     Id;
	WM_HWIN hWin = pMsg->hWin;

	// USER START (Optionally insert additional variables)
	// USER END

	//把edit文字缓冲区清空，防止未输入就按set后出现乱码。

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		//
		// Initialization of 'Framewin'
		//

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "GRAPH");

	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
		BUTTON_SetText(hItem, "Back");



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
				GUI_EndDialog(hWin, 0);
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









