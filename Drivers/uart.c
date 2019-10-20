#include "uart.h"
/*
Author：HIT_XiaoWu
Date:2019/10/19/15:16
Description:完成printf的重定向工作
*/
int fputc(int ch,FILE *f){
	/*等待串口发送完毕*/
	while((USART1->SR&USART_SR_TXE)==RESET){;}
	/*调用串口发送函数*/
  USART1->DR=(uint8_t)ch;
	return ch;
}
/*
Author：HIT_XiaoWu
Date:2019/10/19/15:16
Description：完成对串口1的初始化
*/
void  uart1Init(uint32_t baudrate){
/*-----------------Config RCC---------------------*/
		{
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		}
/*-----------------Config GPIO---------------------*/
		{
		  GPIO_InitTypeDef gpio;
			gpio.GPIO_Mode=GPIO_Mode_AF;
			gpio.GPIO_OType=GPIO_OType_PP;
			gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
			gpio.GPIO_Speed=GPIO_Speed_50MHz;
			gpio.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
			GPIO_Init(GPIOA,&gpio);
		}

/*-----------------Config AF ---------------------*/
			GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
			GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
/*-----------------Config UART1---------------------*/
      USART_InitTypeDef uart;
		  uart.USART_BaudRate=baudrate;
		  uart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
		  uart.USART_Parity=USART_Parity_No;
		  uart.USART_StopBits=USART_StopBits_1;
		  uart.USART_WordLength=USART_WordLength_8b;
		  uart.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
		  USART_Init(USART1,&uart);
		  USART_Cmd(USART1,ENABLE);
}
/*
Author：HIT_XiaoWu
Date:2019/10/19/15:16
Description：串口发送一个字节
*/
void uartSendByte(USART_TypeDef *uart,uint8_t dat){
		 while((uart->SR&USART_SR_TXE)==RESET){;}
     uart->DR=(uint8_t)dat;
}
/*
Author：HIT_XiaoWu
Date:2019/10/19/15:36
Description：串口读取一个字节
*/
uint8_t uartReadByte(USART_TypeDef *uart){
				while((uart->SR&USART_SR_RXNE)==RESET){;}
	      return (uint8_t)uart->DR;   
}
/*
Author：HIT_XiaoWu
Date:2019/10/19/15:36
Description：串口发送若干个字节
*/
void uartSendBytes(uint8_t *buf,uint16_t len){
		 while(len--){
			 uartSendByte(USART1,*buf);buf++;  /*发送若干个字节*/
		 }
}
/*
Author：HIT_XiaoWu
Date:2019/10/19/15:36
Description：串口发送一个字符串
*/
void uartSendString(char*str){
		while(*str!='\0'){
				uartSendByte(USART1,*str);str++;
		}
}
