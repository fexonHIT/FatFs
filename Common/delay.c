#include "delay.h"
/*
Author:HIT_XiaoWu
Date:2016/10/16/16:14
Desctription：对延时函数进行初始化
Note:
The processor has a 24-bit system timer, SysTick, that counts down from the reload value to 
zero, reloads (wraps to) the value in the STK_LOAD register on the next clock edge, then 
counts down on subsequent clocks.
ref: STM32F4x-cm4内核参考手册 P230
*/
#define SYSTICK_MAX_LOAD  0xFFFFFF
uint16_t fac_us=0;
void  delay_init(void){
		  /*关闭滴答定时器*/
	  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	    /*配置系统的滴答定时器为AHB AHB/8 0 AHB 1*/
	  SysTick->CTRL|=SysTick_CTRL_CLKSOURCE_Msk;    //配置系统的时钟频率为180MHz
	    /*关闭滴答定时器的异常*/
	  SysTick->CTRL&=~SysTick_CTRL_TICKINT_Msk;
	  SysTick->LOAD=(uint32_t)SYSTICK_MAX_LOAD;    /*为LOAD寄存器装在最大值2^24-1*/
	  fac_us=SystemCoreClock/(1e6);
}
/*
Author:HIT_XiaoWu
Date:2016/10/16/16:14
Desctription：微妙级别的延时函数
*/

void delay_us(uint32_t us){
	   SysTick->LOAD=(uint32_t)SYSTICK_MAX_LOAD;    /*为LOAD寄存器装在最大值2^24-1*/
	   /*进入函数的初始Ticks*/
	   uint32_t ticksOld=SysTick->VAL;
	   /*开启滴答定时器*/
	   SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	   /*计算需要延时的Ticks数*/
	   uint32_t ticksTotal=us*fac_us;
		 /*初始状态Systick的VAL值*/
	   uint32_t ticksCnt=0,ticksNow=0;
	   while(ticksCnt<ticksTotal){
				   ticksNow=SysTick->VAL;
           if(ticksNow<=ticksOld)  ticksCnt +=ticksOld-ticksNow;	
           else                    ticksCnt +=SYSTICK_MAX_LOAD-ticksNow+ticksOld;
			     ticksOld=ticksNow;
		 }
		 /*关闭滴答定时器*/
		 SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
}
/*
Author:HIT_XiaoWu
Date:2016/10/16/16:14
Desctription：ms级别的延时
*/
void delay_ms(uint32_t ms){
		 delay_us(1000*ms);              /*精确延时函数*/
}
