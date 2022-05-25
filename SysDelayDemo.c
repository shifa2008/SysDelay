#include "SysDelay.h"
#include <signal.h>
#include <sys/time.h>
SysObj_t SysObjThis[1];
Task_t Task[10];
Timers_t Task1Timers[10];
//简单用法
//简单延时
int Action_Task1(void *This)
{
	SysStart(This);
	printf("\r\nAction_Task3:start");
	while (1) {
		SysDelay(This,1000,1,-1,NULL);										 //时间1000MS触发一次
		printf("\r\nAction_Task1(%dms)",1000);
	    SysDelay(This,2000,1,-1,NULL);	
								 //时间2000MS触发一次
		printf("\r\nAction_Task1(%dms)",2000);
    }
	SysStop(Task);
}
//开启线程监视 类似软件看门狗
int Action_Task2(void *This)
{
	SysStart(This);
	SysObjThis->SysWdgRefresh(SysObjThis,1000);
	printf("\r\nAction_Task2:WdgStart");
	while (1) {
		SysDelay(This,100,1,-1,NULL);										 //时间100MS触发一次
		SysObjThis->SysWdgRefresh(SysObjThis,1000);
    }
	SysStop(Task);
}
//信号等待
int Action_Task3(void *This)
{
	SysStart(This);
	printf("\r\nAction_Task3:Start");
	while (1) {
		SysDelay(This,-1,1,-1,NULL);										 //永久等待一个为1的信号  收到触发一次
		printf("\r\nAction_Task3:singnal");
    }
	SysStop(Task);
}
//定时器
int Action_Task4(void *This)
{
    SysStart(This);
	printf("\r\nAction_Task4:Start");
	Task1Timers[0].TimerMode=TimerUP_E;
	Task1Timers[0].Timer=0;
    SysObjThis->SysTimerAdd(This,&Task1Timers[0]);// 正向定时器
    Task1Timers[1].TimerMode=TimerDN_E;
	Task1Timers[1].Timer=5000;
    SysObjThis->SysTimerAdd(This,&Task1Timers[1]);// 反向定时器到0停止
	while (1) {
		SysDelay(This,1000,1,-1,NULL);
		printf("\r\nThis:Action_Taski1->1");
		SysDelay(This,2000,1,-1,NULL);
		printf("\r\nThis:Action_Taski1->2");
        printf("\r\nTimers0:%ld",SysObjThis->SysGetTimer(This,Action_Task4,0));  //打印出定时器中的值
		printf("\r\nTimers1:%ld",SysObjThis->SysGetTimer(This,Action_Task4,1));  //打印出定时器中的值
	}
	SysStop(Task);
}
//信号触发线程 
int Action_Task5(void *This)
{
	SysStart(This);
	printf("\r\nAction_Task5:Start");
	while (1) {
		SysDelay(This,3000,1,-1,NULL);
 		SysObjThis->SysTriggerSignal(SysObjThis,Action_Task3,1);       //触发Action_Task3等待1信号
	}
	SysStop(Task);
}

/*
long GetEventDemo(void)
{
   if(EventTsk!=0)
   {
	EventTsk=0;
	return 1;
   }
   return -1;
}
//函数触发
int Action_Task4(void *This)
{
	SysStart(This);
	SysObjThis->SysWdgRefresh(SysObjThis,10000);
	printf("\r\nAction_Task3:WdgStart");
	while (1) {
		SysDelay(This,-1,0,-1,GetEventDemo);										 //永久等待一个线信号触发
	    SysObjThis->SysWdgRefresh(SysObjThis,10000);
		printf("\r\nAction_Task4:Event");
	    
	}
	SysStop(Task);
}*/

void alarm_handle(int sig){  
 SysObjThis->SysTimeSystick(SysObjThis);
}
void set_time(void){
	struct itimerval itv;
	itv.it_interval.tv_sec=0;
	itv.it_interval.tv_usec=1000; 
	itv.it_value.tv_sec=0;
	itv.it_value.tv_usec=1000;  
	setitimer(ITIMER_REAL,&itv,NULL);
}  
int main()
{
             printf("\r\nstart\r\n");
	CreateSysObj(SysObjThis);
	Task[0].Action=Action_Task1;          
	SysObjThis->SysTaskAdd(SysObjThis,&Task[0]);
	Task[1].Action=Action_Task2;
	SysObjThis->SysTaskAdd(SysObjThis,&Task[1]);

	Task[2].Action=Action_Task3;
	SysObjThis->SysTaskAdd(SysObjThis,&Task[2]);

	Task[3].Action=Action_Task4;
	SysObjThis->SysTaskAdd(SysObjThis,&Task[3]);
	
	Task[4].Action=Action_Task5;
	SysObjThis->SysTaskAdd(SysObjThis,&Task[4]);
 	signal(SIGALRM,alarm_handle);
                printf("\r\nalarm_handle\r\n");
	set_time();

	while(1)
	{
		SysObjThis->SysTaskLoop(SysObjThis);
	}
	return 0;
}

