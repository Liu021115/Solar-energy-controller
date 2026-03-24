

#ifndef __ADC_TASK_H
#define __ADC_TASK_H


#ifdef __cplusplus
 extern "C" {
#endif

#include  "main.h"


#define ADC_Channel_Num 4            /* adc??换通道??*/
#define ADC_NUM ADC_Channel_Num * 14 /* adc??换数??*/
#define AD 0.805                     /* 3300/4096的值，加快后面计算速度*/
#define AD_Compensate 56             /* AD??换补偿值，补偿和实际的相差??*/
#define Iref 1660                    /* 电流基准??*/
#define Isen 110//52                      /* 电流传感器芯片的灵敏??*/
#define Vin_Gain 30                  /* 输入电压计算增益*/
#define Vout_Gain 25                 /* 输出电压计算增益*/
#define R_TEMP_UP 16000              /* ??敏电阻分压电阻值，单位：??*/
#define Vref_3v3 3300                /* 系统供电电压，单位：mV*/

/* ADC采样?? 结构?? */
typedef struct
{
    // 单片机采样口的实际电压??
    uint32_t Vin_vol;
    uint32_t Vout_vol;
    uint32_t Iout_vol;
    uint32_t Temp_vol;
} Adc_IO;
/* ADC采样?? 结构?? */

/* adc实际值结构体 */
typedef struct
{
    // 实际的电压??
    uint32_t Vin;
    uint32_t Vout;
    uint32_t Iout;
    int Temp;
} Adc_Real;
/* adc实际值结构体 */

extern Adc_IO adc_io;                             /* 采样口电压的实例初始化*/
extern Adc_Real adc_real;                         /* 实际电压的实例初始化*/
extern uint32_t ADC_Value[ADC_NUM];               /* ADC??换通道的暂存数组，其中NUM就是暂存数据的数?? */
extern uint32_t AD_Value_Filter[ADC_Channel_Num]; /* ADC滤波之后的暂存数组，方便以后计算使用 */

void processTypes(uint32_t ADC_Value[ADC_NUM], uint32_t numTypes); /* ADC 采样滤波函数 */
void ADC_To_Real(void);                                            /* ADC 实际值转换函?? */
void ADC_To_Real_Temp(void);                                       /* ADC ??换温度函?? */




#ifdef __cplusplus
}
#endif


#endif
