#include "time.h"
int count_time, count_time1, count_time2;

/*
 * MC1:��ģʽ����ʱ��������TACCR0
 * TASSEL_2:ϵͳ��ʱ��
 * TACLR����ʱ������λ
 * ta0ccr0 = SMCLK/200hz(5ms)/2
 40000 = 16000 000 /200/2

 */
void time0InterInit(void)
{

    TA0CTL |= MC_1 + TASSEL_2 + TACLR;       //ʱ��ΪSMCLK,�Ƚ�ģʽ����ʼʱ���������
    TA0CCTL0 = CCIE;            //�Ƚ����ж�ʹ��
    TA0CCR0 = 40000;           //�жϣ����ڣ�5ms
}

/************************��ʱ���жϺ���********************************/
#if 1

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    //static int count_time,count_time1;
    //TA0CCTL0 = 0; 

    count_time++;

    count_time1++;

    count_time2++;

    if (count_time1 == 50){
        Angle = Mpu6050AccelAngle(ACCEL_XOUT,ACCEL_ZOUT);
        Angle_dot =  Mpu6050GyroAngle(GYRO_YOUT);
        count_time1 = 0;
    }


//    pwm_calculate();//PWM�������
    /*
     if(++count_time >=6)
     {
     count_time=0;
     //Kalman_Filter(angle[0],w[0]);
     P4OUT ^= BIT7;              //�γ�����Ч��
     //OledDisApp();
     Psn_Calcu();   //���ת�٣�λ��
     }
     if(++count_time1 >=5)
     {
     count_time1 = 0;
     pwm_calculate();//PWM�������
     }
     */


    // TA0CCTL0 = CCIE; 
}
#endif
