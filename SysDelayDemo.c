#include "SysDelay.h"
#include <signal.h>
#include <sys/time.h>
SysObj_t SysObjThis[1];
Task_t Task[10];
Timers_t Task1Timers[10];
//���÷�
//����ʱ
int Action_Task1(void *This)
{
	SysStart(This);
	printf("\r\nAction_Task3:start");
	while (1) {
		SysDelay(This,1000,1,-1,NULL);										 //ʱ��1000MS����һ��
		printf("\r\nAction_Task1(%dms)",1000);
	    SysDelay(This,2000,1,-1,NULL);	
								 //ʱ��2000MS����һ��
		printf("\r\nAction_Task1(%dms)",2000);
    }
	SysStop(Task);
}
//�����̼߳��� ����������Ź�
int Action_Task2(void *This)
{
	SysStart(This);
	SysObjThis->SysWdgRefresh(SysObjThis,1000);
	printf("\r\nAction_Task2:WdgStart");
	while (1) {
		SysDelay(This,100,1,-1,NULL);										 //ʱ��100MS����һ��
		SysObjThis->SysWdgRefresh(SysObjThis,1000);
    }
	SysStop(Task);
}
//�źŵȴ�
int Action_Task3(void *This)
{
	SysStart(This);
	printf("\r\nAction_Task3:Start");
	while (1) {
		SysDelay(This,-1,1,-1,NULL);										 //���õȴ�һ��Ϊ1���ź�  �յ�����һ��
		printf("\r\nAction_Task3:singnal");
    }
	SysStop(Task);
}
//��ʱ��
int Action_Task4(void *This)
{
    SysStart(This);
	printf("\r\nAction_Task4:Start");
	Task1Timers[0].TimerMode=TimerUP_E;
	Task1Timers[0].Timer=0;
    SysObjThis->SysTimerAdd(This,&Task1Timers[0]);// ����ʱ��
    Task1Timers[1].TimerMode=TimerDN_E;
	Task1Timers[1].Timer=5000;
    SysObjThis->SysTimerAdd(This,&Task1Timers[1]);// ����ʱ����0ֹͣ
	while (1) {
		SysDelay(This,1000,1,-1,NULL);
		printf("\r\nThis:Action_Taski1->1");
		SysDelay(This,2000,1,-1,NULL);
		printf("\r\nThis:Action_Taski1->2");
        printf("\r\nTimers0:%ld",SysObjThis->SysGetTimer(This,Action_Task4,0));  //��ӡ����ʱ���е�ֵ
		printf("\r\nTimers1:%ld",SysObjThis->SysGetTimer(This,Action_Task4,1));  //��ӡ����ʱ���е�ֵ
	}
	SysStop(Task);
}
//�źŴ����߳� 
int Action_Task5(void *This)
{
	SysStart(This);
	printf("\r\nAction_Task5:Start");
	while (1) {
		SysDelay(This,3000,1,-1,NULL);
 		SysObjThis->SysTriggerSignal(SysObjThis,Action_Task3,1);       //����Action_Task3�ȴ�1�ź�
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
//��������
int Action_Task4(void *This)
{
	SysStart(This);
	SysObjThis->SysWdgRefresh(SysObjThis,10000);
	printf("\r\nAction_Task3:WdgStart");
	while (1) {
		SysDelay(This,-1,0,-1,GetEventDemo);										 //���õȴ�һ�����źŴ���
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

