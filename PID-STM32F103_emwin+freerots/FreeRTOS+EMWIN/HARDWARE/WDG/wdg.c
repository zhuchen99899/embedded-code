 /*********************************      
独立看门狗驱动
************************************/
#include "wdg.h"
#include "stm32f10x.h"
/*
*********************************************************************************************************
*	函 数 名: InitIwdg
*	功能说明: 独立看门狗时间配置函数
*	形    参：IWDGTime: 0 - 0x0FFF，设置的是256分频，LSI的时钟频率按40KHz计算。
*             256分频的情况下，最小6.4ms，最大26214.4ms。
	计算：Tout=(4 * 2^PR * (RL+1))/40 ms   
				即Tout = 1*Prescaler*(RL+1)/40 ms
*	返 回 值: 无		        
*********************************************************************************************************
*/
void InitIwdg(uint32_t _ulIWDGTime)
{
		
	/* 检测系统是否从独立看门狗复位中恢复 */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{		
		/* 清除复位标志 */
		RCC_ClearFlag();
	}
	
	/* 使能LSI */
	RCC_LSICmd(ENABLE);
	
	/* 等待直到LSI就绪 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

	/* 写入0x5555表示允许访问IWDG_PR 和IWDG_RLR寄存器 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/*  LSI/32 分频*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	IWDG_SetReload(_ulIWDGTime);
	
	/* 重载IWDG计数 */
	IWDG_ReloadCounter();
	
	/* 使能 IWDG (LSI oscillator 由硬件使能) */
	IWDG_Enable();		
}

/*
*********************************************************************************************************
*	函 数 名: IWDG_Feed
*	功能说明: 喂狗函数
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}

