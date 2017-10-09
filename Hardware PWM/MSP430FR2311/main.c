//MSP130FR2311
#include <msp430.h>
#include <msp430FR2311.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
    PM5CTL0 &= ~LOCKLPM5;       //Stop Default High-Impedance Mode
    //--------------------------Port Select-----------------------------------------
        //P1SEL &= ~BIT0;
        //P2SEL &= ~BIT0;
        //P1SEL |= BIT0;
        //P4SEL |= BIT0;

    //---------------------------LED---------------------------------------------
        P1DIR |= BIT0;          //LED output
        P2DIR |= BIT0;
    //-------------------------BUTTON-------------------------------------------
        P1DIR &= ~BIT1;                 //Button as input
        P1REN |= BIT1;                  //Enable the pull resistor on P1.3
        P1IE |= BIT1;                 //Enable interrupt on P1.3
        P1IES |= BIT1;                //high low
        P1IFG &= ~BIT1;               //Clear the interrupt flag register on P1.3
        P1OUT |= BIT1;
    //-------------------------Timer A0------------------------------------------
        TB0CTL = TBSSEL_2 + MC_1;//SMCLK, UP
        TB0CCTL0 = CCIE;
        TB0CCR0 = 1000;//PWM width 1kHz
        TB0CCTL1 = OUTMOD_7 + CCIE;
        TB0CCR1 = 500;//PWM Duty Cycle 50%

    //---------------------------Etc-------------------------------------------------
        _BIS_SR(GIE);

        }
    //-----------------Timer A0 Interrupt--------------------------------------------

    #pragma vector = TIMER0_B0_VECTOR
    __interrupt void TB0_ISR(void)
        {
        TB0IV = 0;
        }
    //-----------------Timer A1 Interrupt--------------------------------------------

    #pragma vector = TIMER0_B1_VECTOR
    __interrupt void TB1_ISR(void)
        {
    TB0IV = 0;

        }

    //-----------------------Button Interrupt---------------------------------------
    #pragma vector=PORT1_VECTOR         //Set the port 1 interrupt routine
    __interrupt void Port_1(void) {

        if((P1IN & BIT1) == BIT1)
            P1OUT &=~BIT0;
        else
            P1OUT |= BIT0;
        if (TB0CCR1 < 1000)
        {
        TB0CCR1 += 100;
        }
        else
        {
        TB0CCR1 = 20;
        }
        P2OUT ^= BIT0;
        P1IFG &= ~BIT1;               //P1.3 IFG cleared
        }
