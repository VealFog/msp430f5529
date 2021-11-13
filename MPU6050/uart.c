#include "uart.h"
#define DISABLE_INT   UART_SR=__get_SR_register(); _DINT()
#define RESTORE_INT   if(UART_SR & GIE)            _EINT()

unsigned char UART_SR;

unsigned char UART_TXBUF[UART_TXBUFSIZE];       //����FIFO����
unsigned char UART_RXBUF[UART_RXBUFSIZE];       //����FIFO����
unsigned char UART_TXIndexR = 0;                //����FIFO�Ķ�ָ��
unsigned char UART_TXIndexW = 0;                //����FIFO��дָ��
unsigned char UART_RXIndexR = 0;                //����FIFO�Ķ�ָ��
unsigned char UART_RXIndexW = 0;                //����FIFO��дָ��
unsigned char UART_OutLen = 0;                  //����FIFO�ڴ����͵��ֽ���
unsigned char UART_InLen = 0;                   //����FIFO�ڴ���ȡ���ֽ���

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

    //------------------------------����У��λ------------------------------
    switch (Parity)
    {
    case 'n':
    case 'N':
        UCA1CTL0 &= ~UCPEN;
        break;          //��У��
    case 'p':
    case 'P':
        UCA1CTL0 |= UCPEN + UCPAR;
        break;    //żУ��
    case 'o':
    case 'O':
        UCA1CTL0 |= UCPEN;
        break;          //��У��
        //default:  return (0);                                 //��������
    }
    //------------------------------��������λ------------------------------
    switch (DataBits)
    {
    case 7:
    case '7':
        UCA1CTL0 |= UC7BIT;
        break;         //7λ����λ
    case 8:
    case '8':
        UCA1CTL0 &= ~UC7BIT;
        break;         //8λ����λ
        //default:  return (0);                                 //��������
    }
    //------------------------------����ֹͣλ------------------------------
    switch (StopBits)
    {
    case 1:
    case '1':
        UCA1CTL0 &= ~UCSPB;
        break;          //1λֹͣλ
    case 2:
    case '2':
        UCA1CTL0 |= UCSPB;
        break;          //2λֹͣλ
        //default:  return (0);                                 //��������
    }

    // over sampling
    UCA1CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
    UCA1IE |= UCRXIE; //�����ж�

}

/*******************************************************************************
 �������ƣ�UART_PutChar()
 �������ܣ�UART ����һ���ֽ�
 ��ڲ�����
 Chr��   �����͵�һ�ֽ�����
 ���ڲ���������1��ʾ���ͳɹ�������0��ʾ����ʧ��
 Ӧ�÷�����UART_PutChar('a');
 ����˵�������͹����У�������CPU����
 *******************************************************************************/
char UART_PutChar(unsigned char Chr)
{
    if (UART_OutLen == UART_TXBUFSIZE)
        return (0);
    if (UART_OutLen == 0)
        UCA1IFG |= UCTXIFG; //��Ϊ��һ���ֽ�,��Ϊ�����һ�η����ж�
    DISABLE_INT
    ;
    UART_OutLen++;
    UART_TXBUF[UART_TXIndexW] = Chr;          //��������ͨ��дָ��д��FIFO
    if (++UART_TXIndexW >= UART_TXBUFSIZE)
        UART_TXIndexW = 0;
    RESTORE_INT;
    UCA1IE |= UCTXIE;                         //���������ж�
    return (1);
}

/****************************************************************************
 �������ƣ�putchar()
 �������ܣ����׼�ն��豸����һ�ֽ�����(1��ASCII�ַ�)
 ��ڲ�����
 ch��    �����͵��ַ�  
 ���ڲ������������ַ�
 ����˵����printf��������øú�����Ϊ�ײ����������Ӵ�������ַ���PC���ĳ����ն�
 ����ϣ�printf�Ľ������ӡ�������ն��ϡ����޸ĸú��������ַ���������ʽ
 ���������ʾ��LCD�ϣ�printf�Ľ������ʾ��LCD�ϡ�
 ****************************************************************************/
int putchar(int ch)
{
    if (ch == '\n')        //  '\n'(�س�)��չ�� '\n''\r' (�س�+����) 
    {
        UART_PutChar(0x0d); //'\r'
    }
    UART_PutChar(ch);      //�Ӵ��ڷ�������
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
        UART_RXBUF[UART_RXIndexW] = UCA1RXBUF;    //��������ͨ��дָ��д��FIFO
        UART_PutChar(UCA1RXBUF);                    //���ʹ����յ������ݣ�ʵ�������Ѿ��洢������һ��������������ⲿ���ã������������ʾ�ڵ�����
        if (++UART_RXIndexW >= UART_RXBUFSIZE)
            UART_RXIndexW = 0;

        break;
    case 4:                       // Vector 4 - TXIFG
        //P1OUT ^=BIT0;
        if (UART_OutLen > 0)                      //��FIFO���д����͵�����
        {
            UART_OutLen--;
            UCA1TXBUF = UART_TXBUF[UART_TXIndexR];  //ͨ����ָ���FIFO�ж�ȡ���ݲ�����
            while (!(UCA1IFG & UCTXIFG));
            if (++UART_TXIndexR >= UART_TXBUFSIZE)
                UART_TXIndexR = 0;
        }
        else{
            UCA1IE &= ~UCTXIE;                    //���ݷ�����ϣ��رշ����ж�

        }
        break;
    default:
        break;
    }


//     //�����ж�,�������յ�����ʱ����
//     if(UCA1IV ==2)
//     {
//     while (!(UCA0IFG&UCTXIFG));             // �ж��Ƿ��Ƿ����ж�
//     UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
//     P1OUT ^= BIT0;
//     }
//     //�����жϣ��������ݷ��뷢�ͻ�����Ҫ����ʱ����
//     if(UCA1IV ==4)
//     {
//     if (UART_OutLen > 0)                      //��FIFO���д����͵�����
//     {
//     UART_OutLen--;
//     UCA1TXBUF = UART_TXBUF[UART_TXIndexR];  //ͨ����ָ���FIFO�ж�ȡ���ݲ�����
//     while (!(UCA1IFG & UCTXIFG));
//     if (++UART_TXIndexR >= UART_TXBUFSIZE)  UART_TXIndexR = 0;
//     }
//     else UCA1IE &=~UCTXIE;                    //���ݷ�����ϣ��رշ����ж�
//     //P4OUT ^= BIT7;
//     }

}
