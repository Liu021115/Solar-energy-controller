/*
 ******************************************************************************
 * File name    : adc_Task.c
 * Description  : adc 滤波，转换为实际值的一些计算任务
 *
 * 内部温度检测  ADC -------> PA0
 * 输出电流检测  ADC -------> PA1
 * 输入电压检测  ADC -------> PA2
 * 输出电压检测  ADC -------> PA3
 *
 ******************************************************************************
 */

#include "adc_Task.h"
#include <math.h>


Adc_IO adc_io;     /* 采样口电压的实例初始化*/
Adc_Real adc_real; /* 实际电压的实例初始化*/

uint32_t ADC_Value[ADC_NUM];               /* ADC转换通道的暂存数组，其中NUM就是暂存数据的数量 */
uint32_t AD_Value_Filter[ADC_Channel_Num]; /* ADC滤波之后的暂存数组，方便以后计算使用 */

static const uint32_t tempRes_buf[121] = 
{
        177000, 166400, 156500, 147200, 138500, 130400, 122900, 115800, 109100, 102900, 
        97120, 91660, 86540, 81720, 77220, 72980, 69000, 65260, 61760, 58460, 
        55340, 52420, 49660, 47080, 44640, 42340, 40160, 38120, 36200, 34380, 
        32660, 31040, 29500, 28060, 26680, 25400, 24180, 23020, 21920, 20880, 
        19900, 18970, 18090, 17260, 16460, 15710, 15000, 14320, 13680, 13070, 
        12490, 11940, 11420, 10920, 10450, 10000, 9574, 9166, 8778, 8408, 
        8058, 7722, 7404, 7098, 6808, 6532, 6268, 6016, 5776, 5546, 5326, 
        5118, 4918, 4726, 4544, 4368, 4202, 4042, 3888, 3742, 3602, 3468, 
        3340, 3216, 3098, 2986, 2878, 2774, 2674, 2580, 2488, 2400, 2316, 
        2234, 2158, 2082, 2012, 1942, 1876, 1813, 1751, 1693, 1637, 1582, 
        1530, 1480, 1432, 1385, 1341, 1298, 1256, 1216, 1178, 1141, 1105, 
        1071, 1038, 1006, 975, 945, 916
};

/**
 * @brief  Process multiple types of data and store the results in an array.
 * @param  ADC_Value: Array containing all the data.
 * @param  numTypes: Number of data types.
 * @param  results: Array to store the results (memory allocated by caller).
 * @retval None
 */
/* ADC滤波函数，输入参数14个值，去掉最大最小各两个，然后剩余10个取平均 */
void processTypes(uint32_t ADC_Value[ADC_NUM], uint32_t numTypes)
{
    // 把需要转换的数量先提取出来
    uint32_t valuesPerType = ADC_NUM / numTypes;
    uint32_t valuesToKeep = valuesPerType - 4;

    for (uint32_t type = 0; type < numTypes; type++)
    {
        uint32_t type_values[valuesPerType];
        uint32_t idx = type;

        // 把各种类型的ad值先分类提取出来，放入type_values
        for (uint32_t i = 0; i < valuesPerType; i++)
        {
            type_values[i] = ADC_Value[idx];
            idx += numTypes;
        }

        for (uint32_t i = 0; i < valuesPerType - 1; i++)
        {
            for (uint32_t j = 0; j < valuesPerType - i - 1; j++)
            {
                if (type_values[j] > type_values[j + 1])
                {
                    uint32_t temp = type_values[j];
                    type_values[j] = type_values[j + 1];
                    type_values[j + 1] = temp;
                }
            }
        }

        // 排序，目前实现是冒泡排序法
        uint32_t min_max_removed[valuesToKeep];
        for (uint32_t i = 2; i < valuesPerType - 2; i++)
        {
            min_max_removed[i - 2] = type_values[i];
        }

        // 剩余的求平均
        uint32_t sum = 0;
        for (uint32_t i = 0; i < valuesToKeep; i++)
        {
            sum += min_max_removed[i];
        }

        // 返回
        AD_Value_Filter[type] = sum / valuesToKeep;
    }
}
/* ADC滤波函数，输入参数14个值，去掉最大最小各两个，然后剩余10个取平均 */

/* ADC转换为实际值函数 */
void ADC_To_Real(void)
{
    float out;
    /* ADC转换为实际采样口当前电压 */
    adc_io.Temp_vol = AD_Value_Filter[0] * AD + AD_Compensate;
    adc_io.Iout_vol = AD_Value_Filter[1] * AD + AD_Compensate;
    adc_io.Vin_vol = AD_Value_Filter[2] * AD + AD_Compensate;
    adc_io.Vout_vol = AD_Value_Filter[3] * AD + AD_Compensate;

    /* 对电流的AD做一些限定，不然容易出现计算问题 */
    out = ((float)(adc_io.Iout_vol - Iref) / Isen) * 1000;
    if ((out < 30) || (out > 26000))
    {
        out = 0;
    }

    /* ADC转换为实际电压 */
    // adc_real.Iout = (int)out ;
    // adc_real.Vin = adc_io.Vin_vol * Vin_Gain;
    // adc_real.Vout = adc_io.Vout_vol * Vout_Gain;
    adc_real.Iout = (int)(out / 100) *100 ;
    adc_real.Vin = (adc_io.Vin_vol * Vin_Gain / 100) * 100;
    adc_real.Vout = (adc_io.Vout_vol * Vout_Gain / 100) * 100;

    if (adc_real.Vin < 2000)
    {
        adc_real.Vin =0;
    }
    if (adc_real.Vout < 2000)
    {
        adc_real.Vout = 0;
    }
    if (adc_real.Iout < 150)
    {
        adc_real.Iout = 0;
    }
    

}
/* ADC转换为实际值函数 */



/* 查表法获取温度函数 */
static int get_Temp(uint32_t Temp_adc_real)
{
	uint32_t temp_vol;
	temp_vol = Temp_adc_real;
	uint32_t R_temp = 0;
	int Temp_value = 0;
	R_temp = temp_vol * R_TEMP_UP / (Vref_3v3 - temp_vol) ;
	if (R_temp <= tempRes_buf[0] && R_temp> tempRes_buf[120])
	{
		for (short i=0; i<121;i++)
		{
			if (R_temp > tempRes_buf[i])
			{
				Temp_value = i - 30;
				break;
			}
			
		}		
	}
	return Temp_value;
	
}

/* ADC转换温度函数 */
void ADC_To_Real_Temp(void)
{
    adc_real.Temp = get_Temp(adc_io.Temp_vol);
}
/* ADC转换温度函数 */

/* 查表法获取温度函数 */
