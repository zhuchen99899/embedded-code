#include "DIALOG.h"

/*********************************************************************С����ҳ��*********************************************************/

/*********************************************************************
*
*     �������ּ������ҷ����λͼ
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
*���ܰ�����Ϣ����
*
ȫ�ֱ���
*******************************************************************/
static int _aKey[] = { GUI_KEY_BACKSPACE, GUI_KEY_TAB, GUI_KEY_LEFT, GUI_KEY_RIGHT };

/*********************************************************************
*
*    ���ּ��̶Ի��򴴽�
*
��ע��1.���ҷ����������λͼ����
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
*       ���ּ��̶Ի���ص�����
*
**********************************************************************
*/
static void _cbDialogNumPad(WM_MESSAGE * pMsg) {
	WM_HWIN  hItem;
	WM_HWIN  hDlg;
	int      NCode;
	int      Pressed; //����״̬
	unsigned Id;
	unsigned i;

	Pressed = 0; //����״̬��ֵ
	hDlg = pMsg->hWin;//��ȡ�Ի����������ؼ����

					  //��Ϣ֪ͨ
	switch (pMsg->MsgId) {
		//�����ػ���Ϣ
	case WM_PAINT:
		//���̱���Ϊ����ɫ
		GUI_SetBkColor(GUI_LIGHTGRAY);
		GUI_Clear();
		break;
		//���ڳ�ʼ����Ϣ
	case WM_INIT_DIALOG:

		//�������а�ť�ؼ���ֹ�۽�����Ҫ��ͬʱ�����пؼ�ֻ��һ���ؼ����Ծ۽�������ֹ��ť�۽������°�ť�󣬰�ť�Զ��۽�����ʱ�༭��۽�ʧЧ��
		for (i = 0; i < GUI_COUNTOF(_aDialogNumPad) - 1; i++) {
			hItem = WM_GetDialogItem(hDlg, GUI_ID_USER + i);
			BUTTON_SetFocussable(hItem, 0);

			switch (i) {
			case 13://�����ťλͼ����
				BUTTON_SetBitmapEx(hItem, 0, &_bmArrowLeft, 20, 20);  /* Set bitmap for arrow left button (unpressed) */
				BUTTON_SetBitmapEx(hItem, 1, &_bmArrowLeft, 20, 20);  /* Set bitmap for arrow left button (pressed) */
				break;
			case 14://�Ҽ���ťλͼ����
				BUTTON_SetBitmapEx(hItem, 0, &_bmArrowRight, 20, 20); /* Set bitmap for arrow right button (unpressed) */
				BUTTON_SetBitmapEx(hItem, 1, &_bmArrowRight, 20, 20); /* Set bitmap for arrow right button (pressed) */
				break;
			}
		}

		
		break;

		//�����ӿؼ���Ϣ
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc); /* Id of widget */ //�ؼ�id
		NCode = pMsg->Data.v;  /* Notification code */ //֪ͨ��
	
		switch (NCode) {
			//�ӿؼ������
		case WM_NOTIFICATION_CLICKED:
			
			Pressed = 1;
			//��ע����λ��û��break����������ִ���ͷ���Ϣ�ж�
			//�ӿؼ����ͷ�
		case WM_NOTIFICATION_RELEASED:
			//��������Ϣ�Ŀؼ�IDΪ�����ȫ��14�������е�һ��
			if ((Id >= GUI_ID_USER) && (Id <= (GUI_ID_USER + GUI_COUNTOF(_aDialogNumPad) - 2))) {

						int Key;
				
				if(Id < GUI_ID_USER + 15)
				{			
				//������Ϣ�Ŀؼ�Ϊ���ְ����Լ�'.'
						if (Id <= GUI_ID_USER + 10) {
							char acBuffer[10];
							BUTTON_GetText(pMsg->hWinSrc, acBuffer, sizeof(acBuffer)); //��ȡ�����ı������洢acBuffer��������
							Key = acBuffer[0];//�洢��λֵ
							}
						else 
							{
							Key = _aKey[Id - GUI_ID_USER - 11];     //��ȡ�˰�����Ӧ������Ϣ����ȫ�ֱ���_aKey�е��ĸ�ֵ 
							}
							
							
						/*������Ϣ����ǰ�۽��Ĵ��ڣ����������̵ı༭��ؼ�*/
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


