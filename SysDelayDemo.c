#include "SysDelay.h"
#include <signal.h>
#include <sys/time.h>
SysObj_t SysObjThis[1];
Task_t Task[10];
int Action_Task1(void *This)
{
	SysStart(This);
	while (1) {
		SysDelay(This,1000,1,NULL);
		printf("This:Action_Taski1->1\r\n");
		SysDelay(This,2000,1,NULL);
		printf("This:Action_Taski1->2\r\n");
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

