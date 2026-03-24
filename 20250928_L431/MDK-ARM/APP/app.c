/*
 ******************************************************************************
 * File name    : app.c
 * Description  : 
 ******************************************************************************
 */

#include "app.h"


// ProtectionState protect_state = SYSTEM_NORMAL;      /* 系统保护状态初始化 */



int test_pid = 1;
int test_pid_v = 0;
int test_pid_i = 0;

uint32_t g_Systime = 0;
int test_spi = 0;

uint32_t test_key_num = 0;

//uint8_t protect_state = SYSTEM_NORMAL;







#if 0  // test code
#if defined (__CC_ARM)

int32_t $sub$$main(void) 
{
	bsp_common_init();
    bsp_sample_init();
    while(1)
    {
        bsp_delay_ms(10);
    }
}

#endif
#endif

