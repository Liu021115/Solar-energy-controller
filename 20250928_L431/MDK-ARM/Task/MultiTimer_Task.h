/*
 ******************************************************************************
 * File name    : MultiTimer_Task.h
 ******************************************************************************
 */

#ifndef __MULTITIMER_TASK_H_
#define __MULTITIMER_TASK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MultiTimer.h"
    // #include "app.h"

#define TIMER_1mS 1       /* 定时器1定时时间*/
#define TIMER_10mS 10     /* 定时器2定时时间*/
#define TIMER_5mS 5       /* 定时器2定时时间*/
#define TIMER_100mS 300   /* 定时器4定时时间*/
#define TIMER_500mS 500   /* 定时器3定时时间*/
#define TIMER_1000mS 1000 /* 定时器4定时时间*/

    uint64_t PlatformTicksGetFunc(void);                           /* 定时器获取系统心跳函数，会赋值给MultiTimer 进行配置 */
    void MultiTimerInit(void);                                     /* 初始化定时器 */
    void MultiTimer_Start(void);                                   /* 启动定时器 */
    void Timer_Loop(void);                                         /* 循环定时器函数 */
    void TIMER_1mS_callback(MultiTimer *timer, void *userData);    /* 定时器回调函数 */
    void TIMER_10mS_callback(MultiTimer *timer, void *userData);   /* 定时器回调函数 */
    void TIMER_500mS_callback(MultiTimer *timer, void *userData);  /* 定时器回调函数 */
    void TIMER_1000mS_callback(MultiTimer *timer, void *userData); /* 定时器回调函数 */
    void TIMER_100mS_callback(MultiTimer *timer, void *userData);  /* 定时器回调函数 */
    void TIMER_5mS_callback(MultiTimer *timer, void *userData);    /* 定时器回调函数 */

#ifdef __cplusplus
}
#endif

#endif
