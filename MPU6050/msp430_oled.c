/*OLED�ײ�����
*@��������ʽ���ֱ���Ϊ128*64
*��Ÿ�ʽ����.
*[0]0 1 2 3 ... 127	
*[1]0 1 2 3 ... 127	
*[2]0 1 2 3 ... 127	
*[3]0 1 2 3 ... 127	
*[4]0 1 2 3 ... 127	
*[5]0 1 2 3 ... 127	
*[6]0 1 2 3 ... 127	
*[7]0 1 2 3 ... 127 
*/
#include "msp430_oled.h"
#include "stdlib.h"
#include <oledfont.h>

#define X_WIDTH 128
#define Y_WIDTH 64

void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

u8 OLED_GRAM[8][128];

#if 1
uint8_t MasterType1[28] = { 0xae, 0x00, 0x10, 0x40, 0xb0, 0x81, 0xff, 0xa1,
                            0xa6, 0xa8, 0x3f, 0xc8, 0xd3, 0x00, 0xd5, 0x80,
                            0xd8, 0x05, 0xd9, 0xf1, 0xda, 0x12, 0xdb, 0x40,
                            0x8d, 0x14, 0xaf };

/* ����ʵ������ͼ���16*16����*/
const u8 nonside[] =
{
	0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,//��0,
};

void OLED_GPIO_Config(void)
{
	GPIO_setAsOutputPin(OLED_PORT,OLED_SCL_PIN+OLED_SDA_PIN+OLED_RST_PIN+OLED_DC_PIN);
}

void OLED_WrDat(u8 data)
{
	u8 i=8;
	//OLED_CS=0;;
	OLED_DC_SET();
  OLED_SCL_CLR();
  //asm("nop");    
  while(i--)
  {
    if(data&0x80){OLED_SDA_SET();}
    else{OLED_SDA_CLR();}
    OLED_SCL_SET(); 
    //asm("nop");;
		//asm("nop");            
    OLED_SCL_CLR();    
    data<<=1;    
  }
	//OLED_CS=1;
}
void OLED_WrCmd(u8 cmd)
{
	u8 i=8;
	
	//OLED_CS=0;;
	OLED_DC_CLR();
  OLED_SCL_CLR();
  //asm("nop");   
  while(i--)
  {
    if(cmd&0x80){OLED_SDA_SET();}
    else{OLED_SDA_CLR();}
    OLED_SCL_SET();
    //asm("nop");;
		//asm("nop");             
    OLED_SCL_CLR();    
    cmd<<=1;;   
  } 	
	//OLED_CS=1;
}

/*������ʼ��ַ
*Y���ǰ�8��ݽ��ģ�y��0~63��ֻ�ܰ�8��ı�����ʾ��
*��Ϊ����ʽֻ�ܰ�8���ֽڽ���
*/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    OLED_WrCmd(0xb0 + y);
    OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
    OLED_WrCmd((x & 0x0f) | 0x01);
}

void OLED_Fill(unsigned char bmp_dat)
{
    unsigned char y, x;
    for (y = 0; y < 8; y++)
    {
        OLED_WrCmd(0xb0 + y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10);
        for (x = 0; x < 128; x++)
            OLED_WrDat(bmp_dat);
    }
}

void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
    if (cmd)
    {
        OLED_WrDat(dat);
    }
    else
    {
        OLED_WrCmd(dat);
    }
}
void OLED_CLS(void)
{
	u8 y,x;	
	for(y=0;y<8;y++)
	{
		OLED_WrCmd(0xb0+y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10); 
		for(x=0;x<X_WIDTH;x++)
			OLED_WrDat(0);
	}
}
//���һ��0-63
void OLED_CLS_y(char y)
{
	u8 x;	
	
	OLED_WrCmd(0xb0+(y>>3));
	OLED_WrCmd(0x01);
	OLED_WrCmd(0x10); 
	for(x=0;x<X_WIDTH;x++)
	{
		OLED_WrDat(0);
	}
	
}

