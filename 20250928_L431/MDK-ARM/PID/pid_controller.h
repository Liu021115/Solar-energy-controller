#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#define HYSTERESIS_UPPER 50 // 迟滞的上限阈值
#define HYSTERESIS_LOWER -50 // 迟滞的下限阈值

#define Voltage_P   0.02f       /* 电压环 P 参数设定 */
#define Voltage_I   0.003f     /* 电压环 I 参数设定 */
#define Voltage_D   0.002f      /* 电压环 D 参数设定 */
#define Voltage_I_Min   1       /* 电压环 最小积分项设定 */
#define Voltage_I_Max   1000     /* 电压环 最大积分项设定 */


#define Current_P   0.02f       /* 电流环 P 参数设定 */
#define Current_I   0.001f     /* 电流环 I 参数设定 */
#define Current_D   0.003f      /* 电流环 D 参数设定 */
#define Current_I_Min   1       /* 电流环 最小积分项设定 */
#define Current_I_Max   300     /* 电流环 最大积分项设定 */

#define Duty_Min    3.0f        /* 最小占空比设定 */
#define Duty_Max    1900.0f      /* 最大占空比设定 */

#define Target_Voltage      12400.0f        /* 额定目标电压值，单位mV */
#define Target_Current      1000.0f         /* 额定目标电流值，单位mA */

#define PI_OUT_MAX 5.0f      //PI计算结果的最大步进值
#define PI_OUT_MIN -5.0f     //PI计算结果的最小步进值


// 定义迟滞状态的枚举
typedef enum {
    HYSTERESIS_INACTIVE, // 未激活状态
    HYSTERESIS_ACTIVE    // 激活状态
} HysteresisState;

// 定义迟滞状态的枚举
typedef enum {
    CONSTANT_VOLTAGE, // 恒压状态
    CONSTANT_CURRENT  // 恒流状态
} BUCKPIDState;


/**
 * @struct PIDController
 * @brief PID控制器结构体
 */
typedef struct {
    float kp_voltage;       /**< 电压控制的比例参数 */
    float ki_voltage;       /**< 电压控制的积分参数 */
    float kd_voltage;       /**< 电压控制的微分参数 */
    float kp_current;       /**< 电流控制的比例参数 */
    float ki_current;       /**< 电流控制的积分参数 */
    float kd_current;       /**< 电流控制的微分参数 */
    float voltage_setpoint; /**< 电压设定值 */
    float current_setpoint; /**< 电流设定值 */
    float voltage_integral; /**< 电压控制的积分项累加值 */
    float current_integral; /**< 电流控制的积分项累加值 */
    float prev_voltage_error;   /**< 上一次电压误差 */
    float prev_current_error;   /**< 上一次电流误差 */
    float outMin;   /**< 输出最小值 */
    float outMax;   /**< 输出最大值 */
    float voltage_integral_min;   /**< 电压积分项最小值 */
    float voltage_integral_max;   /**< 电压积分项最大值 */
    float current_integral_min;   /**< 电流积分项最小值 */
    float current_integral_max;   /**< 电流积分项最大值 */
    HysteresisState hysteresis_state; /**< 迟滞状态 */
    BUCKPIDState pid_state;
} PIDController;

extern PIDController pid;



float PID_Compute(PIDController* pid, float measured_voltage, float measured_current,int currentDuty);
void PID_SetVoltageSetpoint(PIDController* pid, float voltage_setpoint);
void PID_SetCurrentSetpoint(PIDController* pid, float current_setpoint);
void PID_Reset(PIDController* pid);
void VoltagePID_Init(PIDController* pid, float kp_voltage, float ki_voltage, float kd_voltage);
void CurrentPID_Init(PIDController* pid, float kp_current, float ki_current, float kd_current);
void Hysteresis_Init(PIDController* pid);
void PID_SetOutputLimits(PIDController* pid, float outMin, float outMax);
void PID_SetVoltageIntegralLimits(PIDController* pid, float voltage_integral_min, float voltage_integral_max);
void PID_SetCurrentIntegralLimits(PIDController* pid, float current_integral_min, float current_integral_max);
void PID_State_Init(PIDController* pid);
void PID_Init(void);

#endif /* PID_CONTROLLER_H */
