/*
 ******************************************************************************
 * Description  : 引脚定义
 ******************************************************************************
 */
 
#ifndef __APP_H
#define __APP_H


#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "adc_Task.h"
#include "led_task.h"
#include "check_staus_Task.h"
#include "mppt_controller.h"
#include "buck_control_Task.h"
#include "pid_controller.h"
#include "MultiTimer_Task.h"
#include "ButtonTask.h"
#include "oled_Task.h"
#include "stdio.h"
#include "protection_Task.h"



#define PV_MOS_OPEN     HAL_GPIO_WritePin(PV_MOS_GPIO_Port,PV_MOS_Pin,GPIO_PIN_SET);            /* 打开PV防反MOS */
#define PV_MOS_CLOSE    HAL_GPIO_WritePin(PV_MOS_GPIO_Port,PV_MOS_Pin,GPIO_PIN_RESET);          /* 关闭PV防反MOS */




extern uint32_t g_Systime;
extern uint32_t test_key_num;



#ifdef __cplusplus
}
#endif

#endif      /* APP_H */


