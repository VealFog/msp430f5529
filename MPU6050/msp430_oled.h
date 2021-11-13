#ifndef _MSP430_OLED_H
#define _MSP430_OLED_H

#include "driverlib.h"

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;

//汉字大小，英文数字大小
#define 	TYPE8X16		1
#define 	TYPE16X16		2
#define 	TYPE6X8			3


#define OLED_CMD        0   //写命令
#define OLED_DATA       1   //写数据


//-----------------OLED端口定义----------------  					   

#define OLED_PORT       GPIO_PORT_P6
#define OLED_SCL_PIN    GPIO_PIN1
#define OLED_SDA_PIN    GPIO_PIN2
#define OLED_RST_PIN	    GPIO_PIN3
#define OLED_DC_PIN     GPIO_PIN4


#define OLED_SCL_CLR()	GPIO_setOutputLowOnPin(OLED_PORT, OLED_SCL_PIN)
#define OLED_SCL_SET()	GPIO_setOutputHighOnPin(OLED_PORT, OLED_SCL_PIN)

#define OLED_SDA_CLR()	GPIO_setOutputLowOnPin(OLED_PORT,OLED_SDA_PIN)
#define OLED_SDA_SET()	GPIO_setOutputHighOnPin(OLED_PORT,OLED_SDA_PIN)

#define OLED_RST_CLR()	GPIO_setOutputLowOnPin(OLED_PORT,OLED_RST_PIN)
#define OLED_RST_SET()	GPIO_setOutputHighOnPin(OLED_PORT,OLED_RST_PIN)

#define OLED_DC_CLR()	GPIO_setOutputLowOnPin(OLED_PORT,OLED_DC_PIN)
#define OLED_DC_SET()	GPIO_setOutputHighOnPin(OLED_PORT,OLED_DC_PIN)


void OLED_Init(void);
void OLED_CLS(void);
void OLED_CLS_y(char y);
void OLED_CLS_line_area(u8 start_x,u8 start_y,u8 width);



void OLED_Fill(u8 dat);
void OLED_Refresh_Gram(void);
void OLED_ShowString(u8 x, u8 y, char *chr, u8 Char_Size);
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr,
                   unsigned char Char_Size);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2); //位数增加时，往左移动


#endif