//���һ���ϵ�һ������y=0-63
void OLED_CLS_line_area(u8 start_x,u8 start_y,u8 width)
{
	u8 x;	
	
	OLED_WrCmd(0xb0+(start_y>>3));
	OLED_WrCmd(0x01);
	OLED_WrCmd(0x10); 
	for(x=start_x;x<width;x++)
	{
		OLED_WrDat(0);
	}
	
}


void OLED_DLY_ms(u16 ms)
{                         
  u16 a;
  while(ms)
  {
    a=1335;
    while(a--);
    ms--;
  }
  return;
}

void OLED_Init(void)        
{
  //DDRA=0XFF;
  OLED_GPIO_Config();
	OLED_SCL_SET();
	//OLED_CS=1;	//Ԥ��SLK��SSΪ�ߵ�ƽ   	
	
	OLED_RST_CLR();
	OLED_DLY_ms(50);
	OLED_RST_SET();

  OLED_WrCmd(0xae);//--turn off oled panel
  OLED_WrCmd(0x00);//---set low column address
  OLED_WrCmd(0x10);//---set high column address
  OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WrCmd(0x81);//--set contrast control register
  OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
  OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
  OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
  OLED_WrCmd(0xa6);//--set normal display
  OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  OLED_WrCmd(0x3f);//--1/64 duty
  OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WrCmd(0x00);//-not offset
  OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WrCmd(0xd9);//--set pre-charge period
  OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WrCmd(0xda);//--set com pins hardware configuration
  OLED_WrCmd(0x12);
  OLED_WrCmd(0xdb);//--set vcomh
  OLED_WrCmd(0x40);//Set VCOM Deselect Level
  OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WrCmd(0x02);//
  OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
  OLED_WrCmd(0x14);//--set(0x10) disable
  OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
  OLED_WrCmd(0xaf);//--turn on oled panel
  OLED_Fill(0x00);  //��ʼ����
  OLED_Set_Pos(0,0);  
	
} 

void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr,
                   unsigned char Char_Size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //�õ�ƫ�ƺ��ֵ			
    if (x > 128 - 1)
    {
        x = 0;
        y = y + 2;
    }
    if (Char_Size == 16)
    {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++)
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
    }
    else
    {

        OLED_Set_Pos(x, y);
        for (i = 0; i < 6; i++)
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);

    }
}

void OLED_U32toU16(unsigned char x, unsigned char y, unsigned int n,
                   unsigned char k)
{
    switch (n)
    {
    case 0:
        OLED_ShowChar(x, y, '0', k);
        break;
    case 1:
        OLED_ShowChar(x, y, '1', k);
        break;
    case 2:
        OLED_ShowChar(x, y, '2', k);
        break;
    case 3:
        OLED_ShowChar(x, y, '3', k);
        break;
    case 4:
        OLED_ShowChar(x, y, '4', k);
        break;
    case 5:
        OLED_ShowChar(x, y, '5', k);
        break;
    case 6:
        OLED_ShowChar(x, y, '6', k);
        break;
    case 7:
        OLED_ShowChar(x, y, '7', k);
        break;
    case 8:
        OLED_ShowChar(x, y, '8', k);
        break;
    case 9:
        OLED_ShowChar(x, y, '9', k);
        break;
    }
}

