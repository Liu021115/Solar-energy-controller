/*
 ******************************************************************************
 * File name    : oled_Task.c
 * Description  : OLED显示相关的任务
 *
 * OLED 硬件连接
 * OLED_D0 --------> PC10
 * OLED_D1 --------> PC12
 * OLED_RES -------> PC7
 * OLED_DC --------> PC8
 * OLED_CS --------> PC9
 *
 ******************************************************************************
 */

#include "oled_Task.h"
#include "string.h"
#include <stdint.h>
#include <stdlib.h>
#include "oled.h"
#include "app.h"

OLEDPageState oled_page = PAGE_RUN;  /* 初始化 OELD 页面显示实例 */
SetMode set_mode = SET_MODE_VOLTAGE; /* 初始化设定对象实例 */

extern uint32_t mppt_adj;

char error_code[20];  // 假设最大长度为 20

static void update_error_code(uint8_t *array, int array_size,char *error_code) 
{
    // 清空 error_code 字符串
    strcpy(error_code, "");

    // 遍历数组
    if (array[0] > 0)
    {
        for (int i = 1; i < array_size; i++) 
        {
            if (array[i] > 0) {
                // 如果当前元素大于 0，构建 "E" + 元素索引 + ","
                char temp[10];
                snprintf(temp, sizeof(temp), "E%d,", i); // 假设 "位置1" 对应索引 0
                strcat(error_code, temp);
            }
        }
    }
    else
    {
        strcpy(error_code, "E0");
    }

    // 如果 error_code 最后有逗号，去掉它
    int len = strlen(error_code);
    if (len > 0 && error_code[len - 1] == ',') {
        error_code[len - 1] = '\0';
    }
}


/**
 * OLED显示函数
 */
void UpdateOLEDDisplay()
{

    switch (oled_page)
    {
    case PAGE_RUN:
        Page_Run_Content();
        break;
    case PAGE_SETTING:
        Page_Setting_Content();
        break;
    case PAGE_OTHER:
        Page_Other_Content();
        break;
    case PAGE_DEBUG:
        Page_Debug_Content();
        break;
    default:
        break;
    }
}

/**
 * OLED，运行页面显示内容函数
 */
void Page_Run_Content()
{
    uint8_t font_size = 12;
    char temp_str[10];  // 用于存储格式化后的温度字符串
    sprintf(temp_str, "%d^C", adc_real.Temp);  // 无论正负，%d 会自动处理符号

    OLED_ShowString(0, 0, (unsigned char *)FormatMeasurement("Input:", adc_real.Vin, "V  "), font_size, 1);
    OLED_ShowString(0, font_size * 1, (unsigned char *)FormatMeasurement("Ouput:", adc_real.Vout, "V  "), font_size, 1);
    OLED_ShowString(0, font_size * 2, (unsigned char *)FormatMeasurement("Iout :", adc_real.Iout, "A  "), font_size, 1);

    OLED_ShowString(0, font_size * 3, (unsigned char *)"Temp :", font_size, 1);
    OLED_ShowString(42, font_size * 3, (unsigned char *)temp_str, font_size, 1);
    
    OLED_ShowString(0, font_size * 4, (unsigned char *)"Error:", font_size, 1);    
    update_error_code(SYSTEM_PROTECTION_ARRAYS,PROTECTION_NUM,error_code);
    OLED_ShowString(42, font_size * 4, (unsigned char *)"            ", font_size, 1);  // 清空报错代码
    OLED_ShowString(42, font_size * 4, (unsigned char *)error_code, font_size, 1);

    if ((pid.pid_state == CONSTANT_CURRENT) && (buckState != BUCK_STATE_MPPT) && (buckState != BUCK_STATE_STOP))
    {
        OLED_ShowString(96, 0, (unsigned char *)"CC", font_size * 2, 1);
    }
    else if ((pid.pid_state == CONSTANT_VOLTAGE) && (buckState != BUCK_STATE_MPPT)  && (buckState != BUCK_STATE_STOP))
    {
        OLED_ShowString(96, 0, (unsigned char *)"CV", font_size * 2, 1);
    }
    else if ((buckState == BUCK_STATE_MPPT))
    {
        OLED_ShowString(96, 0, (unsigned char *)"MP", font_size * 2, 1);
    }
    else if ((buckState == BUCK_STATE_STOP))
    {
        OLED_ShowString(96, 0, (unsigned char *)"SP", font_size * 2, 1);
    }


    if (set_mode == SET_MODE_CURRENT)
    {
        OLED_ShowString(96, 24, (unsigned char *)"SET-I", font_size, 1);
    }
    else
    {
        OLED_ShowString(96, 24, (unsigned char *)"SET-V", font_size, 1);
    }

    OLED_Refresh();
}

