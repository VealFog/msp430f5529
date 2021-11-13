#include "MPU6050.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI 3.14159


float Angle, Angle_dot; //外部需要引用的变量,倾角值和倾角加速度值

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//**************************************
//I2C起始信号
//**************************************
void I2C_Start()
{
    SDAOUT;
    SDA1;                    //拉高数据线
    SCLOUT;
    SCL1;                    //拉高时钟线
    delayus(5);                 //延时
    SDA0;                    //产生下降沿
    delayus(5);                  //延时
    SCL0;                    //拉低时钟线
}
//**************************************
//I2C停止信号
//**************************************
void I2C_Stop()
{
    SDAOUT;
    SDA0;                    //拉低数据线
    SCL0;
    SCLOUT;
    SCL1;                    //拉高时钟线
    delayus(5);                   //延时
    SDA1;                    //产生上升沿
    delayus(5);                  //延时
}

//**************************************
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(uchar ack) {

    SDAOUT;
    SCLOUT;
    if(ack) SDA1;
    else SDA0;
//    SDA = ack;                  //写应答信号
    SCL1;                    //拉高时钟线
    delayus(5);                  //延时
    SCL0;                    //拉低时钟线
    delayus(5);                  //延时
}

//**************************************
//I2C接收应答信号
//**************************************
uchar I2C_RecvACK()
{
    uchar cy;
    SCLOUT;
    SCL1;                    //拉高时钟线
    SDAIN;
    delayus(5);                 //延时
    if(SDADATA) {
        cy=1;
    }
    else{
        cy=0;
    }
//    cy = SDA;                   //读应答信号
    SCL0;                    //拉低时钟线
    delayus(5);                //延时
    SDAOUT;
    return cy;
}

//**************************************
//向I2C总线发送一个字节数据
//**************************************

void I2C_SendByte(uchar dat)
{
    uchar i;
    SDAOUT;
    for (i=0; i<8; i++){         //8位计数器
        if((dat<<i)&0x80)
        {
            SDA1;
        }
        else
        {
            SDA0;
        }
       // SDA = cy;               //送数据口
        SCLOUT;
        SCL1;                //拉高时钟线
        delayus(5);              //延时
        SCL0;                //拉低时钟线
        delayus(5);              //延时
    }
    I2C_RecvACK();
}

//**************************************
//从I2C总线接收一个字节数据
//**************************************

uchar I2C_RecvByte(){

    uchar i;
    uchar dat = 0,cy;
    SDAOUT;
    SCLOUT;
    SDA1;                    //使能内部上拉,准备读取数据,
    SDAIN;
    for (i=0; i<8; i++)         //8位计数器
    {
            dat<<= 1;
            SCL1;                //拉高时钟线
            delayus(5);             //延时
            if(SDADATA){
                cy=1;
            }
            else{
                cy=0;
            }
            dat |= cy;             //读数据         SCL0;                //拉低时钟线
            delayus(5);             //延时
    }
    SDAOUT;
    return dat;
}


/**
 *@写单个字节
 */
void Single_WriteI2C(unsigned char Addr, unsigned char data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(Addr);    //内部寄存器地址
    I2C_SendByte(data);       //内部寄存器数据
    I2C_Stop();                   //发送停止信号
}

uchar  Single_ReadI2C(uchar REG_Address)
{
    uchar REG_data;
    I2C_Start();                   //起始信号
    I2C_SendByte(SlaveAddress);    //发送设备地址+写信号
    I2C_SendByte(REG_Address);     //发送存储单元地址从0开始
    I2C_Start();                   //起始信号
    I2C_SendByte(SlaveAddress+1);  //发送设备地址+读信号
    REG_data=I2C_RecvByte();       //读出寄存器数据
    I2C_SendACK(1);                //接收应答信号
    I2C_Stop();                    //停止信号
    return REG_data;
}

//**************************************
//初始化MPU6050
//**************************************

void InitMPU6050(void)
{
    Single_WriteI2C(PWR_MGMT_1, 0x00);  // 解除休眠状态
    Single_WriteI2C(SMPLRT_DIV, 0x07);  // 陀螺仪采样率设置（125HZ）
    Single_WriteI2C(CONFIG, 0x06);      // 低通滤波器频率设置（5HZ）
    Single_WriteI2C(GYRO_CONFIG, 0x18); // 陀螺仪自检及检测范围设置(不自检,16.4LSB/DBS/S)
    Single_WriteI2C(ACCEL_CONFIG, 0x01); // 加速计自检、测量范围及高通滤波频率(不自检，2G(16384LSB/G)，5Hz)
}

void MPU_IIC_Delay(void)
{
    delayus(2);
}

void MPU_IIC_Start(void)
{
    SDAOUT;
    SDA1;
    SCL1;
    MPU_IIC_Delay();
    SDA0;
    MPU_IIC_Delay();
    SCL0;

}

