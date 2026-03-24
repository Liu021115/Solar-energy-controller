#ifndef PROTECTION_TASK_H
#define PROTECTION_TASK_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "app.h"

// 输入电压保护阈值定义
#define VIN_NORMAL_OV_THRESHOLD 50000  // 正常过压阈值 (单位: mV)
#define VIN_INSTANT_OV_THRESHOLD 60000 // 瞬时过压阈值 (单位: mV)
#define VIN_RECOVERY_THRESHOLD 48000   // 恢复电压阈值 (单位: mV)
#define VIN_OV_DELAY_TIME 1000         // 过压延迟时间 (单位: ms)

// 输出电压保护阈值定义
#define VOUT_INSTANT_OV_THRESHOLD 18000 // 瞬时过压阈值，(单位: mV)
#define VOUT_NORMAL_OV_THRESHOLD 16000  // 正常过压阈值，(单位: mV)
#define VOUT_RECOVERY_THRESHOLD 15000   // 恢复阈值，(单位: mV)
#define VOUT_OV_DELAY_TIME 500          // 延迟过压保护时间，(单位: ms)

#define IOUT_INSTANT_OC_THRESHOLD 15000 // 瞬时过流阈值，例如15A
#define IOUT_NORMAL_OC_THRESHOLD 13000  // 正常过流阈值，例如13A
#define IOUT_RECOVERY_THRESHOLD 10000   // 恢复阈值，例如10A
#define IOUT_OC_DELAY_TIME 100         // 延迟过流保护时间，例如100ms

#define TEMP_INSTANT_OT_THRESHOLD 80 // 瞬时过温阈值，例如80°C
#define TEMP_NORMAL_OT_THRESHOLD 65   // 正常过温阈值，例如65°C
#define TEMP_RECOVERY_OT_THRESHOLD 50 // 恢复阈值，例如50°C
#define TEMP_OT_DELAY_TIME 3000        // 延迟过温保护时间，例如3000ms

#define TEMP_INSTANT_UT_THRESHOLD -10 // 瞬时欠温阈值，例如 -10°C
#define TEMP_NORMAL_UT_THRESHOLD 0   // 正常欠温阈值，例如 0°C
#define TEMP_RECOVERY_UT_THRESHOLD 5  // 恢复阈值，例如 5°C
#define TEMP_UT_DELAY_TIME 500        // 延迟欠温保护时间，例如500ms


#define ERROR 1 // 定义错误状态，表示保护触发或错误发生
#define OK 0    // 定义正常状态，表示系统正常运行或保护未触发

#define PROTECTION_NUM 10 // 定义系统保护状态数组的大小，代表支持的保护状态数量

// 保护状态索引定义，用于访问 SYSTEM_PROTECTION_ARRAYS 中对应的保护状态
#define SYSTEM 0  // 系统整体状态
#define VIN_OV 1  // 输入过压保护
#define VIN_UV 2  // 输入欠压保护
#define VOUT_OV 3 // 输出过压保护
#define IOUT_OC 4 // 输出过流保护
#define TEMP_OT 5 // 过温保护
#define TEMP_UT 6 // 欠温保护

// 状态访问宏，通过 SYSTEM_PROTECTION_ARRAYS 数组获取或设置各个保护状态
#define SYSTEM_STATUS SYSTEM_PROTECTION_ARRAYS[SYSTEM]   // 系统整体状态
#define VIN_OV_STATUS SYSTEM_PROTECTION_ARRAYS[VIN_OV]   // 输入过压保护状态
#define VIN_UV_STATUS SYSTEM_PROTECTION_ARRAYS[VIN_UV]   // 输入欠压保护状态
#define VOUT_OV_STATUS SYSTEM_PROTECTION_ARRAYS[VOUT_OV] // 输出过压保护状态
#define IOUT_OC_STATUS SYSTEM_PROTECTION_ARRAYS[IOUT_OC] // 输出过流保护状态
#define TEMP_OT_STATUS SYSTEM_PROTECTION_ARRAYS[TEMP_OT] // 过温保护状态
#define TEMP_UT_STATUS SYSTEM_PROTECTION_ARRAYS[TEMP_UT] // 欠温保护状态

    extern uint8_t SYSTEM_PROTECTION_ARRAYS[PROTECTION_NUM];

    void Protection_Task(void);

#ifdef __cplusplus
}
#endif

#endif
