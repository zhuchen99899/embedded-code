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
*                                         变量
*********************************************************************************************************
*/

extern GUI_CONST_STORAGE unsigned char _acdeskbkbmp[235020UL + 1];


static GUI_MEMDEV_Handle   hMempic; //存储设备句柄用于绘制桌面背景图片

static WM_HWIN hIcon1, hIcon2, hIcon3;

//static uint8_t s_ucIconSwitch = 0;
WM_HWIN hMotion;

// USER START (Optionally insert additional defines)
// USER END

/*
*********************************************************************************************************
*                                         宏定义
*  ICONVIEW的桌面布局 800*480，每个图标太小是72*72
*  1. 图标的宽度位100像素。
*  2. ICONVIEW控件距左右两个边界的距离都是38个像素。
*  3. ICONVIEW控件距顶边界是10个像素。
*  4. 图标与图标之间的X距离是4个像素。
*  5. 图标与图标之间的Y距离是5个像素。
*
*  |-----------------------------------800------------------------------|---
*  |    | 100|   | 100|   | 100|   | 100|   | 100|   | 100|   | 100|    | 10                                                          |
*  |-38-|-图-|-4-|-图-|-4-|-图-|-4-|-图-|-4-|-图-|-4-|-图-|-4-|-图-|-38-|-----》95 图标高度
*  |                                                                    | 5
*  |-38-|-图-|-4-|-图-|-4-|-图-|-4-|-图-|-4-|-图-|-4-|-图-|-4-|-图-|-38-|-----》95 图标高度
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

#define ICONVIEW_TBorder   10   /* 控件ICONVIEW的上边距 */
#define ICONVIEW_LBorder   38   /* 控件ICONVIEW的左边距 */  
#define ICONVIEW_Width     724  /* 控件ICONVIEW的宽 */  
#define ICONVIEW_Height    100  /* 控件ICONVIEW的高 */  
#define ICONVIEW_YSpace    5    /* 控件ICONVIEW的Y方向间距 */
#define ICONVIEW_XSpace    4    /* 控件ICONVIEW的Y方向间距 */

#define ICON_Width     100      /* 控件ICONVIEW的图标的宽度 */  
#define ICON_Height    95       /* 控件ICONVIEW的图标的高度, 含图标下面的文本，一起的高度 */ 

#define SCREEN_Width    800     /* 显示屏的宽度 */  
#define SCREEN_Height   480     /* 显示屏高度   */ 
#define LineCap         16      /* 字符点阵的高是16 */




//图标结构体
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
*                                       emWin字体
*********************************************************************************************************
*/
//extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ_SimSun_16;
//extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16;

/* 默认情况下，系统字体采用GUI_FontHZ16 */
//#define  GUI_SysFontHZ16  GUI_FontHZ16



