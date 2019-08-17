/*
 * SysDelay.h
 *
 *  Created on: 2019年6月30日
 *      Author: 12123
 */

#ifndef SYSDELAY_H_
#define SYSDELAY_H_
#include<stdio.h>
typedef enum
{
	TimerUP_E,  //加
	TimerDN_E,  //减
}TimersMode_e;
typedef enum
{
    Timer_E,		//阻止一段时间 中断信号正常运行,条件成立接上次运行
	TimerEvent_E,		//阻止一段时间 并且清除中断信号，条件成立接上次运行
	Event_E,                //只阻止中断信号，条件成立接上次运行
	TimerReset_E,           //时间到时重新运行指定任务
	TimerEventReset_E,      //
	EventReset_E,
}SysTaskStopMode_e;
typedef struct Timer_n{
	long Timer;
	char TimerMode;
	struct Timer_n *next;
}Timers_t;
typedef struct{
	long Timer;
	long Singnal;
	long TriggerSingnal;
	long Line;
	void *This;
	Timers_t *Timers;
	long (*event)(void);
	unsigned char  RunSingnal;    //0时间触发 1信号触发 2事件触发

}Task_var_t;
typedef struct task_n{
	Task_var_t Var[1];
	int (*Action)(void *This);
	struct task_n *Next;
}Task_t;
typedef struct TaskObj_n{
	Task_t *Run;
	Task_t *MainThis;
	unsigned char *Ver;
	int  (*SysTaskAdd)(struct TaskObj_n *MainThis,Task_t *ThisObj);                                 //增加新任务
	void (*SysTaskLoop)(struct TaskObj_n *ThisObj);                                                 //循环调度
	void (*SysTimeSystick)(struct TaskObj_n *ThisObj);                                              //系统基准时钟
	long (*SysGetTriggerSingnalMode)(struct TaskObj_n *ThisObj);                                    //获得触发信号-1为时间触发
	void (*SysTimerAdd)(struct TaskObj_n *ThisObj,Timers_t *uTimers); 				                //线程中开启定时器
	unsigned char *(*SysGetVer)(struct TaskObj_n *ThisObj);

	//具备夸线程能力的函数
	void (*SysTaskSleep)(struct TaskObj_n *ThisOjb,int (*Action)(void *This),long tim,SysTaskStopMode_e mode);//柱塞某个线程触发
	long (*SysTriggerSignal)(struct TaskObj_n *ThisObj,int (*Action)(void *This),long Singnal);               //跨线程触发信号标志
	long (*SysGetTimer)(struct TaskObj_n *ThisObj,int (*Action)(void *This),long num);                        //线程中获得定时器时间
	long (*SysGetTriggerSignal)(struct TaskObj_n *ThisObj,int (*Action)(void *This));                         //跨线程获得该线程等待的信号信号标志
	long (*SysSetTimer)(struct TaskObj_n *ThisObj,int (*Action)(void *This),long num,long tim);               //重新设置定线程定时器中的值

}SysObj_t;
void SysSetBreakpoint(SysObj_t *This,long Tim,long SetSingnal,long(*event)(void),long Lin);                       //
void SysResetRun(SysObj_t *This);
long SysGetBreakpoint(SysObj_t *This);                                                         //
SysObj_t *CreateSysObj(SysObj_t *This);
#define SysStart(Task) switch(SysGetBreakpoint(Task)){case 0:               //协程开始
#define SysDelay(Task,Tim,SetSingnal,event)  SysSetBreakpoint(Task,Tim,SetSingnal,event,__LINE__);return __LINE__;case __LINE__:  //挂起线程等待下一次唤醒
#define SysStop(Task)  }return 0;                                             //协程结束
#define SysReset(Task)  SysResetRun(Task);return __LINE__ ;


#endif /* SYSDELAY_H_ */
