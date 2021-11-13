#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#include "MPU6050.h"
#include "mathfun.h"
#include "msp430_oled.h"
#include <time.h>
#include "uart.h"
//void OledDisApp(void);

void delay(unsigned int z)
{
  unsigned int x,y;
  for(x=z;x>0;x--)
    for(y=5000;y>0;y--);
}


//******************************************************************************
// Device Initialization *******************************************************
//******************************************************************************

void initClockTo16MHz()
{
    UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
    UCSCTL4 |= SELA_2;                        // Set ACLK = REFO
    __bis_SR_register(SCG0);                  // Disable the FLL control loop
    UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
    UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
    UCSCTL2 = FLLD_0 + 487;                   // Set DCO Multiplier for 16MHz
                                              // (N + 1) * FLLRef = Fdco
                                              // (487 + 1) * 32768 = 16MHz
                                              // Set FLL Div = fDCOCLK
    __bic_SR_register(SCG0);                  // Enable the FLL control loop

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32 x 32 x 16 MHz / 32,768 Hz = 500000 = MCLK cycles for DCO to settle
    __delay_cycles(500000);//
    // Loop until XT1,XT2 & DCO fault flag is cleared
    do
    {
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG); // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                          // Clear fault flags
    }while (SFRIFG1&OFIFG);                         // Test oscillator fault flag
}
uint16_t setVCoreUp(uint8_t level){
    uint32_t PMMRIE_backup, SVSMHCTL_backup, SVSMLCTL_backup;

    //The code flow for increasing the Vcore has been altered to work around
    //the erratum FLASH37.
    //Please refer to the Errata sheet to know if a specific device is affected
    //DO NOT ALTER THIS FUNCTION

    //Open PMM registers for write access
    PMMCTL0_H = 0xA5;

    //Disable dedicated Interrupts
    //Backup all registers
    PMMRIE_backup = PMMRIE;
    PMMRIE &= ~(SVMHVLRPE | SVSHPE | SVMLVLRPE |
                SVSLPE | SVMHVLRIE | SVMHIE |
                SVSMHDLYIE | SVMLVLRIE | SVMLIE |
                SVSMLDLYIE
                );
    SVSMHCTL_backup = SVSMHCTL;
    SVSMLCTL_backup = SVSMLCTL;

    //Clear flags
    PMMIFG = 0;

    //Set SVM highside to new level and check if a VCore increase is possible
    SVSMHCTL = SVMHE | SVSHE | (SVSMHRRL0 * level);

    //Wait until SVM highside is settled
    while((PMMIFG & SVSMHDLYIFG) == 0)
    {
        ;
    }

    //Clear flag
    PMMIFG &= ~SVSMHDLYIFG;

    //Check if a VCore increase is possible
    if((PMMIFG & SVMHIFG) == SVMHIFG)
    {
        //-> Vcc is too low for a Vcore increase
        //recover the previous settings
        PMMIFG &= ~SVSMHDLYIFG;
        SVSMHCTL = SVSMHCTL_backup;

        //Wait until SVM highside is settled
        while((PMMIFG & SVSMHDLYIFG) == 0)
        {
            ;
        }

        //Clear all Flags
        PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG |
                     SVMLVLRIFG | SVMLIFG |
                     SVSMLDLYIFG
                     );

        //Restore PMM interrupt enable register
        PMMRIE = PMMRIE_backup;
        //Lock PMM registers for write access
        PMMCTL0_H = 0x00;
        //return: voltage not set
        return false;
    }

    //Set also SVS highside to new level
    //Vcc is high enough for a Vcore increase
    SVSMHCTL |= (SVSHRVL0 * level);

    //Wait until SVM highside is settled
    while((PMMIFG & SVSMHDLYIFG) == 0)
    {
        ;
    }

    //Clear flag
    PMMIFG &= ~SVSMHDLYIFG;

    //Set VCore to new level
    PMMCTL0_L = PMMCOREV0 * level;

    //Set SVM, SVS low side to new level
    SVSMLCTL = SVMLE | (SVSMLRRL0 * level) |
               SVSLE | (SVSLRVL0 * level);

    //Wait until SVM, SVS low side is settled
    while((PMMIFG & SVSMLDLYIFG) == 0)
    {
        ;
    }

    //Clear flag
    PMMIFG &= ~SVSMLDLYIFG;
    //SVS, SVM core and high side are now set to protect for the new core level

    //Restore Low side settings
    //Clear all other bits _except_ level settings
    SVSMLCTL &= (SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 +
                 SVSMLRRL1 + SVSMLRRL2
                 );

    //Clear level settings in the backup register,keep all other bits
    SVSMLCTL_backup &=
        ~(SVSLRVL0 + SVSLRVL1 + SVSMLRRL0 + SVSMLRRL1 + SVSMLRRL2);

    //Restore low-side SVS monitor settings
    SVSMLCTL |= SVSMLCTL_backup;

    //Restore High side settings
    //Clear all other bits except level settings
    SVSMHCTL &= (SVSHRVL0 + SVSHRVL1 +
                 SVSMHRRL0 + SVSMHRRL1 +
                 SVSMHRRL2
                 );

    //Clear level settings in the backup register,keep all other bits
    SVSMHCTL_backup &=
        ~(SVSHRVL0 + SVSHRVL1 + SVSMHRRL0 + SVSMHRRL1 + SVSMHRRL2);

    //Restore backup
    SVSMHCTL |= SVSMHCTL_backup;

    //Wait until high side, low side settled
    while(((PMMIFG & SVSMLDLYIFG) == 0) &&
          ((PMMIFG & SVSMHDLYIFG) == 0))
    {
        ;
    }

    //Clear all Flags
    PMMIFG &= ~(SVMHVLRIFG | SVMHIFG | SVSMHDLYIFG |
                SVMLVLRIFG | SVMLIFG | SVSMLDLYIFG
                );

    //Restore PMM interrupt enable register
    PMMRIE = PMMRIE_backup;

    //Lock PMM registers for write access
    PMMCTL0_H = 0x00;

    return true;
}
bool increaseVCoreToLevel2()
{
    uint8_t level = 2;
    uint8_t actlevel;
    bool status = true;

    //Set Mask for Max. level
    level &= PMMCOREV_3;

    //Get actual VCore
    actlevel = PMMCTL0 & PMMCOREV_3;

    //step by step increase or decrease
    while((level != actlevel) && (status == true))
    {
        if(level > actlevel)
        {
            status = setVCoreUp(++actlevel);
        }
    }

    return (status);
}

void initGPIO()
{

    P1DIR |=BIT0;
    P4DIR |=BIT7;
    //按键输入
    P2DIR &=~BIT1;
    P1DIR &=~BIT1;

    P2REN |= BIT1;//使能上下拉
    P2OUT |= BIT1;//上拉

    P1REN |= BIT1;//使能上下拉
    P1OUT |= BIT1;//上拉


}


int main(void) {

    WDTCTL = WDTPW | WDTHOLD;
    increaseVCoreToLevel2();
    initClockTo16MHz();                        //配置系统时钟为16Mhz
    UART_Init('n',8,1);
    initGPIO();
    InitMPU6050();
    OLED_Init();
    time0InterInit();
    _EINT();//开启总中断
    while(1){

        OLED_ShowNum(12,12,Angle,3,12);

    }

}
