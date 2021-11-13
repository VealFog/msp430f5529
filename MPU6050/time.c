#include "time.h"
int count_time, count_time1, count_time2;

/*
 * MC1:增模式，定时器计数到TACCR0
 * TASSEL_2:系统主时钟
 * TACLR：定时器清零位
 * ta0ccr0 = SMCLK/200hz(5ms)/2
 40000 = 16000 000 /200/2

 */
void time0InterInit(void)
{

    TA0CTL |= MC_1 + TASSEL_2 + TACLR;       //时钟为SMCLK,比较模式，开始时清零计数器
    TA0CCTL0 = CCIE;            //比较器中断使能
    TA0CCR0 = 40000;           //中断（周期）5ms
}

/************************定时器中断函数********************************/
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


//    pwm_calculate();//PWM计算输出
    /*
     if(++count_time >=6)
     {
     count_time=0;
     //Kalman_Filter(angle[0],w[0]);
     P4OUT ^= BIT7;              //形成闪灯效果
     //OledDisApp();
     Psn_Calcu();   //电机转速，位移
     }
     if(++count_time1 >=5)
     {
     count_time1 = 0;
     pwm_calculate();//PWM计算输出
     }
     */


    // TA0CCTL0 = CCIE; 
}
#endif