void OLED_Show_Number(unsigned char x, unsigned char y, unsigned int a,
                      unsigned char n)
{
    unsigned int b, c, d, e, f, g;
    b = a / 100000;
    c = a % 100000 / 10000;
    d = a % 10000 / 1000;
    e = a % 1000 / 100;
    f = a % 100 / 10;
    g = a % 10;
    if (b != 0)
    {
        OLED_U32toU16(x, y, b, n);
        OLED_U32toU16(x + n / 2, y, c, n);
        OLED_U32toU16(x + n, y, d, n);
        OLED_U32toU16(x + (n / 2) * 3, y, e, n);
        OLED_U32toU16(x + 2 * n, y, f, n);
        OLED_U32toU16(x + (n / 2) * 5, y, g, n);
    }
    else if (b == 0 && c != 0)
    {
        OLED_U32toU16(x, y, c, n);
        OLED_U32toU16(x + n / 2, y, d, n);
        OLED_U32toU16(x + n, y, e, n);
        OLED_U32toU16(x + (n / 2) * 3, y, f, n);
        OLED_U32toU16(x + 2 * n, y, g, n);
    }
    else if (b == 0 && c == 0 && d != 0)
    {
        OLED_U32toU16(x, y, d, n);
        OLED_U32toU16(x + n / 2, y, e, n);
        OLED_U32toU16(x + n, y, f, n);
        OLED_U32toU16(x + (n / 2) * 3, y, g, n);
    }
    else if (b == 0 && c == 0 && d == 0 && e != 0)
    {
        OLED_U32toU16(x, y, e, n);
        OLED_U32toU16(x + n / 2, y, f, n);
        OLED_U32toU16(x + n, y, g, n);
    }
    else if (b == 0 && c == 0 && d == 0 && e == 0 && f != 0)
    {
        OLED_U32toU16(x, y, f, n);
        OLED_U32toU16(x + n / 2, y, g, n);
    }
    else
    {
        OLED_U32toU16(x, y, g, n);
    }
}

/**
 *m^n����:��һ��������n�η�
 *m ����Ҫ��η���������
 *n ����Ҫ��Ĵη���
 *�������أ�m��n�η�
 */
u32 oled_pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
        result *= m;
    return result;
}
//��ʾ2������
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ 0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size2)
{
    u8 t, temp;
    u8 enshow = 0;
//    OLED_Fill(0x00);  //����
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
    }
}

void OLED_ShowString(u8 x, u8 y, char *chr, u8 Char_Size)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j], Char_Size);
        x += 8;
        if (x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}
//��ʾ����
void OLED_ShowCHinese(u8 x, u8 y, u8 no)
{
    u8 t, adder = 0;
    OLED_Set_Pos(x, y);
    for (t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
        adder += 1;
    }
    OLED_Set_Pos(x, y + 1);
    for (t = 0; t < 16; t++)
    {
        OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
        adder += 1;
    }
}

/**
 *@brief    ���㺯��
 *@param    x:0-127,y:0-63�������,color:1��䡢0����
 *@         pos_mode:1:����ʵʱ��������ģʽ��0����ʵʱ�������ꡣ
 *@retval   None
 *@note     pos_mode = 0:��ʵʱ��������,�����OLED_Refresh_Gram();ʹ��
 *@         pos_mode = 1:�������ñ���������ʵʱ��ʾ��
 */
void OLED_DrawPoint(u8 x, u8 y, u8 color, u8 pos_mode)
{
    u8 pos, bx, temp = 0;
    if (x > 127 || y > 63)
        return; //�������˳�
    //��������λ��
    pos = y / 8;    //�ڵڼ�page�����У�����1
    bx = y % 8; //��1����ڼ�λ

    temp = 1 << bx;          //��1����a%bλ (�дӵ͵��ߵ���)
    if (pos_mode)
        OLED_Set_Pos(x, pos);

    if (color)
    {
        OLED_GRAM[pos][x] |= temp;          //��1���ȷ����λ��
        if (pos_mode)
            OLED_WR_Byte(OLED_GRAM[pos][x], OLED_DATA);

    }
    else
    {
        OLED_GRAM[pos][x] &= ~temp;          //ȡ����0���ȷ����λ��
        if (pos_mode)
            OLED_WR_Byte(OLED_GRAM[pos][x], OLED_DATA);

    }
}
//�����Դ浽LCD
void OLED_Refresh_Gram(void)
{
    u8 i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);          //����ҳ��ַ��0~7��
        OLED_WR_Byte(0x00, OLED_CMD);    //������ʾλ��-�е͵�ַ
        OLED_WR_Byte(0x10, OLED_CMD);    //������ʾλ��-�иߵ�ַ

        for (n = 0; n < 128; n++)
            OLED_WR_Byte(OLED_GRAM[i][n], OLED_DATA);

    }

}
#endif
//��ֱ��
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, u8 color,
              u8 pos_mode)
{
    int16_t steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
        swap(&x0, &y0);
        //swap(x0, y0);
        swap(&x1, &y1);
    }

    if (x0 > x1)
    {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1)
        ystep = 1;
    else
        ystep = -1;

    for (; x0 <= x1; x0++)
    {
        if (steep)
            OLED_DrawPoint(y0, x0, color, pos_mode);
        else
            OLED_DrawPoint(x0, y0, color, pos_mode);
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}
/**
 @brief     ��Բ����
 @param     Բ������x0��y0,r:�뾶
 @retval    None
 @note      None
 */
