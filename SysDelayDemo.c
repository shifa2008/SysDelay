#include "SysDelay.h"
#include <signal.h>
#include <sys/time.h>
SysObj_t SysObjThis[1];
Task_t Task[10];
Timers_t Task1Timers[10];
long EventTsk=0;
int Action_Task3(void *This);
int Action_Task1(void *This)
{
	SysStart(This);
	Task1Timers[0].TimerMode=TimerUP_E;
	Task1Timers[0].Timer=0;
    SysObjThis->SysTimerAdd(This,&Task1Timers[0]);// 正向定时器
    Task1Timers[1].TimerMode=TimerDN_E;
	Task1Timers[1].Timer=5000;
    SysObjThis->SysTimerAdd(This,&Task1Timers[1]);// 反向定时器到0停止
	while (1) {
		SysDelay(This,1000,1,NULL);
		printf("\r\nThis:Action_Taski1->1");
		SysDelay(This,2000,1,NULL);
		printf("\r\nThis:Action_Taski1->2");
        printf("\r\nTimers0:%ld",SysObjThis->SysGetTimer(This,Action_Task1,0));  //打印出定时器中的值
		printf("\r\nTimers1:%ld",SysObjThis->SysGetTimer(This,Action_Task1,1));  //打印出定时器中的值
	}
	SysStop(Task);
}
int Action_Task2(void *This)
{
	SysStart(This);
	while (1) {
		SysDelay(This,1000,1,NULL);
		printf("\r\nThis:Action_Taski2->1");
		SysDelay(This,2000,1,NULL);
		printf("\r\nThis:Action_Taski2->2");
		if(SysObjThis->SysGetTimer(This,Action_Task1,1)<=0)
		{
			SysObjThis->SysSetTimer(SysObjThis,Action_Task1,1,10000);	   //重值Action_Task1中定时器1的值
			SysObjThis->SysTriggerSignal(SysObjThis,Action_Task3,1);       //触发Action_Task3等待信号
			printf("\r\nAction_Task1->Timers1:%ld",SysObjThis->SysGetTimer(This,Action_Task1,1));  //打印出定时器中的值
		}
	}
	SysStop(Task);
}
//信号等待触发
int Action_Task3(void *This)
{
	SysStart(This);
	while (1) {
		SysDelay(This,-1,1,NULL);										 //永久等待一个线信号触发
		printf("\r\nAction_Task3:singnal");
		EventTsk=1;
	    
	}
	SysStop(Task);
}
//函数触发
long GetEventDemo(void)
{
   if(EventTsk!=0)
   {
	EventTsk=0;
	return 1;
   }
   return -1;
}
int Action_Task4(void *This)
{
	SysStart(This);
	while (1) {
		SysDelay(This,-1,0,GetEventDemo);										 //永久等待一个线信号触发
		printf("\r\nAction_Task4:Event");
	    
	}
	SysStop(Task);
}

void alarm_handle(int sig){  
 SysObjThis->SysTimeSystick(SysObjThis);
}
void set_time(void){
	struct itimerval itv;
	itv.it_interval.tv_sec=0;
	itv.it_interval.tv_usec=1; //鑷姩瑁呰浇寰�?
	itv.it_value.tv_sec=0;
	itv.it_value.tv_usec=1;    //鍚姩鍊? 1寰
	setitimer(ITIMER_REAL,&itv,NULL);
}  
int main()
{
	CreateSysObj(SysObjThis);
	Task[0].Action=Action_Task1;          
	SysObjThis->SysTaskAdd(SysObjThis,&Task[0]);
	Task[1].Action=Action_Task2;
	SysObjThis->SysTaskAdd(SysObjThis,&Task[1]);

	Task[2].Action=Action_Task3;
	SysObjThis->SysTaskAdd(SysObjThis,&Task[2]);

	Task[3].Action=Action_Task4;
	SysObjThis->SysTaskAdd(SysObjThis,&Task[3]);
        signal(SIGALRM,alarm_handle);
	set_time();
	while(1)
	{
		SysObjThis->SysTaskLoop(SysObjThis);
	}
	return 0;
}

