#ifndef __OLED_TASK_H
#define __OLED_TASK_H

#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum
    {
        PAGE_RUN,     // 运???界??
        PAGE_SETTING, // 设置界面
        PAGE_OTHER,   // 其他界面
        PAGE_DEBUG,   // 调试界面，用于调试打??
    } OLEDPageState;

    typedef enum
    {
        SET_MODE_VOLTAGE, /* ??标电压???置模式 */
        SET_MODE_CURRENT, /* ??标电流???置模式 */
    } SetMode;

    extern OLEDPageState oled_page;
    extern SetMode set_mode;

    void OLED_Initialization(void);
    void OLED_CLEAR(void);
    void UpdateOLEDDisplay(void);                                                  /* 更新 OLED 屏幕内???函?? */
    void Page_Run_Content(void);                                                   /* OLED 运???参数界面显示内?? */
    void Page_Setting_Content(void);                                               /* OLED 设置参数界面显示内??? */
    void Page_Other_Content(void);                                                 /* OLED 其他信息参数界面显示内??? */
    void Page_Debug_Content(void);                                                 /* OLED 调试界面显示内??? */
    char *FormatMeasurement(const char *prefix, uint32_t value, const char *unit); /* 格式化输出内容函?? */

#ifdef __cplusplus
}
#endif

#endif
