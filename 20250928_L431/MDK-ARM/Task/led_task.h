#ifndef LED_TASK_H
#define LED_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "app.h"

#define LED_PV_ON HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET)      // 红色LED开启
#define LED_BAT_ON HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET) // 绿色LED开启
#define LED_SYS_ON HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET)   // 蓝色LED开启
#define LED_PV_OFF HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET)       // 红色LED关闭
#define LED_BAT_OFF HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET)  // 绿色LED关闭
#define LED_SYS_OFF HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET)    // 蓝色LED关闭

#define SYS_LED_FLASH HAL_GPIO_TogglePin(SYS_LED_GPIO_Port, SYS_LED_Pin)   /* 系统心跳灯 */
#define LED_PV_FLASH HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin)    // 红色LED闪烁
#define LED_BAT_FLASH HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin) // 蓝色LED闪烁

    void ledState(void);     /* LED控制函数 */
    void Sys_Led_Show(void); /* 更新系统??状?? */
    void led_Init(void);    // 初始化led的状态
#ifdef __cplusplus
}
#endif

#endif
