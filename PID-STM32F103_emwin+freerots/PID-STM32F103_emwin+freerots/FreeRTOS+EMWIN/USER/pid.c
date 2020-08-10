#include <pid.h>




extern float ADC1_tem;

extern struct pid 

{
	
	
	float SetTem;    //设定的温度
	float ActualTem;  //实际达到的温度
	float err;           //偏差值
	float err_last;			//上一次的偏差值
	float Kp,Ki,Kd;            //PID参数
	float voltage;				//PWM占空比改变值，PID算法的输出值
	float integral;            //积分项
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

float Abs(float val)    //取绝对值
{
    if(val<0)
    {
        val = -val;
    }
    return val;
}

float pid_realize(float tem)
{ 
u8 index=0;       //积分分离选择
	
	pid.SetTem=tem;
	pid.ActualTem=(float)ADC1_tem;
	pid.err=pid.SetTem-pid.ActualTem;
	if(Abs(pid.err)>20) //温差超过20
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


