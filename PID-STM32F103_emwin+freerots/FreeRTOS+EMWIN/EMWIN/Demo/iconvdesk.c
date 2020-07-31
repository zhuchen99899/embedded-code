#include <iconvdesk.h>

#include "DIALOG.h"



#include <GUI.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iconvdesk.h>
#include <ButtonController.h>
#include <setdialog.h>
#include <adcgui.h>
/*
*********************************************************************************************************
*                                         ����
*********************************************************************************************************
*/

extern GUI_CONST_STORAGE unsigned char _acdeskbkbmp[235020UL + 1];


static GUI_MEMDEV_Handle   hMempic; //�洢�豸������ڻ������汳��ͼƬ

static WM_HWIN hIcon1, hIcon2, hIcon3;

//static uint8_t s_ucIconSwitch = 0;
WM_HWIN hMotion;

// USER START (Optionally insert additional defines)
// USER END

/*
*********************************************************************************************************
*                                         �궨��
*  ICONVIEW�����沼�� 800*480��ÿ��ͼ��̫С��72*72
*  1. ͼ��Ŀ��λ100���ء�
*  2. ICONVIEW�ؼ������������߽�ľ��붼��38�����ء�
*  3. ICONVIEW�ؼ��ඥ�߽���10�����ء�
*  4. ͼ����ͼ��֮���X������4�����ء�
*  5. ͼ����ͼ��֮���Y������5�����ء�
*
*  |-----------------------------------800------------------------------|---
*  |    | 100|   | 100|   | 100|   | 100|   | 100|   | 100|   | 100|    | 10                                                          |
*  |-38-|-ͼ-|-4-|-ͼ-|-4-|-ͼ-|-4-|-ͼ-|-4-|-ͼ-|-4-|-ͼ-|-4-|-ͼ-|-38-|-----��95 ͼ��߶�
*  |                                                                    | 5
*  |-38-|-ͼ-|-4-|-ͼ-|-4-|-ͼ-|-4-|-ͼ-|-4-|-ͼ-|-4-|-ͼ-|-4-|-ͼ-|-38-|-----��95 ͼ��߶�
*  |                                                                    |
* 480                                                                   |
*  |                                                                    |
*  |                                                                    |
*  |                                                                    |
*  |                                                                    |
*  |                                                                    |
*  |--------------------------------------------------------------------|
*
*********************************************************************************************************
*/

#define ICONVIEW_TBorder   10   /* �ؼ�ICONVIEW���ϱ߾� */
#define ICONVIEW_LBorder   38   /* �ؼ�ICONVIEW����߾� */  
#define ICONVIEW_Width     724  /* �ؼ�ICONVIEW�Ŀ� */  
#define ICONVIEW_Height    100  /* �ؼ�ICONVIEW�ĸ� */  
#define ICONVIEW_YSpace    5    /* �ؼ�ICONVIEW��Y������ */
#define ICONVIEW_XSpace    4    /* �ؼ�ICONVIEW��Y������ */

#define ICON_Width     100      /* �ؼ�ICONVIEW��ͼ��Ŀ�� */  
#define ICON_Height    95       /* �ؼ�ICONVIEW��ͼ��ĸ߶�, ��ͼ��������ı���һ��ĸ߶� */ 

#define SCREEN_Width    800     /* ��ʾ���Ŀ�� */  
#define SCREEN_Height   480     /* ��ʾ���߶�   */ 
#define LineCap         16      /* �ַ�����ĸ���16 */




//ͼ��ṹ��
extern GUI_CONST_STORAGE GUI_BITMAP bmButtonController_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmApp_Settem;
extern GUI_CONST_STORAGE GUI_BITMAP bmADC;
	
GUI_CONST_STORAGE GUI_BITMAP bmButtonController_1 = {
	72, // xSize
	72, // ySize
	288, // BytesPerLine
	32, // BitsPerPixel
	(unsigned char *)_ButtonController_1,  // Pointer to picture data
	NULL,  // Pointer to palette
	GUI_DRAW_BMP8888
};

