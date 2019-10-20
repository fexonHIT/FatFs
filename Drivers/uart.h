#ifndef __UART_H
#define __UART_H
#include <stdio.h>
#include "app.h"
void  uart1Init(uint32_t baudrate);
void uartSendByte(USART_TypeDef *uart,uint8_t dat);
uint8_t uartReadByte(USART_TypeDef *uart);
void uartSendBytes(uint8_t *buf,uint16_t len);
void uartSendString(char*str);
#endif /*__UART_H*/
