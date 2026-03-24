/*
 ******************************************************************************
 * File name    : ButtonTask.c
 * Description  : 按键相关任务功能实现
 * 
 * 按键功能描述，及硬件连接
 * KEY1 目前实现功能------> PB0
 * 单击：增加目标电压/电流设定值
 * 长按：持续增加目标电压/电流设定值
 * 
 * KEY2 目前实现功能------> PB1
 * 单击：增加目标电压/电流设定值
 * 长按：持续目标电压/电流设定值
 * 
 * KEY3 目前实现功能------> PB2
 * 单击：切换 OLED 屏幕页面显示内容
 * 双击：切换设定目标是电压/电流
 * 
 ******************************************************************************
 */

#include "./ButtonTask.h"
#include "app.h"
#include "gpio.h"


struct Button KEY_UP;      /* 初始化按键 KEY_UP 实例 */ 
struct Button KEY_DOWN;    /* 初始化按键 KEY_DOWN 实例 */ 
struct Button KEY_OK;      /* 初始化按键 KEY_OK 实例 */ 



/**
 * 初始化按键的一些操作，简化主函数任务，增加可读性
 */
void Button_init(void)
{
    button_init(&KEY_UP,read_button_GPIO,1,KEY_UP_id);                          //初始化KEY_UP
    button_attach(&KEY_UP,SINGLE_CLICK,KEY_UP_SINGLE_CLICK_Handler);            //绑定KEY_UP的单击事件
    button_attach(&KEY_UP,DOUBLE_CLICK,KEY_UP_DOUBLE_CLICK_Handler);            //绑定KEY_UP 双击事件
    button_attach(&KEY_UP,LONG_PRESS_START,KEY_UP_LONG_PRESS_START_Handler);    //绑定KEY_UP 长按事件
    button_attach(&KEY_UP,LONG_PRESS_HOLD,KEY_UP_LONG_PRESS_HOLD_Handler);      //绑定KEY_UP 持续按住事件
    button_start(&KEY_UP);                                                      //启动KEY_UP任务

    button_init(&KEY_DOWN,read_button_GPIO,1,KEY_DOWN_id);                      //初始化KEY_DOWN
    button_attach(&KEY_DOWN,SINGLE_CLICK,KEY_DOWN_SINGLE_CLICK_Handler);        //绑定KEY_DOWN的单击事件
    button_attach(&KEY_DOWN,DOUBLE_CLICK,KEY_DOWN_DOUBLE_CLICK_Handler);        //绑定KEY_DOWN 双击事件
    button_attach(&KEY_DOWN,LONG_PRESS_START,KEY_DOWN_LONG_PRESS_START_Handler);//绑定KEY_DOWN 长按事件
    button_attach(&KEY_DOWN,LONG_PRESS_HOLD,KEY_DOWN_LONG_PRESS_HOLD_Handler);  //绑定KEY_DOWN 持续按住事件
    button_start(&KEY_DOWN);                                                    //启动KEY_DOWN任务

    button_init(&KEY_OK,read_button_GPIO,1,KEY_OK_id);                          //初始化KEY_OK
    button_attach(&KEY_OK,SINGLE_CLICK,KEY_OK_SINGLE_CLICK_Handler);            //绑定KEY_OK的单击事件
    button_attach(&KEY_OK,DOUBLE_CLICK,KEY_OK_DOUBLE_CLICK_Handler);            //绑定KEY_OK 双击事件
    button_attach(&KEY_OK,LONG_PRESS_START,KEY_OK_LONG_PRESS_START_Handler);    //绑定KEY_OK 长按事件
    // button_attach(&KEY_OK,LONG_PRESS_HOLD,KEY_OK_LONG_PRESS_HOLD_Handler);      //绑定KEY_OK 持续按住事件
    button_start(&KEY_OK);                                                      //启动KEY_OK任务

}


/**
 * 读取按键GPIO的状态
 * @param button_id 需要读取状态的KEI ID
 */
uint8_t read_button_GPIO(uint8_t button_id)
{
	// you can share the GPIO read function with multiple Buttons
	switch(button_id)
	{
		case KEY_UP_id:
			return READ_KEY_UP();
		case KEY_DOWN_id:
			return READ_KEY_DOWN();
        case KEY_OK_id:
           return  READ_KEY_OK();
		default:
			return 0;
	}
}

/**
 * KEY_UP按键的单击回调函数
 */