GUI_CONST_STORAGE GUI_BITMAP bmApp_Settem = {
	72, // xSize
	72, // ySize
	288, // BytesPerLine
	32, // BitsPerPixel
	(unsigned char *)acSettem,  // Pointer to picture data
	NULL,  // Pointer to palette
	GUI_DRAW_BMP8888
};

GUI_CONST_STORAGE GUI_BITMAP bmADC = {
  72, // xSize
  72, // ySize
  288, // BytesPerLine
  32, // BitsPerPixel
  (unsigned char *)_acADC,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMP8888
};


/*
*********************************************************************************************************
*                                       emWin����
*********************************************************************************************************
*/
//extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ_SimSun_16;
//extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16;

/* Ĭ������£�ϵͳ�������GUI_FontHZ16 */
//#define  GUI_SysFontHZ16  GUI_FontHZ16



/*
*********************************************************************************************************
*                               ����ICONVIEW�ؼ��������
*********************************************************************************************************
*/


typedef struct
{
	const GUI_BITMAP * pBitmap;
	const char       * pTextEn;
	const char       * pTextCn;
} BITMAP_ITEM;

typedef struct
{
	int xPos;
	int yPos;
	int Index;
} ITEM_INFO;

typedef struct
{
	int          Pos;
	int          FinalMove;
	ITEM_INFO    * pItemInfo;
} PARA;

/*
*********************************************************************************************************
*                                  Ӧ�ó�����ں���
*********************************************************************************************************
*/
static void(*_apModules0[])(WM_HWIN hWin) =
{
	App_Button_Controller_1,
	App_Settem,
	App_ADC,
	//App_PID_TEM,

};




/******************************************
*�� �� �� : _cbBkWindow
* ����˵�� : ���洰�ڵĻص�����, ������Ҫ�ǻ��Ʊ�������
*	��    �� : pMsg  WM_MESSAGE����ָ�����
*	�� �� ֵ : ��
*******************************************/
static void _cbBkWindow(WM_MESSAGE * pMsg)
{

	switch (pMsg->MsgId)
	{
		

		/* �ػ���Ϣ*/
		case WM_PAINT:
			
		GUI_MEMDEV_WriteAt(hMempic, 0, 0);

		
		break;


	default:
		WM_DefaultProc(pMsg);
		break;
	}
}


/*
*********************************************************************************************************
*	�� �� ��: _cbMotion
*	����˵��: Motion���ڵĻص���������Ҫ������ͼ��Ļ�������
*	��    ��: pMsg  WM_MESSAGE����ָ�����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _cbMotion(WM_MESSAGE * pMsg)
{
	WM_MOTION_INFO * pInfo;
	WM_HWIN          hWin = pMsg->hWin;
	PARA           * pPara;
	static uint32_t  tStart, tEnd;
	int NCode, Id;

	switch (pMsg->MsgId)
	{
	case WM_PRE_PAINT:
	
		break;

	case WM_POST_PAINT:
		
		break;

	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch (Id)
		{
			/* ��һ�������ϵ�ͼ�� */
		case GUI_ID_ICONVIEW0:
			switch (NCode)
			{
				/* ICON�ؼ������Ϣ */
			case WM_NOTIFICATION_CLICKED:
				tStart = GUI_GetTime();
				break;

				/* ICON�ؼ��ͷ���Ϣ */
			case WM_NOTIFICATION_RELEASED:
				tEnd = GUI_GetTime() - tStart;
				if (tEnd > 800)
				{
					WM_SetFocus(WM_HBKWIN);
					break;
				}
				_apModules0[ICONVIEW_GetSel(pMsg->hWinSrc)](WM_HBKWIN);
				break;
			}

			break;

	
		}
		
		
		break;

	case WM_MOTION:
		WM_GetUserData(hWin, &pPara, sizeof(pPara));
		pInfo = (WM_MOTION_INFO *)pMsg->Data.p;
		switch (pInfo->Cmd)
		{

		case WM_MOTION_INIT:

			break;

		case WM_MOTION_MOVE:

			break;

		case WM_MOTION_GETPOS:
			pInfo->xPos = pPara->Pos;
			break;
		}
		break;
	}
}




