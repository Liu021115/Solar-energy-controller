#ifndef _BUTTON_H_
#define _BUTTON_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "multi_button.h"
#include "app.h"

    // extern Button KEY_UP;      /* 初始化按键 KEY_UP 实例 */
    // extern Button KEY_DOWN;    /* 初始化按键 KEY_DOWN 实例 */
    // extern Button KEY_OK;      /* 初始化按键 KEY_OK 实例 */

    #define READ_KEY_UP()    HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin)
    #define READ_KEY_DOWN()    HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin)
    #define READ_KEY_OK()    HAL_GPIO_ReadPin(KEY_OK_GPIO_Port, KEY_OK_Pin)

    #define Increment_Step 100 /* 单击按键电压增加步进值，单位mV */
    #define Decrement_Step 100 /* 单击按键电压减少步进值，单位mV */

    enum Button_IDs
    { /* 按键ID 枚举 */
      KEY_UP_id,
      KEY_DOWN_id,
      KEY_OK_id
    };

    void Button_init(void);                      /* 初始化按键的一些操作，简化主函数任务，增加可读性 */
    uint8_t read_button_GPIO(uint8_t button_id); /* 读取按键IO状态，根据不同的库，修改这个获取GPIO状态的函数即可 */

    void KEY_UP_SINGLE_CLICK_Handler(void *param);     /* KEY_UP 单击事件处理函数 */
    void KEY_UP_DOUBLE_CLICK_Handler(void *param);     /* KEY_UP 双击事件处理函数 */
    void KEY_UP_LONG_PRESS_START_Handler(void *param); /* KEY_UP 长按事件处理函数 */
    void KEY_UP_LONG_PRESS_HOLD_Handler(void *param);  /* KEY_UP 长按不放事件处理函数 */

    void KEY_DOWN_SINGLE_CLICK_Handler(void *param);     /* KEY_DOWN 单击事件处理函数 */
    void KEY_DOWN_DOUBLE_CLICK_Handler(void *param);     /* KEY_DOWN 双击事件处理函数 */
    void KEY_DOWN_LONG_PRESS_START_Handler(void *param); /* KEY_DOWN 长按事件处理函数 */
    void KEY_DOWN_LONG_PRESS_HOLD_Handler(void *param);  /* KEY_DOWN 长按不放事件处理函数 */

    void KEY_OK_SINGLE_CLICK_Handler(void *param);     /* KEY_OK 单击事件处理函数 */
    void KEY_OK_DOUBLE_CLICK_Handler(void *param);     /* KEY_OK 双击事件处理函数 */
    void KEY_OK_LONG_PRESS_START_Handler(void *param); /* KEY_OK 长按事件处理函数 */
    void KEY_OK_LONG_PRESS_HOLD_Handler(void *param);  /* KEY_OK 长按不放事件处理函数 */

#ifdef __cplusplus
}
#endif

#endif
