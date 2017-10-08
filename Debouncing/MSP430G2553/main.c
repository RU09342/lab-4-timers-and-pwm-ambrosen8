//MSP430G2553
#include <msp430.h>
#include <msp430G2553.h>


int num = 0;


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    //--------------------------LED---------------------------------------------
    P1DIR |= BIT0;                  //Set P1.0 (LED) as an output
    P1OUT &= ~BIT0;                 // LED off

    //-------------------------BUTTON-------------------------------------------
    P1DIR &= ~BIT3;                 //Button as input
    P1REN |= BIT3;                  //Enable the pull resistor on P1.3

    P1IE |= BIT3;                 //Enable interrupt on P1.3
    P1IES |= BIT3;                //high low
    P1IFG &= ~BIT3;               //Clear the interrupt flag register on P1.3
    P1OUT |= BIT3;
    //--------------------------TIMER----------------------------------------------
    TA0CCTL0 = CCIE;                   //Enable interrupts for the first capture/compare register.
    TA0CTL = TASSEL_2 + MC_0 + ID_3;        // SMCLK, stop, /8
    TA0CCR0 = 400;



    __bis_SR_register(GIE);         //Enter interrupt mode

}

//Interrupt vector service routine for Timer A0.
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void) {
    TA0CTL = TASSEL_2 + MC_0 + ID_3;        // SMCLK, stop, /8
    P1IE |= BIT3;
}

//Interrupt vector for button.
#pragma vector=PORT1_VECTOR         //Set the port 1 interrupt routine
__interrupt void Port_1(void) {

    P1IFG &= ~BIT3;               //P1.3 IFG cleared
    P1IE &= ~BIT3;
    TA0CTL = TASSEL_2 + MC_1 + ID_3;        // SMCLK, UP DOWN, /8
    if (num == 0)
          {
            P1OUT |= BIT0;                  //Sets LED
            num  = 1;
            P1IES &= ~BIT3;                 // Clear edge detect to rising edge
          }
    else
          {
            P1OUT &= ~BIT0;               //Resets LED
            num  = 0;
            P1IES |= BIT3;           // Set edge detect to rising edge
          }
}