/*
*********************************************************************************************************
*	�� �� ��: _CreateICONVIEW
*	����˵��: ����ICONVIEW
*	��    �Σ�hParent   ������
*             pBm       ICONVIEW�ϵ�λͼ
*             BitmapNum ICONVIEW��ͼ�����
*             x         x������
*             y         y������
*             w         ICONVIEW��
*             h         ICONVIEW��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static WM_HWIN _CreateICONVIEW(WM_HWIN hParent, const BITMAP_ITEM *pBm, int BitmapNum, int Id, int x, int y, int w, int h)
{
	WM_HWIN hIcon;
	int i;

	/*��ָ��λ�ô���ָ���ߴ��ICONVIEW С����*/
	hIcon = ICONVIEW_CreateEx(x, 					/* С���ߵ��������أ��ڸ������У�*/
		y, 					/* С���ߵ��������أ��ڸ������У�*/
		w,                     /* С���ߵ�ˮƽ�ߴ磨��λ�����أ�*/
		h, 	                /* С���ߵĴ�ֱ�ߴ磨��λ�����أ�*/
		hParent, 				            /* �����ڵľ�������Ϊ0 ������С���߽���Ϊ���棨�������ڣ����Ӵ��� */
		WM_CF_SHOW | WM_CF_HASTRANS,       /* ���ڴ�����ǡ�ΪʹС���������ɼ���ͨ��ʹ�� WM_CF_SHOW */
		0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* Ĭ����0�����������ʵ������������ֱ������ */
		Id, 			                    /* С���ߵĴ���ID */
		ICON_Width, 				        /* ͼ���ˮƽ�ߴ� */
		ICON_Height);						/* ͼ��Ĵ�ֱ�ߴ�,ͼ����ļ������������棬��Ҫ����ICONVIEW�ĸ߶ȣ�����Text��ʾ������*/


											/* ��ICONVIEW С���������ͼ�� */
	for (i = 0; i < BitmapNum; i++)
	{
		ICONVIEW_AddBitmapItem(hIcon, pBm[i].pBitmap, pBm[i].pTextCn);
	}

	//ICONVIEW_SetFont(hIcon, &GUI_SysFontHZ16);
	
	/* ���ó�ʼѡ���ͼ�꣬-1��ʾ��ѡ���κ�ͼ�� */
	ICONVIEW_SetSel(hIcon, -1);
	/* ����С���ߵı���ɫ 32 λ��ɫֵ��ǰ8 λ������alpha��ϴ���Ч��*/
	ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_WHITE | 0x80000000);

	/* ����X����ı߽�ֵΪ0��Ĭ�ϲ���0, Y����Ĭ����0����������Ҳ����һ�����ã������Ժ��޸� */
	ICONVIEW_SetFrame(hIcon, GUI_COORD_X, 0);
	ICONVIEW_SetFrame(hIcon, GUI_COORD_Y, 0);

	/* ����ͼ����x ��y �����ϵļ�ࡣ*/
	ICONVIEW_SetSpace(hIcon, GUI_COORD_X, ICONVIEW_XSpace);
	ICONVIEW_SetSpace(hIcon, GUI_COORD_Y, ICONVIEW_YSpace);

	/* ���ö��뷽ʽ ��5.22�汾�����¼���� */
	ICONVIEW_SetIconAlign(hIcon, ICONVIEW_IA_HCENTER | ICONVIEW_IA_TOP);

	//ICONVIEW_SetTextColor(hIcon, ICONVIEW_CI_UNSEL, 0xF020A0);
	return hIcon;
}


