#ifndef CHECK_STAUS_TASK_H
#define CHECK_STAUS_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "app.h"

#define OPEN_PV_MOS_CURRENT 2000 // 开启PV防反MOS的电流，与下管相同，单位mA
#define OV_PROTECT_VOLTAGE 2000   // 比目标电压高多少就触发过压保护，单位mV

#define ENABLE_CHARGING_DETECTION 1 // 是否启动充电条件检测
#define ENABLE_CC_CV_DETECTION 1    // 是否启动CC，CV转换条件检测
#define ENABLE_PV_MOS_DETECTION 1   // 是否开启PV防反开关的条件检测
#define ENABLE_BAT_OFF_DETECTION 1  // 是否开启电池突然断开的的条件检测
#define ENABLE_STOP_DETECTION 1     // 是否开启停止充电的检测条件
#define ENABLE_UNNORMAL_DETECTION 1 // 是否开启不正常的充电检测条件

    void check_staus(void);         /* 检查系统状态任务状态 */

    extern uint8_t pid_count;

#ifdef __cplusplus
}
#endif

#endif
