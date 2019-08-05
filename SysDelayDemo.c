#include "SysDelay.h"
#include <signal.h>
#include <sys/time.h>
SysObj_t SysObjThis[1];
Task_t Task[10];
Timers_t Task1Timers[10];
int Action_Task1(void *This)
{
	SysStart(This);
	Task1Timers[0].TimerMode=TimerUP_E;
	Task1Timers[0].Timer=0;
        SysObjThis->SysTimerAdd(This,&Task1Timers[0]);
	while (1) {
		SysDelay(This,1000,1,NULL);
		printf("This:Action_Taski1->1\r\n");
		SysDelay(This,2000,1,NULL);
		printf("This:Action_Taski1->2\r\n");
                printf("Timers[0]:%ld",SysObjThis->SysGetTimer(This,&Task1Timers[0]));  

	}
	SysStop(Task);
}
int Action_Task2(void *This)
{
	SysStart(This);
	while (1) {
		SysDelay(This,1000,1,NULL);
		printf("This:Action_Taski2->1\r\n");
		SysDelay(This,2000,1,NULL);
		printf("This:Action_Taski2->2\r\n");
	}
	SysStop(Task);
}
void alarm_handle(int sig){  
 SysObjThis->SysTimeSystick(SysObjThis);
}
void set_time(void){
	struct itimerval itv;
	itv.it_interval.tv_sec=0;
	itv.it_interval.tv_usec=1; //自动装载微值
	itv.it_value.tv_sec=0;
	itv.it_value.tv_usec=1;    //启动值 1微秒
	setitimer(ITIMER_REAL,&itv,NULL);
}  
int main()
{
	CreateSysObj(SysObjThis);
	Task[0].Action=Action_Task1;          
	SysObjThis->SysTaskAdd(SysObjThis,&Task[0]);
	Task[1].Action=Action_Task2;
	SysObjThis->SysTaskAdd(SysObjThis,&Task[1]);
        signal(SIGALRM,alarm_handle);
	set_time();
	while(1)
	{
		SysObjThis->SysTaskLoop(SysObjThis);
	}
	return 0;
}

