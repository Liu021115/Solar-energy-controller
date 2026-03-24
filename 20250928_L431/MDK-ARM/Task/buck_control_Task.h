#ifndef __BUCK_CONTROL_TASK_H
#define __BUCK_CONTROL_TASK_H

#ifdef __cplusplus
extern "C" {
#endif



/* Buck??换器的状态枚?? */
typedef enum {
    BUCK_STATE_SOFT_START,          // ??起阶??
    BUCK_STATE_PID_ADJUSTMENT,      // PID调整阶???，会由CC切换到CV
    BUCK_STATE_PROTECTION,          // 保护状??
    BUCK_STATE_STOP,                // 停???状??
    BUCK_STATE_MPPT,                // MPPT状??
} BuckState;
/* Buck??换器的状态枚?? */


extern BuckState buckState;



void softStartBuck(float inputVoltage, float targetVoltage, float softStartTime);   /* ??起动 函数 */
void controlBuckConverter(void);    /* BUCK 状态???理函数 */
void PWM_Start(void);       /* 开??系统的PWM */
void PWM_Stop(void);        /* 关闭系统的PWM */



#ifdef __cplusplus
}
#endif

#endif 

