
#include <msp430.h>
#include <msp430FR6989.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
    PM5CTL0 &= ~LOCKLPM5;       //Stop Default High-Impedance Mode



    //---------------------------LED---------------------------------------------
        P1DIR |= BIT0;          //LED output
        P9DIR |= BIT7;
    //-------------------------BUTTON-------------------------------------------
        P1DIR &= ~BIT1;                 //Button as input
        P1REN |= BIT1;                  //Enable the pull resistor on P1.3
        P1IE |= BIT1;                 //Enable interrupt on P1.3
        P1IES |= BIT1;                //high low
        P1IFG &= ~BIT1;               //Clear the interrupt flag register on P1.3
        P1OUT |= BIT1;
    //-------------------------Timer A0------------------------------------------
        TA0CTL = TASSEL_2 + MC_1;//SMCLK, UP
        TA0CCTL0 = CCIE;
        TA0CCR0 = 1000;//PWM width 1kHz
        TA0CCTL1 = OUTMOD_7 + CCIE;
        TA0CCR1 = 500;//PWM Duty Cycle 50%

    //---------------------------Etc-------------------------------------------------
        _BIS_SR(GIE);

        }
    //-----------------Timer A0 Interrupt--------------------------------------------

    #pragma vector = TIMER0_A0_VECTOR
    __interrupt void TA0_ISR(void)
        {
        TA0IV = 0;
        }
    //-----------------Timer A1 Interrupt--------------------------------------------

    #pragma vector = TIMER0_A1_VECTOR
    __interrupt void TA1_ISR(void)
        {
    TA0IV = 0;

        }

    //-----------------------Button Interrupt---------------------------------------
    #pragma vector=PORT1_VECTOR         //Set the port 1 interrupt routine
    __interrupt void Port_1(void) {

        if((P1IN & BIT1) == BIT1)
            P9OUT &=~BIT7;
        else
            P9OUT |= BIT7;
        if (TA0CCR1 < 1000)
        {
        TA0CCR1 += 100;
        }
        else
        {
        TA0CCR1 = 20;
        }
        P1OUT ^= BIT0;
        P1IFG &= ~BIT1;               //P1.3 IFG cleared
        }
