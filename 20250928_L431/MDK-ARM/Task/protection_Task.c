/*
 ******************************************************************************
 * File name    : protection_Task.c
 * Description  :
 ******************************************************************************
 */

#include "protection_Task.h"



uint8_t SYSTEM_PROTECTION_ARRAYS[PROTECTION_NUM]={OK};



static void Check_Vin_Protection(uint32_t Vin) 
{
    static uint16_t delay_counter_OV = 0;  // 静态计时器，计时输入过压

    // 快速过压保护 (电压超过瞬时过压阈值，立即触发)
    if (Vin > VIN_INSTANT_OV_THRESHOLD) 
    {
        // 处理输入过压保护的内容
        VIN_OV_STATUS = ERROR;
        buckState = BUCK_STATE_STOP;
    }

    // 延迟过压保护
    if (Vin > VIN_NORMAL_OV_THRESHOLD) 
    {
        delay_counter_OV++; // 计时器增加
        if (delay_counter_OV >= VIN_OV_DELAY_TIME) 
        {
            // 处理输入过压保护的内容
            VIN_OV_STATUS = ERROR;
            buckState = BUCK_STATE_STOP;
        }
    } 
    else 
    {
        delay_counter_OV = 0;
    }

    // 恢复逻辑
    if ((Vin < VIN_RECOVERY_THRESHOLD)) 
    {
        // 处理恢复的保护的内容
        VIN_OV_STATUS = OK;
    }
}

static void Check_Vout_Protection(uint32_t Vout) 
{
    static uint16_t delay_counter_OV = 0;  // 静态计时器，计时输出过压

    // 快速过压保护 (电压超过瞬时过压阈值，立即触发)
    if (Vout > VOUT_INSTANT_OV_THRESHOLD) 
    {
        // 处理输出过压保护的内容
        VOUT_OV_STATUS = ERROR;
        buckState = BUCK_STATE_STOP;
    }

    // 延迟过压保护
    if (Vout > VOUT_NORMAL_OV_THRESHOLD) 
    {
        delay_counter_OV++; // 计时器增加
        if (delay_counter_OV >= VOUT_OV_DELAY_TIME) 
        {
            // 处理输出过压保护的内容
            VOUT_OV_STATUS = ERROR;
            buckState = BUCK_STATE_STOP;
        }
    } 
    else 
    {
        delay_counter_OV = 0;
    }

    // 恢复逻辑
    if (Vout < VOUT_RECOVERY_THRESHOLD) 
    {
        // 处理恢复的保护的内容
        VOUT_OV_STATUS = OK;
    }
}

static void Check_Iout_Protection(uint32_t Iout) 
{
    static uint16_t delay_counter_OC = 0;  // 静态计时器，计时输出过流

    // 快速过流保护 (电流超过瞬时过流阈值，立即触发)
    if (Iout > IOUT_INSTANT_OC_THRESHOLD) 
    {
        // 处理输出过流保护的内容
        IOUT_OC_STATUS = ERROR;
        buckState = BUCK_STATE_STOP;
    }

    // 延迟过流保护
    if (Iout > IOUT_NORMAL_OC_THRESHOLD) 
    {
        delay_counter_OC++; // 计时器增加
        if (delay_counter_OC >= IOUT_OC_DELAY_TIME) 
        {
            // 处理输出过流保护的内容
            IOUT_OC_STATUS = ERROR;
            buckState = BUCK_STATE_STOP;
        }
    } 
    else 
    {
        delay_counter_OC = 0;
    }

    // 恢复逻辑
    if (Iout < IOUT_RECOVERY_THRESHOLD) 
    {
        // 处理恢复的保护的内容
        IOUT_OC_STATUS = OK;
    }
}

static void Check_OverTemperature_Protection(int temperature) 
{
    static uint16_t delay_counter_OT = 0;  // 静态计时器，计时过温

    // 快速过温保护 (温度超过瞬时过温阈值，立即触发)
    if (temperature > TEMP_INSTANT_OT_THRESHOLD) 
    {
        // 处理过温保护的内容
        TEMP_OT_STATUS = ERROR;
        buckState = BUCK_STATE_STOP;
    }

    // 延迟过温保护
    if (temperature > TEMP_NORMAL_OT_THRESHOLD) 
    {
        delay_counter_OT++; // 计时器增加
        if (delay_counter_OT >= TEMP_OT_DELAY_TIME) 
        {
            // 处理过温保护的内容
            TEMP_OT_STATUS = ERROR;
            buckState = BUCK_STATE_STOP;
        }
    } 
    else 
    {
        delay_counter_OT = 0;
    }

    // 恢复逻辑
    if (temperature < TEMP_RECOVERY_OT_THRESHOLD) 
    {
        // 处理恢复的保护的内容
        TEMP_OT_STATUS = OK;
    }
}

static void Check_UnderTemperature_Protection(int temperature) 
{
    static uint16_t delay_counter_UT = 0;  // 静态计时器，计时欠温

    // 快速欠温保护 (温度低于瞬时欠温阈值，立即触发)
    if (temperature < TEMP_INSTANT_UT_THRESHOLD) 
    {
        // 处理欠温保护的内容
        TEMP_UT_STATUS = ERROR;
        buckState = BUCK_STATE_STOP;
    }

    // 延迟欠温保护
    if (temperature < TEMP_NORMAL_UT_THRESHOLD) 
    {
        delay_counter_UT++; // 计时器增加
        if (delay_counter_UT >= TEMP_UT_DELAY_TIME) 
        {
            // 处理欠温保护的内容
            TEMP_UT_STATUS = ERROR;
            buckState = BUCK_STATE_STOP;
        }
    } 
    else 
    {
        delay_counter_UT = 0;
    }

    // 恢复逻辑
    if (temperature > TEMP_RECOVERY_UT_THRESHOLD) 
    {
        // 处理恢复的保护的内容
        TEMP_UT_STATUS = OK;
    }
}


void Protection_Task() 
{
    uint8_t sum = 0;

    Check_Vin_Protection(adc_real.Vin);
    Check_Vout_Protection(adc_real.Vout);
    Check_Iout_Protection(adc_real.Iout);
    Check_OverTemperature_Protection(adc_real.Temp);
    Check_UnderTemperature_Protection(adc_real.Temp);

    // 从数组的第二个元素 (index 1) 开始求和，这里是保证系统的状态是根据总的报错信息来的，顺便还可以求得报错总数
    for (int i = 1; i < PROTECTION_NUM; i++) 
    {
        sum += SYSTEM_PROTECTION_ARRAYS[i];
    }

    SYSTEM_STATUS = sum;    // 只要系统状态不等于0，那就说明有错，有错多少个，刚好也是这个数组搞定
}









