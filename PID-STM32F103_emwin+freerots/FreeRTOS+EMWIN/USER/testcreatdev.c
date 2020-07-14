#include <testcreatdev.h>
#include <GUI.h>
#include <stdio.h>


/*
  C-file generated by Bin2C
  Compiled:    Aug 18 2014 at 17:01:27

  Copyright (C) 2014
  Segger Microcontroller GmbH & Co. KG
  www.segger.com

  Solutions for real time microcontroller applications
*/

/*************************** End of file ****************************/



void TestMain(void)
{

  GUI_MEMDEV_Handle hMemJPEG;
	GUI_JPEG_INFO JpegInfo;
	
	GUI_Init();
	
	GUI_SetBkColor(GUI_BLUE);
	GUI_Clear();

	GUI_JPEG_GetInfo(_ac2,sizeof(_ac2),&JpegInfo);
	
	hMemJPEG=GUI_MEMDEV_CreateEx(0,0,JpegInfo.XSize,JpegInfo.YSize,GUI_MEMDEV_HASTRANS);

	GUI_MEMDEV_Select(hMemJPEG);
	GUI_JPEG_Draw(_ac2,sizeof(_ac2),0,0);
	

	GUI_MEMDEV_Select(0);
	

	GUI_MEMDEV_WriteAt(hMemJPEG,0,0);
	
	
		if(hMemJPEG==0){
	GUI_DispStringHCenterAt("is 0 ", 50, 50);

	}
	
	else GUI_DispStringHCenterAt("exist", 50, 50);
	
	while(1)
	{
		
	GUI_Delay(10);
		
	}

}

