/*
 ******************************************************************************
 * File name    : buck_control_Task.c
 * 硬件连接
 * PWMH -------> PA9
 * PWML -------> PB14
 *
 ******************************************************************************
 */

#include "app.h"
#include "tim.h"
#include "buck_control_Task.h"

BuckState buckState = BUCK_STATE_STOP; /* BUCK 状态初始化 */


extern uint32_t mppt_count;

/* BUCK软启动函数 */ /* BUCK软起动函数工作逻辑改成，1ms调整一次占空比，每次+1，知道达到目标电压左右，然后切出软起动模式 */
void softStartBuck(float inputVoltage, float targetVoltage, float softStartTime)
{
    static int StartTime = 0;        // 软启动已经进行的时间
//    float dutyCycleIncrement = 0.0f; // 占空比增加幅度       /* htim1.Init.Period */

    PWM_Start(); /* 开启 PWM 输出 */

    int test_time = (targetVoltage / inputVoltage) * htim1.Init.Period; /* 获取设定软起目标时间，以计算每次占空比更新的步进值 */

    if ((adc_real.Vout < targetVoltage) /* && (StartTime < test_time) */)
    {
        TIM1->CCR2 += 2; /* 在软起模式，每次执行函数便+1 */
        StartTime++;     /* 增加启动时间 */
    }
    else if (adc_real.Vout >= targetVoltage /* || (StartTime >= test_time) */)
    {
        buckState = BUCK_STATE_PID_ADJUSTMENT; /* 这里就是切出软起动模式，进入PID调整模式,只有PID调整需要软起，MPPT不用 */
        StartTime = 0;                         /* 重置运行时间 */
    }
    else
    {

        buckState = BUCK_STATE_PROTECTION; /* 其他状态的话，就报故障 */
        PWM_Stop();                        /* 停止 PWM 输出 */
    }
}
/* BUCK软启动函数 */

/* BUCK状态函数 */ /* 实现方法相当于状态机 */
void controlBuckConverter()
{
    switch (buckState)
    {
    case BUCK_STATE_SOFT_START:
    {
        // 执行软启动操作
        softStartBuck(adc_real.Vin, pid.voltage_setpoint, 500);

        break;
    }
    case BUCK_STATE_MPPT:
    {
        // 执行MPPT调整操作
        
        if (MPPT.initialized == 0)  
        {
            MPPT.pv_open_voltage = adc_real.Vin;
            MPPT.initialized = 1; // 标记为已初始化
        }
        PWM_Start();
        int out = 0;
        out = MPPT_Compute(&MPPT,adc_real.Vin,adc_real.Vout, adc_real.Iout, TIM1->CCR2);
        TIM1->CCR2 = out;        /* 直接赋给占空比的寄存器，加快速度 */

        break;
    }

    case BUCK_STATE_PID_ADJUSTMENT:
    {
        // 执行PID调整操作
        PWM_Start();
        int out = 0;
        
        out = PID_Compute(&pid, adc_real.Vout, adc_real.Iout, TIM1->CCR2); /* 调用 PID 计算函数，算出来占空比的值 */
        TIM1->CCR2 = out;                                                  /* 直接赋给占空比的寄存器，加快速度 */
        // if (adc_real.Vout < pid.voltage_setpoint)
        // {
        //     TIM1->CCR2++;
        // }
        // else if (adc_real.Vout > pid.voltage_setpoint)
        // {
        //     TIM1->CCR2--;
        // }

        break;
    }

    case BUCK_STATE_PROTECTION:
    {
        // 执行保护操作
        PWM_Stop(); /* 停止 PWM 输出 */

        break;
    }

    case BUCK_STATE_STOP:
    {
        // 执行停止操作
        PWM_Stop(); /* 停止 PWM 输出 */
        
        MPPT.mppt_count = 0;     //恢复mppt的计数值
        MPPT.initialized = 0;   // MPPT 初始化除掉，为下一次上电，重新赋值MPPT的条件
        
        break;
    }

    default:
        // 处理未知状态
        break;
    }
}
/* BUCK状态函数 */

void PWM_Start(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);       /* 配置PWM，启动上管 */
    // HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);  /* 配置PWM，启动下管 */
}

void PWM_Stop(void)
{
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);    /* 关闭上管 PWM */
    HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2); /* 关闭下管 PWM */
    TIM1->CCR2 = 0;                             /* 置 0 占空比 */
}
