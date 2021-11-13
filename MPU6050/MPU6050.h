#ifndef MPU_6050_
#define MPU_6050_

#include <msp430.h> 
#include <stdint.h>
#include "msp430_oled.h"

#define CPU_F ((double)16000000)
#define delayus(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))  //�궨����ʱ����
#define delayms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define SCL1 P4OUT |=BIT1       //IIC��������
#define SCL0 P4OUT &=~BIT1
#define SCLOUT P4DIR |= BIT1;


#define SDA1 P4OUT |=BIT2
#define SDA0 P4OUT &=~BIT2
#define SDAIN P4DIR &=~BIT2
#define SDAOUT P4DIR |=BIT2
#define SDADATA (P4IN & BIT2)

//********Mpu6050�����У׼ֵ**************
#define MPU6050_ZERO_ACCELL 378
#define MPU6050_ZERO_GYRO 30

//*************����MPU6050�ڲ���ַ*******************
#define  SMPLRT_DIV    0x19  //�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define CONFIG      0x1A  //��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define GYRO_CONFIG   0x1B  //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define ACCEL_CONFIG          0x1C  //���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
/***************���ٶȴ������Ĵ���******************/
#define ACCEL_XOUT_H          0x3B
#define ACCEL_XOUT_L          0x3C
#define ACCEL_XOUT      ACCEL_XOUT_H    // X���ȡ��ַ����λΪ��ʼλ
#define ACCEL_YOUT_H          0x3D
#define ACCEL_YOUT_L          0x3E
#define ACCEL_YOUT      ACCEL_YOUT_H    // Y���ȡ��ַ����λΪ��ʼλ
#define ACCEL_ZOUT_H          0x3F
#define ACCEL_ZOUT_L          0x40    
#define ACCEL_ZOUT      ACCEL_ZOUT_H    // Z���ȡ��ַ����λΪ��ʼλ
/*****************�¶ȴ������Ĵ���****************/
#define TEMP_OUT_H    0x41
#define TEMP_OUT_L    0x42    
#define TEMP_OUT          TEMP_OUT_H    // �¶ȴ�������ȡ��ַ����λΪ��ʼλ
/*****************�����ǼĴ���********************/
#define GYRO_XOUT_H   0x43
#define GYRO_XOUT_L   0x44  
#define GYRO_XOUT        GYRO_XOUT_H    // ������X���ȡ��ַ����λΪ��ʼλ
#define GYRO_YOUT_H   0x45
#define GYRO_YOUT_L   0x46
#define GYRO_YOUT        GYRO_YOUT_H    // ������Y���ȡ��ַ����λΪ��ʼλ
#define GYRO_ZOUT_H   0x47
#define GYRO_ZOUT_L   0x48 
#define GYRO_ZOUT        GYRO_ZOUT_H    // ������Z���ȡ��ַ����λΪ��ʼλ

#define PWR_MGMT_1    0x6B  //��Դ��������ֵ��0x00(��������)
#define WHO_AM_I          0x75  //IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define SlaveAddress          0xD0  //IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

typedef signed char int8;
typedef unsigned char uchar;

u8 MPU_IIC_Read_Byte(unsigned char ack);
void InitMPU6050(void);
//float Mpu6050AccelAngle(int8 dir);
float Mpu6050AccelAngle(int8 ax_dir, int8 az_dir);
float Mpu6050GyroAngle(int8 dir);



extern float Angle, Angle_dot;

#endif

