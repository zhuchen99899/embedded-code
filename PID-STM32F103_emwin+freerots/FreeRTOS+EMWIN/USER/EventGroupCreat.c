#include <EventGroupCreat.h>
#include <freertos.h>
#include "event_groups.h"


extern EventGroupHandle_t EventGroupHandler;	//�¼���־����

void EventGroupCreat(void)
{

	/* �����¼���־�� */
	EventGroupHandler = xEventGroupCreate();
	
	if(EventGroupHandler == NULL)
    {
       printf("�¼���־�鴴��ʧ��");
    }

	else
	{
	
	printf("�¼���־�鴴���ɹ�");
	}


	}



