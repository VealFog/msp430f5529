#ifndef _UART_H_
#define _UART_H_

#include <msp430.h> 
#include "stdio.h"
#include "MYTYPE.h"

//���²�����Ҫ�û�����
#define UART_TXBUFSIZE      64                //����FIFO����
#define UART_RXBUFSIZE      16                //����FIFO����

extern uint16 interrupt_count;
extern uint8 interrupt_flag;
unsigned char UART_SR;

extern unsigned char UART_TXBUF[UART_TXBUFSIZE];       //����FIFO����
extern unsigned char UART_RXBUF[UART_RXBUFSIZE];       //����FIFO����
extern unsigned char UART_TXIndexR;                //����FIFO�Ķ�ָ��
extern unsigned char UART_TXIndexW;                //����FIFO��дָ��
extern unsigned char UART_RXIndexR;                //����FIFO�Ķ�ָ��
extern unsigned char UART_RXIndexW;                //����FIFO��дָ��
extern unsigned char UART_OutLen;                  //����FIFO�ڴ����͵��ֽ���
extern unsigned char UART_InLen;                   //����FIFO�ڴ���ȡ���ֽ���

void UART_Init(char Parity, char DataBits, char StopBits);
char UART_PutChar(unsigned char Chr);

#endif