void KEY_UP_SINGLE_CLICK_Handler(void *param)
{
    /* KEY_UP 单击事件函数 */
    /* 判定当前属于什么的设置模式，增加相对应模式的设定值 */
    switch (set_mode)
    {
    case SET_MODE_VOLTAGE:
        pid.voltage_setpoint += Increment_Step;
        break;
    case SET_MODE_CURRENT:
        pid.current_setpoint += Increment_Step;
        break;
    default:
        break;
    }
    

}

/**
 * KEY_UP按键的双击回调函数
 */
void KEY_UP_DOUBLE_CLICK_Handler(void *param)
{
    /* KEY_UP 双击事件函数 */
    
}

/**
 * KEY_UP按键的长按回调函数
 */
void KEY_UP_LONG_PRESS_START_Handler(void *param)
{
    /* KEY_UP 长按事件函数 */

}

/**
 * KEY_UP按键的按住回调函数
 */
void KEY_UP_LONG_PRESS_HOLD_Handler(void *param)
{
    /* KEY_UP 按键的按住回调函数 */
    /* 判定当前属于什么的设置模式，增加相对应模式的设定值 */
    // static int i = 0;   //i 在这里是为了减缓数据增加的速度，经测试，50比较适合
    // i += 1;
    // if (i > 200)
    // {

    // }
    switch (set_mode)
    {
        case SET_MODE_VOLTAGE:
            pid.voltage_setpoint += 1;
            break;
        case SET_MODE_CURRENT:
            pid.current_setpoint += 1;
            break;
        default:
            break;
    }
    
    

}

/**
 * KEY_DOWN按键的单击回调函数
 */
void KEY_DOWN_SINGLE_CLICK_Handler(void *param)
{
    /* KEY_DOWN 单击事件函数 */
    switch (set_mode)
    {
        case SET_MODE_VOLTAGE:
            pid.voltage_setpoint -= Decrement_Step;
            break;
        case SET_MODE_CURRENT:
            pid.current_setpoint -= Decrement_Step;
            break;
        default:
            break;
    }
}

/**
 * KEY_DOWN按键的双击回调函数
 */
void KEY_DOWN_DOUBLE_CLICK_Handler(void *param)
{
    /* KEY_DOWN 双击事件函数 */
    
}

/**
 * KEY_DOWN按键的长按回调函数
 */
void KEY_DOWN_LONG_PRESS_START_Handler(void *param)
{
    /* KEY_DOWN 长按事件函数 */

}

/**
 * KEY_DOWN 按键的按住回调函数
 */
void KEY_DOWN_LONG_PRESS_HOLD_Handler(void *param)
{
    /* KEY_DOWN 按键的按住回调函数 */
    /* 判定当前属于什么的设置模式，减少相对应模式的设定值 */
    switch (set_mode)
    {
        case SET_MODE_VOLTAGE:
            pid.voltage_setpoint -= 1;
            break;
        case SET_MODE_CURRENT:
            pid.current_setpoint -= 1;
            break;
        default:
            break;
    }
    

}

/**
 * KEY_OK按键的单击回调函数
 */
void KEY_OK_SINGLE_CLICK_Handler(void *param)
{
    /* KEY_OK 单击事件函数 */
    /* 切换OLED 显示页面的内容，每次切换内容的时候，清一下屏幕，防止有缓存的数据 */
    OLED_CLEAR();


    switch (oled_page)
    {
    case PAGE_RUN:
        oled_page = PAGE_SETTING;
        break;
    case PAGE_SETTING:
        oled_page = PAGE_OTHER;
        break;
    case PAGE_OTHER:
        oled_page = PAGE_RUN;
        break;
    default:
        OLED_CLEAR();
        oled_page = PAGE_RUN;
        break;
    }


}

/**
 * KEY_OK按键的双击回调函数
 */
void KEY_OK_DOUBLE_CLICK_Handler(void *param)
{
    /* KEY_OK 双击事件函数 */
    /* 切换设定模式 */
    switch (set_mode)
    {
    case SET_MODE_VOLTAGE:
        set_mode = SET_MODE_CURRENT;
        break;
    case SET_MODE_CURRENT:
        set_mode = SET_MODE_VOLTAGE;
        break;
    default:
        break;
    }
    
}

/**
 * KEY_OK按键的长按回调函数
 */
void KEY_OK_LONG_PRESS_START_Handler(void *param)
{
    /* KEY_OK 长按事件函数 */

    
}

/**
 * KEY_OK 按键的按住回调函数
 */
void KEY_OK_LONG_PRESS_HOLD_Handler(void *param)
{
    /* KEY_OK 按键的按住回调函数 */

}




