#include "mppt_controller.h"


uint32_t mppt_adj = 0;

MPPTInfo MPPT;


void mppt_Init(void)
{
    MPPT.mppt_count = 0;
    MPPT.initialized = 0;
}


int MPPT_Compute(MPPTInfo *mppt_info, uint32_t pv_voltage, uint32_t output_voltage, uint32_t output_current, uint32_t dutycycle)
{
    uint32_t duty = dutycycle;
    uint32_t PV_First_Target = mppt_info->pv_open_voltage * 0.8;   // 第一个目标电压就是开路电压乘以0.8
    static uint16_t adj_count = 0;

    if (mppt_info->mppt_count < 2000)  //这2s是 MPPT 先执行一开始的追踪目标电压，先追踪到最大的目标电压附近，再进行下一步动态调整
    {
        mppt_info->current_pv_target_voltage = PV_First_Target;							

        
        if( pv_voltage > mppt_info->current_pv_target_voltage)//在曲线右边，增加占空比，往左边走
        {
            duty += 1;
        }
        else if(pv_voltage < mppt_info->current_pv_target_voltage)//在曲线左边，减小占空比，往右边走
        {
            duty -= 1;
        }
        if (mppt_info->mppt_count == 1999)
        {
            mppt_info->current_power = pv_voltage/100 * output_current/10;  //再最后的循环，把当前功率和上一个功率赋给结构体
            mppt_info->prev_power = mppt_info->current_power - 100;         //随便给定一个方向
            mppt_info->prev_pv_target_voltage = mppt_info->current_pv_target_voltage + 100;    //随便给一个目标电压，给定初始方向
        }
		
		mppt_info->mppt_count++;
    }
    else
    {
        adj_count++;    
        if (adj_count > 10) //进入动态调整阶段之后，每个10ms才调整一次占空比，不然太快导致太乱
        {
            adj_count = 0;
            if( pv_voltage > mppt_info->current_pv_target_voltage)// 增大占空比可以拉低 PV 电压
            {
                duty += 1;
            }
            else if(pv_voltage < mppt_info->current_pv_target_voltage) // 减小占空比可以拉高 PV 电压
            {
                duty -= 1;
            }
        }
        
        mppt_adj++;

        if (mppt_adj>100)   //100ms调整一次目标电压，根据当前功率和上一次的功率，调整当前的目标电压
        {
            int mppt_random_mode = 50;     //扰动的时候，电压的步进值
            uint32_t pre_target_temp = 0;   // 功率的临时值存放
            mppt_info->current_power = pv_voltage/100 * output_current/10;  //当前功率的计算得出
            pre_target_temp = mppt_info->current_pv_target_voltage;         //上一次功率的结果临时保存

            if (mppt_info->current_power > mppt_info->prev_power)   //如果当前功率大于上一次的功率
            {
                // 从左边往中间爬
                if (mppt_info->current_pv_target_voltage > mppt_info->prev_pv_target_voltage)
                {
                    mppt_info->current_pv_target_voltage += 100;
                }
                // 从右边往中间爬
                else if (mppt_info->current_pv_target_voltage < mppt_info->prev_pv_target_voltage)
                {
                    mppt_info->current_pv_target_voltage -= 300;
                }
            }
            else if (mppt_info->current_power < mppt_info->prev_power)  //如果当前功率小于上一次的功率
            {
                // 追过右边了
                if (mppt_info->current_pv_target_voltage > mppt_info->prev_pv_target_voltage)
                {
                    mppt_info->current_pv_target_voltage -= 300;
                }
                // 追过左边了
                else if (mppt_info->current_pv_target_voltage < mppt_info->prev_pv_target_voltage)
                {
                    mppt_info->current_pv_target_voltage += 100;
                }
            }
            else   //这里就应该只有相等的情况了，相等的话就随机扰动，比如这一次相等就加目标电压，下一次就会是减去目标电压     
            {
                mppt_info->current_pv_target_voltage += mppt_random_mode;
                mppt_random_mode = mppt_random_mode * -1;
            }

            //这里判断电源模式的时候，如果目标电压已经大于开路电压了，就要往回走走，目标电压不可能比输入的开路电压还要高
            if (mppt_info->current_pv_target_voltage > mppt_info->pv_open_voltage - 100)
            {
                mppt_info->current_pv_target_voltage -= 300;
            }

            // 这里判断，追太过左边的情况，就是PV电压拉得比较低得时候，因为Vmp最大的电压一般在开路电压的0.8附近，所以暂设0.6倍的开路电压
            if (mppt_info->current_pv_target_voltage < mppt_info->pv_open_voltage * 6/10)
            {
                mppt_info->current_pv_target_voltage = mppt_info->pv_open_voltage * 8/10;   //就直接强等于0.8倍的开路电压
            }

            mppt_info->prev_pv_target_voltage = pre_target_temp;    //把当前的功率提供给上一次功率
            mppt_info->prev_power = mppt_info->current_power;       //把当前的功率提供给上一次功率

            mppt_adj = 0; 

        }     
    }
    
    if (duty > 1800) //先简单限幅一下
    {
        duty = 1800;
    }     
    return duty;
}