void drawCircle(int16_t x0, int16_t y0, int16_t r, u8 color, u8 pos_mode)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    OLED_DrawPoint(x0, y0 + r, color, pos_mode);
    OLED_DrawPoint(x0, y0 - r, color, pos_mode);
    OLED_DrawPoint(x0 + r, y0, color, pos_mode);
    OLED_DrawPoint(x0 - r, y0, color, pos_mode);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        OLED_DrawPoint(x0 + x, y0 + y, color, pos_mode);
        OLED_DrawPoint(x0 - x, y0 + y, color, pos_mode);
        OLED_DrawPoint(x0 + x, y0 - y, color, pos_mode);
        OLED_DrawPoint(x0 - x, y0 - y, color, pos_mode);
        OLED_DrawPoint(x0 + y, y0 + x, color, pos_mode);
        OLED_DrawPoint(x0 - y, y0 + x, color, pos_mode);
        OLED_DrawPoint(x0 + y, y0 - x, color, pos_mode);
        OLED_DrawPoint(x0 - y, y0 - x, color, pos_mode);
    }
}
//��ֱ��
void drawFastVLine(int16_t x, int16_t y, int16_t h, u8 color, u8 pos_mode)
{
    // Update in subclasses if desired!
    drawLine(x, y, x, y + h - 1, color, pos_mode);
}
//ˮƽ��
void drawFastHLine(int16_t x, int16_t y, int16_t w, u8 color, u8 pos_mode)
{
    // Update in subclasses if desired!
    drawLine(x, y, x + w - 1, y, color, pos_mode);
}
/**
 @brief     �����κ���
 @param
 @retval    None
 @note      None
 */
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, u8 color, u8 pos_mode)
{

    drawFastHLine(x, y, w, color, pos_mode);
    drawFastHLine(x, y + h - 1, w, color, pos_mode);
    drawFastVLine(x, y, h, color, pos_mode);
    drawFastVLine(x + w - 1, y, h, color, pos_mode);
}
//���Σ�����Խ�����
void drawRect2(int16_t x0, int16_t y0, int16_t x1, int16_t y1, u8 color,
               u8 pos_mode)
{
    drawLine(x0, y0, x1, y0, color, pos_mode);  //��
    drawLine(x0, y1, x1, y1, color, pos_mode);  //��
    drawLine(x0, y0, x0, y1, color, pos_mode);  //���
    drawLine(x1, y0, x1, y1, color, pos_mode);  //�ұ�

}
/**
 @brief     �����κ���
 @param
 @retval    None
 @note      None
 */
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, u8 color, u8 pos_mode)
{
    int16_t i;
    // Update in subclasses if desired!
    for (i = x; i < x + w; i++)
        drawFastVLine(i, y, h, color, pos_mode);
}
/*
 *@brief    ���Բ
 */
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                      int16_t delta, uint16_t color, u8 pos_mode)
{

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1)
        {
            drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color, pos_mode);
            drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color, pos_mode);
        }
        if (cornername & 0x2)
        {
            drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color, pos_mode);
            drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color, pos_mode);
        }
    }
}
//���Բ
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color, u8 pos_mode)
{
    drawFastVLine(x0, y0 - r, 2 * r + 1, color, pos_mode);
    fillCircleHelper(x0, y0, r, 3, 0, color, pos_mode);
}


