/*
 ******************************************************************************
 * Copyright (C), 2022-2032, shanzhen.Co.Ltd. All rights reserved
 * File name    : bsp_sample.c
 * Description  :
 ******************************************************************************
 */

#include "MultiTimer_Task.h"
#include "oled.h"
#include "app.h"

MultiTimer timer_1ms;    /* 定时任务1ms的实例初始化*/
MultiTimer timer_5ms;    /* 定时任务5ms的实例初始化*/
MultiTimer timer_10ms;   /* 定时任务10ms的实例初始化*/
MultiTimer timer_500ms;  /* 定时任务500ms的实例初始化*/
MultiTimer timer_1000ms; /* 定时任务1000ms的实例初始化*/
MultiTimer timer_100ms;  /* 定时任务100ms的实例初始化*/

/* 获取系统心跳函数，会赋值给MultiTimer 进行配置 */
uint64_t PlatformTicksGetFunc(void)
{
    return (uint64_t)HAL_GetTick();
}
/* 获取系统心跳函数，会赋值给MultiTimer 进行配置 */

/* 初始化Multi Timer 定时器相关设置 */
void MultiTimerInit(void)
{
    MultiTimerInstall(PlatformTicksGetFunc); /* 安装 Multi Timer */
    MultiTimer_Start(); /* 启动循环定时器任务 */
}
/* 初始化Multi Timer 定时器相关设置 */

/* 启  动Multi Timer定时器 */
void MultiTimer_Start(void)
{
    MultiTimerStart(&timer_1ms, TIMER_1mS, TIMER_1mS_callback, NULL); /* 启动定时器 1  */
    MultiTimerStart(&timer_5ms, TIMER_5mS, TIMER_5mS_callback, NULL);
    MultiTimerStart(&timer_10ms, TIMER_10mS, TIMER_10mS_callback, NULL);       /* 启动定时器 2  */
    MultiTimerStart(&timer_500ms, TIMER_500mS, TIMER_500mS_callback, NULL);    /* 启动定时器 3  */
    MultiTimerStart(&timer_1000ms, TIMER_1000mS, TIMER_1000mS_callback, NULL); /* 启动定时器 4  */
    MultiTimerStart(&timer_100ms, TIMER_100mS, TIMER_100mS_callback, NULL);    /* 启动定时器 4  */
}
/* 启  动Multi Timer定时器 */

/* 封装一层循环启动函数，这样就不用在main.c里面包含定时器的头文件了 */
void Timer_Loop(void)
{
    MultiTimerYield();
}

/* 定时器1任务实现 */
void TIMER_1mS_callback(MultiTimer *timer, void *userData)
{
    

    /* 2023年9月12日  测试下来，整个1ms任务完成需要 100us 左右 */

    // HAL_GPIO_WritePin(SYS_LED_GPIO_Port,SYS_LED_Pin,GPIO_PIN_SET);
    processTypes(ADC_Value, ADC_Channel_Num); /* 处理每个类型的ADC，并且进行滤波 */
    ADC_To_Real();                            /* 将当前IO的ADC的值，转换成实际的值 */   
    check_staus(); 
    controlBuckConverter();                   /* 控制 Buck 转换器，进行PID算法控制 */
    Protection_Task();
    MultiTimerStart(&timer_1ms, TIMER_1mS, TIMER_1mS_callback, NULL);
    // HAL_GPIO_WritePin(SYS_LED_GPIO_Port,SYS_LED_Pin,GPIO_PIN_RESET);
}
/* 定时器1任务实现 */

/* 定时器2任务实现 */
void TIMER_10mS_callback(MultiTimer *timer, void *userData)
{
    MultiTimerStart(&timer_10ms, TIMER_10mS, TIMER_10mS_callback, NULL);
}
/* 定时器2任务实现 */

/* 定时器3任务实现 */
void TIMER_100mS_callback(MultiTimer *timer, void *userData)
{
    MultiTimerStart(&timer_100ms, TIMER_100mS, TIMER_100mS_callback, NULL);
    
    UpdateOLEDDisplay(); /* OLED 屏幕显示内容处理函数 */
    
}

/* 定时器3任务实现 */

/* 定时器3任务实现 */
void TIMER_5mS_callback(MultiTimer *timer, void *userData)
{
    button_ticks();
    MultiTimerStart(&timer_5ms, TIMER_5mS, TIMER_5mS_callback, NULL);
}

/* 定时器3任务实现 */
void TIMER_500mS_callback(MultiTimer *timer, void *userData)
{

    MultiTimerStart(&timer_500ms, TIMER_500mS, TIMER_500mS_callback, NULL);

    Sys_Led_Show();      /* 系统LED灯状态更新 */
    ledState();          /* 更新LED灯的状态 */
    
}

/* 定时器3任务实现 */

/* 定时器4任务实现 */
void TIMER_1000mS_callback(MultiTimer *timer, void *userData)
{

    MultiTimerStart(&timer_1000ms, TIMER_1000mS, TIMER_1000mS_callback, NULL);
    ADC_To_Real_Temp();

    // static uint8_t stop_wait_time = 0;

    /* 等待两秒后,才进行软起然后到恒压输出 保证一上电先没有输出 */
    // if (buckState == BUCK_STATE_STOP && adc_real.Vin > 20000/*  && systemStatus == SYSTEM_NORMAL */)
    // {
    //     stop_wait_time++;
    //     if (stop_wait_time >= 2)
    //     {
    //         buckState = BUCK_STATE_MPPT;
    //         stop_wait_time = 0;
    //         PWM_Start(); /* 开启 PWM 输出 */
    //     }
    // }
    /* 等待两秒后,才进行软起然后到恒压输出 */
}

/* 定时器4任务实现 */

#if 0 // test code
#if defined(__CC_ARM)

int32_t $sub$$main(void) 
{
	bsp_common_init();
    bsp_sample_init();
    while(1)
    {
        bsp_delay_ms(10);
    }
}

#endif
#endif
