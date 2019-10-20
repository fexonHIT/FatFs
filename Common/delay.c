#include "delay.h"
/*
Author:HIT_XiaoWu
Date:2016/10/16/16:14
Desctription������ʱ�������г�ʼ��
Note:
The processor has a 24-bit system timer, SysTick, that counts down from the reload value to 
zero, reloads (wraps to) the value in the STK_LOAD register on the next clock edge, then 
counts down on subsequent clocks.
ref: STM32F4x-cm4�ں˲ο��ֲ� P230
*/
#define SYSTICK_MAX_LOAD  0xFFFFFF
uint16_t fac_us=0;
void  delay_init(void){
		  /*�رյδ�ʱ��*/
	  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	    /*����ϵͳ�ĵδ�ʱ��ΪAHB AHB/8 0 AHB 1*/
	  SysTick->CTRL|=SysTick_CTRL_CLKSOURCE_Msk;    //����ϵͳ��ʱ��Ƶ��Ϊ180MHz
	    /*�رյδ�ʱ�����쳣*/
	  SysTick->CTRL&=~SysTick_CTRL_TICKINT_Msk;
	  SysTick->LOAD=(uint32_t)SYSTICK_MAX_LOAD;    /*ΪLOAD�Ĵ���װ�����ֵ2^24-1*/
	  fac_us=SystemCoreClock/(1e6);
}
/*
Author:HIT_XiaoWu
Date:2016/10/16/16:14
Desctription��΢������ʱ����
*/

void delay_us(uint32_t us){
	   SysTick->LOAD=(uint32_t)SYSTICK_MAX_LOAD;    /*ΪLOAD�Ĵ���װ�����ֵ2^24-1*/
	   /*���뺯���ĳ�ʼTicks*/
	   uint32_t ticksOld=SysTick->VAL;
	   /*�����δ�ʱ��*/
	   SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	   /*������Ҫ��ʱ��Ticks��*/
	   uint32_t ticksTotal=us*fac_us;
		 /*��ʼ״̬Systick��VALֵ*/
	   uint32_t ticksCnt=0,ticksNow=0;
	   while(ticksCnt<ticksTotal){
				   ticksNow=SysTick->VAL;
           if(ticksNow<=ticksOld)  ticksCnt +=ticksOld-ticksNow;	
           else                    ticksCnt +=SYSTICK_MAX_LOAD-ticksNow+ticksOld;
			     ticksOld=ticksNow;
		 }
		 /*�رյδ�ʱ��*/
		 SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
}
/*
Author:HIT_XiaoWu
Date:2016/10/16/16:14
Desctription��ms�������ʱ
*/
void delay_ms(uint32_t ms){
		 delay_us(1000*ms);              /*��ȷ��ʱ����*/
}
