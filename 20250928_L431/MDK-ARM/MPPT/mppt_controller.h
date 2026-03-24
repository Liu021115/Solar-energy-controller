#ifndef MPPT_CONTROLLER_H
#define MPPT_CONTROLLER_H

#include "stdint.h"

typedef struct
{
    uint32_t pv_open_voltage;           // PV开路电压
    uint32_t current_pv_target_voltage; // 当前PV电压
    uint32_t prev_pv_target_voltage;    // 上一个PV电压
    uint32_t current_power;             // 当前充电功率
    uint32_t prev_power;                // 上一个充电功率
    uint8_t initialized;                // 是否初始化过PV开路电压
    uint16_t mppt_count;                // mppt的运行次数
} MPPTInfo;

extern MPPTInfo MPPT;

int MPPT_Compute(MPPTInfo *mppt_info, uint32_t pv_voltage, uint32_t output_voltage, uint32_t output_current, uint32_t dutycycle);
void mppt_Init(void);

#endif /* MPPT_CONTROLLER_H */
