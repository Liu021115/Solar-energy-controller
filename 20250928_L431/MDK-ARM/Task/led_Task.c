/*
 ******************************************************************************
 * File name    : led_Task.c
 * 硬件连接
 * LED_RED -------> PB5
 * LED_BLUE ------> PB6
 * LED_GREEN -----> PB7
 * SYS_LED -------> PA12
 *
 ******************************************************************************
 */

#include "led_task.h"


/* LED 状态函数，根据当前的BUCK状态来判定 */
void ledState()
{
    // 第一个 LED 的状态操作。代表PV状态
    if (adc_real.Vin > 10000 && buckState == BUCK_STATE_STOP)   // 没在充电，并且PV大于10V，代表输入已经有电压了，就常亮
    {
        LED_PV_ON;
    }
    else if ( buckState == BUCK_STATE_MPPT || buckState == BUCK_STATE_PID_ADJUSTMENT)   // 在任何充电模式中，都代表在充电，LED就闪烁
    {
        LED_PV_FLASH;
    }
    else
    {
        LED_PV_OFF;
    }

    // 第二个 LED 的状态操作，代表电池状态
    if ((adc_real.Vout > pid.voltage_setpoint - 500 ) && (adc_real.Iout < 1000))     // 电池电压大于目标电压-0.5V，并且电流小于1A，代表充满电，LED常亮
    {
        LED_BAT_ON;
    }
    else
    {
        LED_BAT_FLASH;
    }

    // 第三个 LED 的状态操作，常亮代表系统是否出错
    if (buckState == BUCK_STATE_PROTECTION)
    {
        LED_SYS_ON;
    }
    else
    {
        LED_SYS_OFF;
    }

}
/* LED 状态函数，根据当前的BUCK状态来判定 */


/* 系统LED显示函数 */
void Sys_Led_Show()
{
    SYS_LED_FLASH;
}
/* 系统LED显示函数 */

void led_Init()
{
    LED_PV_OFF;
    LED_BAT_OFF;
    LED_SYS_OFF;
}