/*
*********************************************************************************************************
*                               创建ICONVIEW控件所需变量
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
*                                  应用程序入口函数
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
*函 数 名 : _cbBkWindow
* 功能说明 : 桌面窗口的回调函数, 这里主要是绘制背景窗口
*	形    参 : pMsg  WM_MESSAGE类型指针变量
*	返 回 值 : 无
*******************************************/
static void _cbBkWindow(WM_MESSAGE * pMsg)
{

	switch (pMsg->MsgId)
	{
		

		/* 重绘消息*/
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
*	函 数 名: _cbMotion
*	功能说明: Motion窗口的回调函数，主要是桌面图标的滑动处理
*	形    参: pMsg  WM_MESSAGE类型指针变量
*	返 回 值: 无
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
			/* 第一个界面上的图标 */
		case GUI_ID_ICONVIEW0:
			switch (NCode)
			{
				/* ICON控件点击消息 */
			case WM_NOTIFICATION_CLICKED:
				tStart = GUI_GetTime();
				break;

				/* ICON控件释放消息 */
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
*	函 数 名: _CreateICONVIEW
*	功能说明: 创建ICONVIEW
*	形    参：hParent   父窗口
*             pBm       ICONVIEW上的位图
*             BitmapNum ICONVIEW上图标个数
*             x         x轴坐标
*             y         y轴坐标
*             w         ICONVIEW宽
*             h         ICONVIEW高
*	返 回 值: 无
*********************************************************************************************************
*/
static WM_HWIN _CreateICONVIEW(WM_HWIN hParent, const BITMAP_ITEM *pBm, int BitmapNum, int Id, int x, int y, int w, int h)
{
	WM_HWIN hIcon;
	int i;

	/*在指定位置创建指定尺寸的ICONVIEW 小工具*/
	hIcon = ICONVIEW_CreateEx(x, 					/* 小工具的最左像素（在父坐标中）*/
		y, 					/* 小工具的最上像素（在父坐标中）*/
		w,                     /* 小工具的水平尺寸（单位：像素）*/
		h, 	                /* 小工具的垂直尺寸（单位：像素）*/
		hParent, 				            /* 父窗口的句柄。如果为0 ，则新小工具将成为桌面（顶级窗口）的子窗口 */
		WM_CF_SHOW | WM_CF_HASTRANS,       /* 窗口创建标记。为使小工具立即可见，通常使用 WM_CF_SHOW */
		0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* 默认是0，如果不够现实可设置增减垂直滚动条 */
		Id, 			                    /* 小工具的窗口ID */
		ICON_Width, 				        /* 图标的水平尺寸 */
		ICON_Height);						/* 图标的垂直尺寸,图标和文件都包含在里面，不要大于ICONVIEW的高度，导致Text显示不完整*/


											/* 向ICONVIEW 小工具添加新图标 */
	for (i = 0; i < BitmapNum; i++)
	{
		ICONVIEW_AddBitmapItem(hIcon, pBm[i].pBitmap, pBm[i].pTextCn);
	}

	//ICONVIEW_SetFont(hIcon, &GUI_SysFontHZ16);
	
	/* 设置初始选择的图标，-1表示不选中任何图标 */
	ICONVIEW_SetSel(hIcon, -1);
	/* 设置小工具的背景色 32 位颜色值的前8 位可用于alpha混合处理效果*/
	ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_WHITE | 0x80000000);

	/* 设置X方向的边界值为0，默认不是0, Y方向默认是0，这里我们也进行一下设置，方便以后修改 */
	ICONVIEW_SetFrame(hIcon, GUI_COORD_X, 0);
	ICONVIEW_SetFrame(hIcon, GUI_COORD_Y, 0);

	/* 设置图标在x 或y 方向上的间距。*/
	ICONVIEW_SetSpace(hIcon, GUI_COORD_X, ICONVIEW_XSpace);
	ICONVIEW_SetSpace(hIcon, GUI_COORD_Y, ICONVIEW_YSpace);

	/* 设置对齐方式 在5.22版本中最新加入的 */
	ICONVIEW_SetIconAlign(hIcon, ICONVIEW_IA_HCENTER | ICONVIEW_IA_TOP);

	//ICONVIEW_SetTextColor(hIcon, ICONVIEW_CI_UNSEL, 0xF020A0);
	return hIcon;
}


/* 用于第一屏桌面上ICONVIEW图标的创建 */
static const BITMAP_ITEM _aBitmapItem1[] =
{

	{ &bmButtonController_1,      "ButtonController",       "JDQ_1" },
	{ &bmApp_Settem,      				"SetTem",       					"SET" },
	{ &bmADC,    									"ADC",                    "ADC"},
	//{ &bmusr1,     "framewin2", "温度控制" },

};



// USER END
/*主函数*/
void MainTask(void)
{
//	GUI_JPEG_INFO JpegInfo;
//	uint16_t usY = 0;
	PARA Para;
	PARA *pPara;

	/* 数据初始化 */
	pPara = &Para;
	pPara->Pos = 0;

	GUI_Init();


	GUI_DispStringAt("loading..................",50,50);
	


//	WM_MOTION_Enable(1);    /* 使能滑动 */

							/* 默认是500ms，这里将其修改为50ms，这个参数一定程度上决定的灵敏度，能决定灵敏度，主要还是
							因为F429的性能有限。
							*/
							//WM_MOTION_SetDefaultPeriod(50);
//	WM_MOTION_SetDefaultPeriod(50);
/******************
如果需要使用存储设备，宏定义WM_Set_MEMDEV
*******************/
#ifdef WM_Set_MEMDEV
	WM_SetCreateFlags(WM_CF_MEMDEV);
#endif

#define WM_Set_MEMDEV //使能存储设备

	/*绘制桌面窗口背景图像——————————————*/
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
	
		/*使能桌面窗口（注意需要使能，否则背景图片无法绘制）*/
		WM_SetCallback(WM_HBKWIN, _cbBkWindow);
	

	
	/* 第3步：绘制桌面窗口的背景图片 ------------------------------------------*/
	
	hMotion = WM_CreateWindowAsChild(0,
		0,
		SCREEN_Width * 3,
		SCREEN_Height,
		WM_HBKWIN,
		WM_CF_MOTION_X | WM_CF_SHOW | WM_CF_HASTRANS,
		_cbMotion,
		sizeof(pPara));

	//WM_SetUserData(hMotion, &pPara, sizeof(pPara));


	/* 第4步：绘制桌面窗口的背景图片 ------------------------------------------*/
	/* 第1个界面图标 */
	hIcon1 = _CreateICONVIEW(hMotion,
		_aBitmapItem1,
		GUI_COUNTOF(_aBitmapItem1),
		GUI_ID_ICONVIEW0,
		ICONVIEW_LBorder,
		ICONVIEW_TBorder,
		ICONVIEW_Width,
		ICONVIEW_Height * 2);
		
		/*测试存储设备创建
	if(hMempic==0){
	GUI_DispStringHCenterAt("is 0 ", 300, 300);
	}
	
	else GUI_DispStringHCenterAt("exist", 300, 300);
	*/



	/* 防止警告，留待以后备用 */
	(void)hIcon1;
	(void)hIcon2;
	(void)hIcon3;

	/* 第5步：绘制桌面窗口的背景图片 ------------------------------------------*/
	//CreateSysInfoDlg();

	/* 第6步：绘制桌面窗口的背景图片 ------------------------------------------*/
	//CreateStatusDlg();
	while (1)
	{

		GUI_Delay(10);
	}




}





