	/*
	 ******************************************************************************
	 * File name    : check_staus_Task.c
	 * Description  :检查每个任务切换的条件，便于MPPT、CC、CV之前的切换
	 *
	 *
	 ******************************************************************************
	 */

	#include "check_staus_Task.h"
	#include "tim.h"

	uint8_t pid_count = 0;

	void check_staus(void)
	{

	#if ENABLE_STOP_DETECTION  
			static uint8_t count_stop = 0;
			/* 停止充电条件 */
			if (adc_real.Vin < 15000)
			{
					count_stop++;
					if (count_stop > 200)
					{
							PWM_Stop();
							buckState = BUCK_STATE_STOP;
							MPPT.initialized = 0;
					}
			}
			else
			{
					count_stop = 0;
			}    
	#endif

	#if ENABLE_CHARGING_DETECTION
			
			/* 开始充电条件 */
			if ((adc_real.Vin > 17000) && (buckState == BUCK_STATE_STOP) && (SYSTEM_STATUS == OK))
			{
					static int i = 0;
					i++;
					if (i > 2000 && adc_real.Vout > 8000) /* 累计1s，，才能确定充电状态，增加判断，如果存在电池，就直接进入 MPPT 状态*/
					{
							buckState = BUCK_STATE_MPPT;
							MPPT.pv_open_voltage = adc_real.Vin;
							MPPT.initialized = 1;
							i = 0;
					}
					else if (i > 100 && adc_real.Vout < 5000) /* 累计1s，，才能确定充电状态，增加判断，如果不存在电池，输出电压小于5V，就直接进入 PID 状态*/
					{
							buckState = BUCK_STATE_SOFT_START;
					}
			}
			
	#endif

	#if ENABLE_CC_CV_DETECTION
			/* 切换为CC，CV条件,不在PID阶段才考虑是否接入PID */
			if ((buckState != BUCK_STATE_PID_ADJUSTMENT) && (SYSTEM_STATUS == OK) && buckState != BUCK_STATE_STOP)
			{
					if ((adc_real.Vout > pid.voltage_setpoint) || (adc_real.Iout > pid.current_setpoint)) // 电压超过目标电压，或者电流超过目标电流
					{
							pid_count++;
							if (pid_count > 5) /* 累计 5 ms才确定转换状态 */
							{
									buckState = BUCK_STATE_PID_ADJUSTMENT;
									MPPT.initialized = 0;
									pid_count = 0;
							}
					}
					else // 记得清零这个数据，不然，采样误差就会进入PID调整
					{
							pid_count = 0;
							TIM1->CCR2 += 0;
					}
			}
	#endif

	#if ENABLE_PV_MOS_DETECTION
			static uint8_t count_mos = 0;
			/* PV防反MOS开启关闭条件 与 下管的开启关闭条件，都是和充电电流有关，电流比较小，不开下管*/
			if (adc_real.Iout > OPEN_PV_MOS_CURRENT)
			{

					count_mos++;
					if (count_mos > 100)
					{
							PV_MOS_OPEN;
							HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2); /* 配置PWM，启动下管 */
					}
			}
			else
			{
					count_mos = 0;
					PV_MOS_CLOSE;
					HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2); /* 关闭下管 PWM */
			}
	#endif

	#if ENABLE_BAT_OFF_DETECTION
			/* 防止电池突然拔掉出现的超压，这个会与确定充电电压冲突，因为关掉之后，输入电压还在，同时状态又在停止状态，就会触发上面可以充电的状态*/
			if (adc_real.Vout > pid.voltage_setpoint + OV_PROTECT_VOLTAGE)
			{
					PWM_Stop(); // 要首先关掉管子，不然等下一个周期来临，已经迟了
					buckState = BUCK_STATE_STOP;
			}
	#endif

	#if ENABLE_UNNORMAL_DETECTION
			
			static uint16_t count_unnormal = 0;
			// 主要来处理一些不正常的条件，例如达到最大的占空比，电池电压也没到目标电压，电流也没有到目标电流
			if (buckState == BUCK_STATE_MPPT || buckState == BUCK_STATE_PID_ADJUSTMENT)
			{
					if ((TIM1->CCR2 == Duty_Max) || (TIM1->CCR2 == Duty_Min))
					{
							count_unnormal++;
							if (count_unnormal > 500)
							{
									if ((adc_real.Vout < pid.voltage_setpoint) || (adc_real.Iout < pid.current_setpoint))
									{
											PWM_Stop(); // 要首先关掉管子，不然等下一个周期来临，已经迟了
											buckState = BUCK_STATE_STOP;
									}
							}
					}
					else
					{
							count_unnormal = 0;
					}
			}
			
	#endif
	}
