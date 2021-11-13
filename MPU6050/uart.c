#include "uart.h"
#define DISABLE_INT   UART_SR=__get_SR_register(); _DINT()
#define RESTORE_INT   if(UART_SR & GIE)            _EINT()

unsigned char UART_SR;

unsigned char UART_TXBUF[UART_TXBUFSIZE];       //发送FIFO数组
unsigned char UART_RXBUF[UART_RXBUFSIZE];       //接收FIFO数组
unsigned char UART_TXIndexR = 0;                //发送FIFO的读指针
unsigned char UART_TXIndexW = 0;                //发送FIFO的写指针
unsigned char UART_RXIndexR = 0;                //接收FIFO的读指针
unsigned char UART_RXIndexW = 0;                //接收FIFO的写指针
unsigned char UART_OutLen = 0;                  //发送FIFO内待发送的字节数
unsigned char UART_InLen = 0;                   //接收FIFO内待读取的字节数

//uint16 interrupt_count=0;
//uint8 interrupt_flag=0;

void UART_Init( char Parity, char DataBits, char StopBits)
{
    P4SEL = BIT4 + BIT5;                        // P4.4,5 = USCI_A0 TXD/RXD
    UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA1CTL1 |= UCSSEL_2;                     // SMCLK 25mhz
    UCA1BR0 = 8;                              // 16MHz 115200 (see User's Guide)
    UCA1BR1 = 0;                              // 16MHz 115200
    UCA1MCTL = UCBRS_0 + UCBRF_10 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,

    //------------------------------设置校验位------------------------------
    switch (Parity)
    {
    case 'n':
    case 'N':
        UCA1CTL0 &= ~UCPEN;
        break;          //无校验
    case 'p':
    case 'P':
        UCA1CTL0 |= UCPEN + UCPAR;
        break;    //偶校验
    case 'o':
    case 'O':
        UCA1CTL0 |= UCPEN;
        break;          //奇校验
        //default:  return (0);                                 //参数错误
    }
    //------------------------------设置数据位------------------------------
    switch (DataBits)
    {
    case 7:
    case '7':
        UCA1CTL0 |= UC7BIT;
        break;         //7位数据位
    case 8:
    case '8':
        UCA1CTL0 &= ~UC7BIT;
        break;         //8位数据位
        //default:  return (0);                                 //参数错误
    }
    //------------------------------设置停止位------------------------------
    switch (StopBits)
    {
    case 1:
    case '1':
        UCA1CTL0 &= ~UCSPB;
        break;          //1位停止位
    case 2:
    case '2':
        UCA1CTL0 |= UCSPB;
        break;          //2位停止位
        //default:  return (0);                                 //参数错误
    }

    // over sampling
    UCA1CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
    UCA1IE |= UCRXIE; //接收中断

}

/*******************************************************************************
 函数名称：UART_PutChar()
 函数功能：UART 发送一个字节
 入口参数：
 Chr：   待发送的一字节数据
 出口参数：返回1表示发送成功，返回0表示发送失败
 应用范例：UART_PutChar('a');
 函数说明：发送过程中，不阻塞CPU运行
 *******************************************************************************/
char UART_PutChar(unsigned char Chr)
{
    if (UART_OutLen == UART_TXBUFSIZE)
        return (0);
    if (UART_OutLen == 0)
        UCA1IFG |= UCTXIFG; //若为第一个字节,人为制造第一次发送中断
    DISABLE_INT
    ;
    UART_OutLen++;
    UART_TXBUF[UART_TXIndexW] = Chr;          //发送数据通过写指针写入FIFO
    if (++UART_TXIndexW >= UART_TXBUFSIZE)
        UART_TXIndexW = 0;
    RESTORE_INT;
    UCA1IE |= UCTXIE;                         //开启发送中断
    return (1);
}

/****************************************************************************
 函数名称：putchar()
 函数功能：向标准终端设备发送一字节数据(1个ASCII字符)
 入口参数：
 ch：    待发送的字符  
 出口参数：发出的字符
 函数说明：printf函数会调用该函数作为底层输出。这里从串口输出字符到PC机的超级终端
 软件上，printf的结果将打印到超级终端上。若修改该函数，将字符以其他方式
 输出，如显示到LCD上，printf的结果将显示在LCD上。
 ****************************************************************************/
int putchar(int ch)
{
    if (ch == '\n')        //  '\n'(回车)扩展成 '\n''\r' (回车+换行) 
    {
        UART_PutChar(0x0d); //'\r'
    }
    UART_PutChar(ch);      //从串口发出数据
    return (ch);
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    switch (__even_in_range(UCA1IV, 4))
    {
    case 0:
        break;                 // Vector 0 - no interrupt
    case 2:                       // Vector 2 - RXIFG
        //P1OUT ^=BIT0;
        UART_InLen++;
        UART_RXBUF[UART_RXIndexW] = UCA1RXBUF;    //接收数据通过写指针写入FIFO
        UART_PutChar(UCA1RXBUF);                    //发送串口收到的数据，实际数据已经存储在了上一步的数组里，可以外部调用，这个仅用来显示在电脑上
        if (++UART_RXIndexW >= UART_RXBUFSIZE)
            UART_RXIndexW = 0;

        break;
    case 4:                       // Vector 4 - TXIFG
        //P1OUT ^=BIT0;
        if (UART_OutLen > 0)                      //若FIFO内有待发送的数据
        {
            UART_OutLen--;
            UCA1TXBUF = UART_TXBUF[UART_TXIndexR];  //通过读指针从FIFO中读取数据并发送
            while (!(UCA1IFG & UCTXIFG));
            if (++UART_TXIndexR >= UART_TXBUFSIZE)
                UART_TXIndexR = 0;
        }
        else{
            UCA1IE &= ~UCTXIE;                    //数据发送完毕，关闭发送中断

        }
        break;
    default:
        break;
    }


//     //接收中断,当串口收到数据时发生
//     if(UCA1IV ==2)
//     {
//     while (!(UCA0IFG&UCTXIFG));             // 判断是否是发送中断
//     UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
//     P1OUT ^= BIT0;
//     }
//     //发送中断，当有数据放入发送缓存需要发送时发生
//     if(UCA1IV ==4)
//     {
//     if (UART_OutLen > 0)                      //若FIFO内有待发送的数据
//     {
//     UART_OutLen--;
//     UCA1TXBUF = UART_TXBUF[UART_TXIndexR];  //通过读指针从FIFO中读取数据并发送
//     while (!(UCA1IFG & UCTXIFG));
//     if (++UART_TXIndexR >= UART_TXBUFSIZE)  UART_TXIndexR = 0;
//     }
//     else UCA1IE &=~UCTXIE;                    //数据发送完毕，关闭发送中断
//     //P4OUT ^= BIT7;
//     }

}
