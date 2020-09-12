 /*********************************      
�������Ź�����
************************************/
#include "wdg.h"
#include "stm32f10x.h"
/*
*********************************************************************************************************
*	�� �� ��: InitIwdg
*	����˵��: �������Ź�ʱ�����ú���
*	��    �Σ�IWDGTime: 0 - 0x0FFF�����õ���256��Ƶ��LSI��ʱ��Ƶ�ʰ�40KHz���㡣
*             256��Ƶ������£���С6.4ms�����26214.4ms��
	���㣺Tout=(4 * 2^PR * (RL+1))/40 ms   
				��Tout = 1*Prescaler*(RL+1)/40 ms
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
void InitIwdg(uint32_t _ulIWDGTime)
{
		
	/* ���ϵͳ�Ƿ�Ӷ������Ź���λ�лָ� */
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{		
		/* �����λ��־ */
		RCC_ClearFlag();
	}
	
	/* ʹ��LSI */
	RCC_LSICmd(ENABLE);
	
	/* �ȴ�ֱ��LSI���� */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

	/* д��0x5555��ʾ�������IWDG_PR ��IWDG_RLR�Ĵ��� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/*  LSI/32 ��Ƶ*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	IWDG_SetReload(_ulIWDGTime);
	
	/* ����IWDG���� */
	IWDG_ReloadCounter();
	
	/* ʹ�� IWDG (LSI oscillator ��Ӳ��ʹ��) */
	IWDG_Enable();		
}

/*
*********************************************************************************************************
*	�� �� ��: IWDG_Feed
*	����˵��: ι������
*	��    �Σ���
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}

