#include <pid.h>




extern float ADC1_tem;

extern struct pid 

{
	
	
	float SetTem;    //�趨���¶�
	float ActualTem;  //ʵ�ʴﵽ���¶�
	float err;           //ƫ��ֵ
	float err_last;			//��һ�ε�ƫ��ֵ
	float Kp,Ki,Kd;            //PID����
	float voltage;				//PWMռ�ձȸı�ֵ��PID�㷨�����ֵ
	float integral;            //������
}pid;




void pid_init()
{
	pid.SetTem=0.0;
	pid.ActualTem=0.0;
	pid.err=0.0;
	pid.err_last=0.0;
	pid.Kp=0.0;
	pid.Ki=0.000;
	pid.Kd=0.00;
	pid.voltage=0;
	pid.integral=0;
	
}

float Abs(float val)    //ȡ����ֵ
{
    if(val<0)
    {
        val = -val;
    }
    return val;
}

float pid_realize(float tem)
{ 
u8 index=0;       //���ַ���ѡ��
	
	pid.SetTem=tem;
	pid.ActualTem=(float)ADC1_tem;
	pid.err=pid.SetTem-pid.ActualTem;
	if(Abs(pid.err)>20) //�²��20
	{
	
	index=0;
	}
	else
	{
	index=1;
	pid.integral+=pid.err;
	}
	pid.voltage=pid.Kp*pid.err+index*pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);
	pid.err_last=pid.err;
	if(pid.voltage>820)
	{
	pid.voltage=820;
	}
	
	return pid.voltage;
}