/* ���ڵ�һ��������ICONVIEWͼ��Ĵ��� */
static const BITMAP_ITEM _aBitmapItem1[] =
{

	{ &bmButtonController_1,      "ButtonController",       "JDQ_1" },
	{ &bmApp_Settem,      				"SetTem",       					"SET" },
	{ &bmADC,    									"ADC",                    "ADC"},
	//{ &bmusr1,     "framewin2", "�¶ȿ���" },

};



// USER END
/*������*/
void MainTask(void)
{
//	GUI_JPEG_INFO JpegInfo;
//	uint16_t usY = 0;
	PARA Para;
	PARA *pPara;

	/* ���ݳ�ʼ�� */
	pPara = &Para;
	pPara->Pos = 0;

	GUI_Init();


	GUI_DispStringAt("loading..................",50,50);
	


//	WM_MOTION_Enable(1);    /* ʹ�ܻ��� */

							/* Ĭ����500ms�����ｫ���޸�Ϊ50ms���������һ���̶��Ͼ����������ȣ��ܾ��������ȣ���Ҫ����
							��ΪF429���������ޡ�
							*/
							//WM_MOTION_SetDefaultPeriod(50);
//	WM_MOTION_SetDefaultPeriod(50);
/******************
�����Ҫʹ�ô洢�豸���궨��WM_Set_MEMDEV
*******************/
#ifdef WM_Set_MEMDEV
	WM_SetCreateFlags(WM_CF_MEMDEV);
#endif

#define WM_Set_MEMDEV //ʹ�ܴ洢�豸

	/*�������洰�ڱ���ͼ�񡪡�������������������������*/
	//GUI_JPEG_GetInfo(_ac1, sizeof(_ac1), &JpegInfo);
	hMempic = GUI_MEMDEV_CreateEx(0,
		0,
		LCD_GetXSize(),
		LCD_GetYSize(),
		GUI_MEMDEV_HASTRANS
		);
	GUI_MEMDEV_Select(hMempic);
	
	
	
	GUI_JPEG_Draw(_acdeskbkbmp, sizeof(_acdeskbkbmp), 0, 0);

	GUI_MEMDEV_Select(0);
	
		/*ʹ�����洰�ڣ�ע����Ҫʹ�ܣ����򱳾�ͼƬ�޷����ƣ�*/
		WM_SetCallback(WM_HBKWIN, _cbBkWindow);
	

	
	/* ��3�����������洰�ڵı���ͼƬ ------------------------------------------*/
	
	hMotion = WM_CreateWindowAsChild(0,
		0,
		SCREEN_Width * 3,
		SCREEN_Height,
		WM_HBKWIN,
		WM_CF_MOTION_X | WM_CF_SHOW | WM_CF_HASTRANS,
		_cbMotion,
		sizeof(pPara));

	//WM_SetUserData(hMotion, &pPara, sizeof(pPara));


	/* ��4�����������洰�ڵı���ͼƬ ------------------------------------------*/
	/* ��1������ͼ�� */
	hIcon1 = _CreateICONVIEW(hMotion,
		_aBitmapItem1,
		GUI_COUNTOF(_aBitmapItem1),
		GUI_ID_ICONVIEW0,
		ICONVIEW_LBorder,
		ICONVIEW_TBorder,
		ICONVIEW_Width,
		ICONVIEW_Height * 2);
		
		/*���Դ洢�豸����
	if(hMempic==0){
	GUI_DispStringHCenterAt("is 0 ", 300, 300);
	}
	
	else GUI_DispStringHCenterAt("exist", 300, 300);
	*/



	/* ��ֹ���棬�����Ժ��� */
	(void)hIcon1;
	(void)hIcon2;
	(void)hIcon3;

	/* ��5�����������洰�ڵı���ͼƬ ------------------------------------------*/
	//CreateSysInfoDlg();

	/* ��6�����������洰�ڵı���ͼƬ ------------------------------------------*/
	//CreateStatusDlg();
	while (1)
	{

		GUI_Delay(10);
	}




}





