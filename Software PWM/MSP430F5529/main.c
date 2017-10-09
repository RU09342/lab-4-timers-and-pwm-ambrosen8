//MSP430F5529
#include <msp430.h>
#include <msp430F5529.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
//---------------------------LED---------------------------------------------
    P1DIR |= BIT0;          //LED output
    P4DIR |= BIT7;
    P1OUT |= (BIT0);
    P4OUT |= BIT7;
//-------------------------BUTTON-------------------------------------------
    P2DIR &= ~BIT1;                 //Button as input
    P2REN |= BIT1;                  //Enable the pull resistor on P1.3
    P2IE |= BIT1;                 //Enable interrupt on P1.3
    P2IES |= BIT1;                //high low
    P2IFG &= ~BIT1;               //Clear the interrupt flag register on P1.3
    P2OUT |= BIT1;
//-------------------------Timer A0------------------------------------------
    TA0CTL = TASSEL_2 + MC_1;//SMCLK, UP
    TA0CCTL1 = CCIE;
    TA0CCR0 = 1000;//PWM width 1kHz
    TA0CCR1 = 500;//PWM Duty Cycle 50%
//-------------------------Timer A1--------------------------------------------
/*
    TA1CCTL0 = CCIE;
    TA1CTL = TASSEL_2 + MC_0 + ID_3;        // SMCLK, stop, /8
    TA1CCR0 = 400;                //10Hz
*/
//---------------------------Etc-------------------------------------------------
    _BIS_SR(LPM0_bits + GIE);

    }
//-----------------Timer A0 Interrupt--------------------------------------------
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_ISR(void)
    {
    P1OUT ^= BIT0;
    TA0CCTL0 &= ~BIT0;
    }
//-----------------Timer A1 Interrupt--------------------------------------------

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TA1_ISR(void)
    {
    P1OUT ^= BIT0;
    TA0CCTL1 &= ~BIT0;
    }

//-----------------------Button Interrupt---------------------------------------
#pragma vector=PORT2_VECTOR         //Set the port 1 interrupt routine
__interrupt void Port_2(void) {

    if ((P2IN & BIT1) == 0x00) //if button pressed
    {
    P2IES &= ~BIT1; //trigger on rising edge
    P4OUT &= ~BIT7; //LED off
    }
    else //if button released
    {
    P2IES |= BIT1; //trigger on falling edge
    P4OUT |= BIT7; // LED On
    }
    if (TA0CCR1 < 1000)
    {
    TA0CCR1 += 100;
    }
    else
    {
    TA0CCR1 = 20;
    }
    P2IFG &= ~BIT1;               //P1.3 IFG cleared
    }
