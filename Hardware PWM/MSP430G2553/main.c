//MSP430G2553
#include <msp430.h>
#include <msp430G2553.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
//--------------------------Port Select-----------------------------------------
    P1SEL &= ~BIT0;
    P1SEL &= ~BIT1;
    P1SEL2 &= ~BIT6;
    P1SEL |= BIT6;

//---------------------------LED---------------------------------------------
    P1DIR |= BIT0;          //LED output
    P1DIR |= BIT6;
//-------------------------BUTTON-------------------------------------------
    P1DIR &= ~BIT3;                 //Button as input
    P1REN |= BIT3;                  //Enable the pull resistor on P1.3
    P1IE |= BIT3;                 //Enable interrupt on P1.3
    P1IES |= BIT3;                //high low
    P1IFG &= ~BIT3;               //Clear the interrupt flag register on P1.3
    P1OUT |= BIT3;
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

    if (TA0CCR1 < 1000)
    {
    TA0CCR1 += 100;
    }
    else
    {
    TA0CCR1 = 20;
    }
    P1OUT ^= BIT0;
    P1IFG &= ~BIT3;               //P1.3 IFG cleared
    }
