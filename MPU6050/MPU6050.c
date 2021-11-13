#include "MPU6050.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define PI 3.14159


float Angle, Angle_dot; //�ⲿ��Ҫ���õı���,���ֵ����Ǽ��ٶ�ֵ

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//**************************************
//I2C��ʼ�ź�
//**************************************
void I2C_Start()
{
    SDAOUT;
    SDA1;                    //����������
    SCLOUT;
    SCL1;                    //����ʱ����
    delayus(5);                 //��ʱ
    SDA0;                    //�����½���
    delayus(5);                  //��ʱ
    SCL0;                    //����ʱ����
}
//**************************************
//I2Cֹͣ�ź�
//**************************************
void I2C_Stop()
{
    SDAOUT;
    SDA0;                    //����������
    SCL0;
    SCLOUT;
    SCL1;                    //����ʱ����
    delayus(5);                   //��ʱ
    SDA1;                    //����������
    delayus(5);                  //��ʱ
}

//**************************************
//I2C����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
//**************************************
void I2C_SendACK(uchar ack) {

    SDAOUT;
    SCLOUT;
    if(ack) SDA1;
    else SDA0;
//    SDA = ack;                  //дӦ���ź�
    SCL1;                    //����ʱ����
    delayus(5);                  //��ʱ
    SCL0;                    //����ʱ����
    delayus(5);                  //��ʱ
}

//**************************************
//I2C����Ӧ���ź�
//**************************************
uchar I2C_RecvACK()
{
    uchar cy;
    SCLOUT;
    SCL1;                    //����ʱ����
    SDAIN;
    delayus(5);                 //��ʱ
    if(SDADATA) {
        cy=1;
    }
    else{
        cy=0;
    }
//    cy = SDA;                   //��Ӧ���ź�
    SCL0;                    //����ʱ����
    delayus(5);                //��ʱ
    SDAOUT;
    return cy;
}

//**************************************
//��I2C���߷���һ���ֽ�����
//**************************************

void I2C_SendByte(uchar dat)
{
    uchar i;
    SDAOUT;
    for (i=0; i<8; i++){         //8λ������
        if((dat<<i)&0x80)
        {
            SDA1;
        }
        else
        {
            SDA0;
        }
       // SDA = cy;               //�����ݿ�
        SCLOUT;
        SCL1;                //����ʱ����
        delayus(5);              //��ʱ
        SCL0;                //����ʱ����
        delayus(5);              //��ʱ
    }
    I2C_RecvACK();
}

//**************************************
//��I2C���߽���һ���ֽ�����
//**************************************

uchar I2C_RecvByte(){

    uchar i;
    uchar dat = 0,cy;
    SDAOUT;
    SCLOUT;
    SDA1;                    //ʹ���ڲ�����,׼����ȡ����,
    SDAIN;
    for (i=0; i<8; i++)         //8λ������
    {
            dat<<= 1;
            SCL1;                //����ʱ����
            delayus(5);             //��ʱ
            if(SDADATA){
                cy=1;
            }
            else{
                cy=0;
            }
            dat |= cy;             //������         SCL0;                //����ʱ����
            delayus(5);             //��ʱ
    }
    SDAOUT;
    return dat;
}


/**
 *@д�����ֽ�
 */
void Single_WriteI2C(unsigned char Addr, unsigned char data)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    I2C_SendByte(Addr);    //�ڲ��Ĵ�����ַ��
    I2C_SendByte(data);       //�ڲ��Ĵ������ݪ�
    I2C_Stop();                   //����ֹͣ�ź�
}

uchar  Single_ReadI2C(uchar REG_Address)
{
    uchar REG_data;
    I2C_Start();                   //��ʼ�ź�
    I2C_SendByte(SlaveAddress);    //�����豸��ַ+д�ź�
    I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ����0��ʼ
    I2C_Start();                   //��ʼ�ź�
    I2C_SendByte(SlaveAddress+1);  //�����豸��ַ+���ź�
    REG_data=I2C_RecvByte();       //�����Ĵ�������
    I2C_SendACK(1);                //����Ӧ���ź�
    I2C_Stop();                    //ֹͣ�ź�
    return REG_data;
}

//**************************************
//��ʼ��MPU6050
//**************************************

void InitMPU6050(void)
{
    Single_WriteI2C(PWR_MGMT_1, 0x00);  // �������״̬
    Single_WriteI2C(SMPLRT_DIV, 0x07);  // �����ǲ��������ã�125HZ��
    Single_WriteI2C(CONFIG, 0x06);      // ��ͨ�˲���Ƶ�����ã�5HZ��
    Single_WriteI2C(GYRO_CONFIG, 0x18); // �������Լ켰��ⷶΧ����(���Լ�,16.4LSB/DBS/S)
    Single_WriteI2C(ACCEL_CONFIG, 0x01); // ���ټ��Լ졢������Χ����ͨ�˲�Ƶ��(���Լ죬2G(16384LSB/G)��5Hz)
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
        MPU_IIC_NAck();//·¢ËÍnACK
    else
        MPU_IIC_Ack(); //·¢ËÍACK
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
//�ϳ�����
//**************************************
int Get6050Data(u8 REG_Address)
{
    u8 H,L;
    H=MPU_Read_Byte(REG_Address);
    L=MPU_Read_Byte(REG_Address+1);
    return (H<<8)+L;   //�ϳ�����
}

float Mpu6050AccelAngle(int8 ax_dir, int8 az_dir)
//float Mpu6050AccelAngle(int8 dir)
{
#if 1
    float accel_agle; // ���������ֵ
    float ax_result, ax_result1; // ����ֵ�������
    float az_result, az_result1;

    ax_result = (float) Get6050Data(ax_dir); // ������ǰ����ļ��ٶ�ֵ,ת��Ϊ������
    az_result = (float) Get6050Data(az_dir);

    ax_result1 = map(ax_result, -15924, 16629, -16384, 16384);
    az_result1 = map(az_result, -17454, 15750, -16384, 16384);

    accel_agle = atan2(ax_result1, az_result1) * 180 / PI; //���ٶȼ���Ƕȡ��õ�ƽ��λ�õĽǶ�

    return accel_agle; // ���ز���ֵ

#endif
#if 0 //�÷�����׼��ԭ��δ֪
    float accel_agle; // ���������ֵ
    float result;// ����ֵ�������
    result = (float)Get6050Data(dir);// ������ǰ����ļ��ٶ�ֵ,ת��Ϊ������
    accel_agle = (result + MPU6050_ZERO_ACCELL)/16384;// ȥ�����ƫ��,����õ��Ƕȣ����ȣ�
    accel_agle = accel_agle*1.2*180/PI;//����ת��Ϊ��

    return accel_agle;// ���ز���ֵ
#endif
}
float Mpu6050GyroAngle(int8 dir)
{
    float gyro_angle, gyro_angle1;
    gyro_angle = (float) Get6050Data(dir);   // ��������ǵĵ�ǰֵ

    gyro_angle1 = -(gyro_angle - MPU6050_ZERO_GYRO) / 131.0; //ȥ�����ƫ�ƣ�������ٶ�ֵ,����Ϊ������ 
    return gyro_angle1; // ���ز���ֵ
}
