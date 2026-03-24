#include "pid_controller.h"
#include "math.h"


PIDController pid;

/**
 * @brief 设置电压设定值
 * @param pid PID控制器实例
 * @param voltage_setpoint 电压设定值
 */
void PID_SetVoltageSetpoint(PIDController* pid, float voltage_setpoint)
{
    pid->voltage_setpoint = voltage_setpoint;
}

/**
 * @brief 设置电流设定值
 * @param pid PID控制器实例
 * @param current_setpoint 电流设定值
 */
void PID_SetCurrentSetpoint(PIDController* pid, float current_setpoint)
{
    pid->current_setpoint = current_setpoint;
}


/**
 * @brief 电压环的初始化
 * @param pid PID控制器实例
 * @param kp_voltage 电压控制的比例参数
 * @param ki_voltage 电压控制的积分参数
 * @param kd_voltage 电压控制的微分参数
 */
void VoltagePID_Init(PIDController* pid, float kp_voltage, float ki_voltage, float kd_voltage)
{
    pid->kp_voltage = kp_voltage;
    pid->ki_voltage = ki_voltage;
    pid->kd_voltage = kd_voltage;
    pid->voltage_setpoint = 0.0f;
    pid->voltage_integral = 0.0f;
    pid->prev_voltage_error = 0.0f;
}

/**
 * @brief 电流环的初始化
 * @param pid PID控制器实例
 * @param kp_current 电流控制的比例参数
 * @param ki_current 电流控制的积分参数
 * @param kd_current 电流控制的微分参数
 */
void CurrentPID_Init(PIDController* pid, float kp_current, float ki_current, float kd_current)
{
    pid->kp_current = kp_current;
    pid->ki_current = ki_current;
    pid->kd_current = kd_current;
    pid->current_setpoint = 0.0f;
    pid->current_integral = 0.0f;
    pid->prev_current_error = 0.0f;
}


/**
 * @brief 设置PID控制器的输出限制范围
 * @param pid PID控制器实例
 * @param outMin 输出最小值
 * @param outMax 输出最大值
 */
void PID_SetOutputLimits(PIDController* pid, float outMin, float outMax)
{
    pid->outMin = outMin;
    pid->outMax = outMax;
}

/**
 * @brief 初始化迟滞状态
 * @param pid PID控制器实例
 */
void Hysteresis_Init(PIDController* pid)
{
    pid->hysteresis_state = HYSTERESIS_INACTIVE;
}

void PID_State_Init(PIDController* pid)
{
    pid->pid_state = CONSTANT_VOLTAGE;
}


/**
 * @brief 使用PID控制器计算占空比
 * @param pid PID控制器实例
 * @param measured_voltage 实际测量的电压值
 * @param measured_current 实际测量的电流值
 * @param measured_current 实际测量的当前占空比
 * @return 计算得到的占空比
 */
