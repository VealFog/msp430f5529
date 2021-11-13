#ifndef _UART_H_
#define _UART_H_

#include <msp430.h> 
#include "stdio.h"
#include "MYTYPE.h"

//以下部分需要用户配置
#define UART_TXBUFSIZE      64                //发送FIFO容量
#define UART_RXBUFSIZE      16                //接收FIFO容量

extern uint16 interrupt_count;
extern uint8 interrupt_flag;
unsigned char UART_SR;

extern unsigned char UART_TXBUF[UART_TXBUFSIZE];       //发送FIFO数组
extern unsigned char UART_RXBUF[UART_RXBUFSIZE];       //接收FIFO数组
extern unsigned char UART_TXIndexR;                //发送FIFO的读指针
extern unsigned char UART_TXIndexW;                //发送FIFO的写指针
extern unsigned char UART_RXIndexR;                //接收FIFO的读指针
extern unsigned char UART_RXIndexW;                //接收FIFO的写指针
extern unsigned char UART_OutLen;                  //发送FIFO内待发送的字节数
extern unsigned char UART_InLen;                   //接收FIFO内待读取的字节数

void UART_Init(char Parity, char DataBits, char StopBits);
char UART_PutChar(unsigned char Chr);

#endif