/**
 * OLED，设置页面显示内容函数
 */
void Page_Setting_Content()
{
    uint8_t font_size = 12;
    OLED_ShowString(1, 0, (unsigned char *)FormatMeasurement("Voltage:", pid.voltage_setpoint, " V"), font_size, 1);
    OLED_ShowString(1, font_size * 1, (unsigned char *)FormatMeasurement("Current:", pid.current_setpoint, " A"), font_size, 1);
    if (set_mode == SET_MODE_CURRENT)
    {
        OLED_ShowString(96, 24, (unsigned char *)"SET-I", font_size, 1);
    }
    else
    {
        OLED_ShowString(96, 24, (unsigned char *)"SET-V", font_size, 1);
    }

    OLED_Refresh();
}

/**
 * OLED，其他信息页面显示内容函数
 */
void Page_Other_Content()
{
    uint8_t font_size = 12;

    OLED_ShowString(0, 0, (unsigned char *)"Hardware Version:", font_size, 1);
    OLED_ShowString(0, font_size * 1, (unsigned char *)"---V1.0---", font_size, 1);
    OLED_ShowString(0, font_size * 2, (unsigned char *)"Software Version:", font_size, 1);
    OLED_ShowString(0, font_size * 3, (unsigned char *)"---V2.0---", font_size, 1);
    OLED_ShowString(0, font_size * 4, (unsigned char *)"Author : FN @ 2025", font_size, 1);
    OLED_Refresh();
}

/**
 * OLED，DEBUG页面显示内容函数
 */
void Page_Debug_Content()
{
    uint8_t font_size = 12;

    OLED_ShowString(0, 0, (unsigned char *)FormatMeasurement("Input:", adc_real.Vin, "V"), font_size, 1);
    OLED_ShowString(0, font_size * 1, (unsigned char *)FormatMeasurement("Ouput:", adc_real.Vout, "V"), font_size, 1);
    OLED_ShowString(0, font_size * 2, (unsigned char *)FormatMeasurement("Iout :", adc_real.Iout, "A"), font_size, 1);
    OLED_ShowString(0, font_size * 3, (unsigned char *)FormatMeasurement("Duty :", TIM1->CCR2 * 1000, " "), font_size, 1);  
    OLED_ShowString(0, font_size * 4, (unsigned char *)FormatMeasurement("T :", MPPT.current_pv_target_voltage, "C"), font_size, 1);
    OLED_ShowString(64, font_size * 4, (unsigned char *)FormatMeasurement("", mppt_adj*1000 , ""), font_size, 1);
    if (buckState == BUCK_STATE_PID_ADJUSTMENT)
    {
        OLED_ShowString(96, 0, (unsigned char *)"PI", font_size * 2, 1);
    }
    else if (buckState == BUCK_STATE_MPPT)
    {
        OLED_ShowString(96, 0, (unsigned char *)"MP", font_size * 2, 1);
    }
    else if (buckState == BUCK_STATE_STOP)
    {
        OLED_ShowString(96, 0, (unsigned char *)"SP", font_size * 2, 1);
    }
    else if (buckState == BUCK_STATE_SOFT_START)
    {
        OLED_ShowString(96, 0, (unsigned char *)"SS", font_size * 2, 1);
    }
    else
    {
        OLED_ShowString(96, 0, (unsigned char *)"ER", font_size * 2, 1);
    }

    if (pid.pid_state == CONSTANT_CURRENT)
    {
        OLED_ShowString(96, font_size * 3, (unsigned char *)"CC", font_size * 2, 1);
    }
    else if (pid.pid_state == CONSTANT_VOLTAGE)
    {
        OLED_ShowString(96, font_size * 3, (unsigned char *)"CV", font_size * 2, 1);
    }
    
    OLED_Refresh();
}

/**
 * 格式化显示字符函数，拼接需要显示的内容等
 */
char *FormatMeasurement(const char *prefix, uint32_t value_mV, const char *unit)
{
    static char measurementString[64]; // 存储测量字符串的缓冲区
    float value_V = value_mV / 1000.0; /* 转换为伏特 */
    snprintf(measurementString, sizeof(measurementString), "%s %.2f %s", prefix, value_V, unit);
    return measurementString;
}

void OLED_CLEAR(void)
{
    OLED_Clear();
}

void OLED_Initialization(void)
{
    OLED_Init();         /* OLED 初始化 */
    OLED_ColorTurn(0);   /* OLED 初始化，是否开启颜色反转显示，默认 0 就好 */
    OLED_DisplayTurn(0); /* OLED 初始化，是否开启反转显示，默认 0 就好 */
}
