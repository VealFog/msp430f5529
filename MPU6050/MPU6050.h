#ifndef MPU_6050_
#define MPU_6050_

#include <msp430.h> 
#include <stdint.h>
#include "msp430_oled.h"

#define CPU_F ((double)16000000)
#define delayus(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))  //宏定义延时函数
#define delayms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define SCL1 P4OUT |=BIT1       //IIC数据引脚
#define SCL0 P4OUT &=~BIT1
#define SCLOUT P4DIR |= BIT1;


#define SDA1 P4OUT |=BIT2
#define SDA0 P4OUT &=~BIT2
#define SDAIN P4DIR &=~BIT2
#define SDAOUT P4DIR |=BIT2
#define SDADATA (P4IN & BIT2)

//********Mpu6050的零点校准值**************
#define MPU6050_ZERO_ACCELL 378
#define MPU6050_ZERO_GYRO 30

//*************定义MPU6050内部地址*******************
#define  SMPLRT_DIV    0x19  //陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG      0x1A  //低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG   0x1B  //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG          0x1C  //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
/***************加速度传感器寄存器******************/
#define ACCEL_XOUT_H          0x3B
#define ACCEL_XOUT_L          0x3C
#define ACCEL_XOUT      ACCEL_XOUT_H    // X轴读取地址，高位为起始位
#define ACCEL_YOUT_H          0x3D
#define ACCEL_YOUT_L          0x3E
#define ACCEL_YOUT      ACCEL_YOUT_H    // Y轴读取地址，高位为起始位
#define ACCEL_ZOUT_H          0x3F
#define ACCEL_ZOUT_L          0x40    
#define ACCEL_ZOUT      ACCEL_ZOUT_H    // Z轴读取地址，高位为起始位
/*****************温度传感器寄存器****************/
#define TEMP_OUT_H    0x41
#define TEMP_OUT_L    0x42    
#define TEMP_OUT          TEMP_OUT_H    // 温度传感器读取地址，高位为起始位
/*****************陀螺仪寄存器********************/
#define GYRO_XOUT_H   0x43
#define GYRO_XOUT_L   0x44  
#define GYRO_XOUT        GYRO_XOUT_H    // 陀螺仪X轴读取地址，高位为起始位
#define GYRO_YOUT_H   0x45
#define GYRO_YOUT_L   0x46
#define GYRO_YOUT        GYRO_YOUT_H    // 陀螺仪Y轴读取地址，高位为起始位
#define GYRO_ZOUT_H   0x47
#define GYRO_ZOUT_L   0x48 
#define GYRO_ZOUT        GYRO_ZOUT_H    // 陀螺仪Z轴读取地址，高位为起始位

#define PWR_MGMT_1    0x6B  //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I          0x75  //IIC地址寄存器(默认数值0x68，只读)
#define SlaveAddress          0xD0  //IIC写入时的地址字节数据，+1为读取

typedef signed char int8;
typedef unsigned char uchar;

u8 MPU_IIC_Read_Byte(unsigned char ack);
void InitMPU6050(void);
//float Mpu6050AccelAngle(int8 dir);
float Mpu6050AccelAngle(int8 ax_dir, int8 az_dir);
float Mpu6050GyroAngle(int8 dir);



extern float Angle, Angle_dot;

#endif