float PID_Compute(PIDController* pid, float measured_voltage, float measured_current,int currentDuty)
{
	float voltage_output, current_output;
	
    float voltage_error = pid->voltage_setpoint - measured_voltage;      // 电压误差

    // 计算电压控制的比例项
    float pTerm_voltage = pid->kp_voltage * voltage_error;

    // 计算电压控制的积分项
    pid->voltage_integral += voltage_error;

    // 限制电压环积分项的范围
    if (pid->voltage_integral > pid->voltage_integral_max) 
    {
        // pid->voltage_integral = pid->voltage_integral_max;
        pid->voltage_integral = 0;              /* 2023年10月12日更改，因为发现不能一直把积分项限制在最大，这样虽然也能运行，但是直觉不好，所以在最大的时候应该清零 */
    } else if (pid->voltage_integral < pid->voltage_integral_min) 
    {
        pid->voltage_integral = pid->voltage_integral_min;
    }

    float iTerm_voltage = pid->ki_voltage * pid->voltage_integral;


    // 计算电压控制的微分项
    float dTerm_voltage = pid->kd_voltage * (voltage_error - pid->prev_voltage_error);
    pid->prev_voltage_error = voltage_error;

    
	
	
	// 计算电流误差
	float current_error = (pid->current_setpoint - measured_current);      

    // 计算电流控制的比例项
    float pTerm_current = pid->kp_current * current_error;

    // 计算电流控制的积分项
    pid->current_integral += current_error;

    // 限制电流环积分项的范围
    if (pid->current_integral > pid->current_integral_max) 
    {
        // pid->current_integral = pid->current_integral_max;
        pid->current_integral = 0;     /* 同电流环积分项 */
    } else if (pid->current_integral < pid->current_integral_min) 
    {
        pid->current_integral = pid->current_integral_min;
    }

    float iTerm_current = pid->ki_current * pid->current_integral;


    // 计算电流控制的微分项
    float dTerm_current = pid->kd_current * (current_error - pid->prev_current_error);
    pid->prev_current_error = current_error;


    if (pid->pid_state == CONSTANT_CURRENT)
    {
        /* 在电流环控制下，要将电压环的积分项及时清零，不然电压环的积分项会一直累加 */
        // pid->ki_voltage = 0;
        // pid->kd_voltage = 0;
        pid->voltage_integral = 0;
        pTerm_voltage = 0;
        iTerm_voltage = 0;
        dTerm_voltage = 0;
    }
    else if(pid->pid_state == CONSTANT_VOLTAGE)
    {
        /* 当切换回电压环的时候，要重新赋值电压环的控制参数，这样电压环控制才生效 */
        // pid->kd_voltage = 0.002;
        // pid->kd_voltage = 0.0001;
        pid->current_integral = 0;
        pTerm_current = 0;
        iTerm_current = 0;
        dTerm_current = 0;
        
    }



    // 计算电压输出和电流输出
    voltage_output = pTerm_voltage + iTerm_voltage + dTerm_voltage;
    current_output = pTerm_current + iTerm_current + dTerm_current;
    
    //输出限幅，保证每次增加的步进值都在合理范围内
    current_output = fmaxf(PI_OUT_MIN, fminf(PI_OUT_MAX, current_output ));
	  voltage_output = fmaxf(PI_OUT_MIN, fminf(PI_OUT_MAX, voltage_output ));

    voltage_output += currentDuty;
    current_output += currentDuty;



    // 输出限制：确保电压输出（voltage_output）在指定范围内
    voltage_output =    (voltage_output > pid->outMax) ? pid->outMax :      // 如果电压输出大于最大值，将其截断为最大值
                        (voltage_output < pid->outMin) ? pid->outMin :      // 如果电压输出小于最小值，将其截断为最小值
                        voltage_output;                                     // 如果电压输出在范围内，保持不变

    // 输出限制：确保电流输出（current_output）在指定范围内
    current_output =    (current_output > pid->outMax) ? pid->outMax :      // 如果电流输出大于最大值，将其截断为最大值
                        (current_output < pid->outMin) ? pid->outMin :      // 如果电流输出小于最小值，将其截断为最小值
                        current_output;                                     // 如果电流输出在范围内，保持不变


	
  

    // 如果测量电流接近或高于设定点 (允许一些偏差)
    if ((pid->pid_state == CONSTANT_VOLTAGE) && (measured_current >= pid->current_setpoint + 100))
    {
        // 进入恒流模式
        pid->pid_state = CONSTANT_CURRENT;
    }    
    else if ((measured_voltage >= pid->voltage_setpoint + 100) || (measured_current <= pid->current_setpoint - 300) )
    {
        pid->pid_state = CONSTANT_VOLTAGE;
    }

    if (pid->pid_state == CONSTANT_CURRENT)
    {
        // 如果在恒流状态下，使用电流控制
        return current_output;
    }
    else
    {
        // 否则，返回电压控制
        return voltage_output;
    }
	
}

/**
 * @brief 设置电压积分项最小值和最大值
 * @param pid PID控制器实例
 * @param voltage_integral_min 电压积分项最小值
 * @param voltage_integral_max 电压积分项最大值
 */
void PID_SetVoltageIntegralLimits(PIDController* pid, float voltage_integral_min, float voltage_integral_max)
{
    /* 最小值不大于最大值 */
    if (voltage_integral_max >= voltage_integral_min) 
    {
        pid->voltage_integral_min = voltage_integral_min;
        pid->voltage_integral_max = voltage_integral_max;
    }
}

/**
 * @brief 设置电流积分项最小值和最大值
 * @param pid PID控制器实例
 * @param current_integral_min 电流积分项最小值
 * @param current_integral_max 电流积分项最大值
 */
void PID_SetCurrentIntegralLimits(PIDController* pid, float current_integral_min, float current_integral_max)
{
    /* 最小值不大于最大值 */
    if (current_integral_max >= current_integral_min) 
    {
        pid->current_integral_min = current_integral_min;
        pid->current_integral_max = current_integral_max;
    }
}


/**
 * @brief 重置PID控制器
 * @param pid PID控制器实例
 */
void PID_Reset(PIDController* pid)
{
    pid->voltage_integral = 0.0f;
    pid->current_integral = 0.0f;
    pid->prev_voltage_error = 0.0f;
    pid->prev_current_error = 0.0f;
}

void PID_Init(void)
{
    VoltagePID_Init(&pid, Voltage_P, Voltage_I, Voltage_D);           /* 电压环参数初始化 */
    CurrentPID_Init(&pid, Current_P, Current_I, Current_D);           /* 电流环参数初始化 */
    PID_SetOutputLimits(&pid, Duty_Min, Duty_Max);                    /* PID输出最大最小值限制 */
    Hysteresis_Init(&pid);                                            /* PID 迟滞状态 初始化 */
    PID_State_Init(&pid);                                             /* PID 状态初始化 */
    PID_SetVoltageSetpoint(&pid, Target_Voltage);                     /* PID 设定目标电压值，单位mV */
    PID_SetCurrentSetpoint(&pid, Target_Current);                     /* PID 设定目标电压值，单位mA */
    PID_SetVoltageIntegralLimits(&pid, Voltage_I_Min, Voltage_I_Max); /* 电压环积分项最小最大值限制设定 */
    PID_SetCurrentIntegralLimits(&pid, Current_I_Min, Current_I_Max); /* 电流环积分项最小最大值限制设定 */
}