void MPU_IIC_Stop(void)
{
    SDAOUT;
    SCL0;
    SDA0;//STOP:when CLK is high DATA change form low to high
    MPU_IIC_Delay();
    SCL1;
    SDA1;
    MPU_IIC_Delay();
}

u8 MPU_IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    SDAIN;
    SDA1;MPU_IIC_Delay();
    SCL1;MPU_IIC_Delay();
    while(SDADATA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            MPU_IIC_Stop();
            return 1;
        }
    }
    SCL0;
    return 0;
}

void MPU_IIC_Ack(void)
{
    SCL0;
    SDAOUT;
    SDA0;
    MPU_IIC_Delay();
    SCL1;
    MPU_IIC_Delay();
    SCL0;
}
void MPU_IIC_NAck(void)
{
    SCL0;
    SDAOUT;
    SDA1;
    MPU_IIC_Delay();
    SCL1;
    MPU_IIC_Delay();
    SCL0;
}

void MPU_IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDAOUT;
    SCL0;
    for(t=0;t<8;t++)
    {
        if(((txd&0x80)>>7)==1)
            SDA1;
        else SDA0;

        txd<<=1;
            SCL1;
            MPU_IIC_Delay();
            SCL0;
            MPU_IIC_Delay();
    }
}

u8 MPU_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDAIN;
    for(i=0;i<8;i++ )
    {
        SCL0;
        MPU_IIC_Delay();
        SCL1;
        receive<<=1;
        if(SDADATA)receive++;
        MPU_IIC_Delay();
    }
    if (!ack)
        MPU_IIC_NAck();//路垄脣脥nACK
    else
        MPU_IIC_Ack(); //路垄脣脥ACK
    return receive;
}

u8 MPU_Read_Byte(u8 reg)
{
    u8 res;
    MPU_IIC_Start();

    MPU_IIC_Send_Byte((0X68<<1)|0);
    MPU_IIC_Wait_Ack();
    MPU_IIC_Send_Byte(reg);
    MPU_IIC_Wait_Ack();
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((0X68<<1)|1);
    MPU_IIC_Wait_Ack();
    res=MPU_IIC_Read_Byte(0);
    MPU_IIC_Stop();
    return res;
}

u8 MPU_Write_Byte(u8 reg,u8 data)
{
    MPU_IIC_Start();
    MPU_IIC_Send_Byte((0X68<<1)|0);
    if(MPU_IIC_Wait_Ack())
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Send_Byte(reg);
    MPU_IIC_Wait_Ack();
    MPU_IIC_Send_Byte(data);
    if(MPU_IIC_Wait_Ack())
    {
        MPU_IIC_Stop();
        return 1;
    }
    MPU_IIC_Stop();
    return 0;
}

//**************************************
//合成数据
//**************************************
int Get6050Data(u8 REG_Address)
{
    u8 H,L;
    H=MPU_Read_Byte(REG_Address);
    L=MPU_Read_Byte(REG_Address+1);
    return (H<<8)+L;   //合成数据
}

float Mpu6050AccelAngle(int8 ax_dir, int8 az_dir)
//float Mpu6050AccelAngle(int8 dir)
{
#if 1
    float accel_agle; // 测量的倾角值
    float ax_result, ax_result1; // 测量值缓存变量
    float az_result, az_result1;

    ax_result = (float) Get6050Data(ax_dir); // 测量当前方向的加速度值,转换为浮点数
    az_result = (float) Get6050Data(az_dir);

    ax_result1 = map(ax_result, -15924, 16629, -16384, 16384);
    az_result1 = map(az_result, -17454, 15750, -16384, 16384);

    accel_agle = atan2(ax_result1, az_result1) * 180 / PI; //加速度计算角度。得到平衡位置的角度

    return accel_agle; // 返回测量值

#endif
#if 0 //该方法不准，原因未知
    float accel_agle; // 测量的倾角值
    float result;// 测量值缓存变量
    result = (float)Get6050Data(dir);// 测量当前方向的加速度值,转换为浮点数
    accel_agle = (result + MPU6050_ZERO_ACCELL)/16384;// 去除零点偏移,计算得到角度（弧度）
    accel_agle = accel_agle*1.2*180/PI;//弧度转换为度

    return accel_agle;// 返回测量值
#endif
}
float Mpu6050GyroAngle(int8 dir)
{
    float gyro_angle, gyro_angle1;
    gyro_angle = (float) Get6050Data(dir);   // 检测陀螺仪的当前值

    gyro_angle1 = -(gyro_angle - MPU6050_ZERO_GYRO) / 131.0; //去除零点偏移，计算角速度值,负号为方向处理 
    return gyro_angle1; // 返回测量值
}
