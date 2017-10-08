//MSP430FR2311
#include <msp430.h>
#include <msp430FR2311.h>

int num =0;


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
    PM5CTL0 &= ~LOCKLPM5;       //Stop Default High-Impedance Mode

    //--------------------------LED---------------------------------------------
          P1DIR |= BIT0;                  //Set P1.0 (LED) as an output
          P1OUT &= ~BIT0;                 // LED off

          //-------------------------BUTTON-------------------------------------------
          P1DIR &= ~BIT1;                 //Button as input
          P1REN |= BIT1;                  //Enable the pull resistor

          P1IE |= BIT1;                 //Enable interrupt
          P1IES |= BIT1;                //high low
          P1IFG &= ~BIT1;               //Clear the interrupt flag register
          P1OUT |= BIT1;
          //--------------------------TIMER----------------------------------------------
          TB0CCTL0 = CCIE;
          TB0CTL = TBSSEL_2 + MC_0 + ID_3;//SMCLK/8, UP
          TB0CCR0 = 400;               // 100 Hz divided by the frequency


          _BIS_SR(GIE);


}
//---------------------------Interrupts---------------------------------------
#pragma vector = TIMER0_B0_VECTOR           //Timer counts
__interrupt void TB1_ISR(void)
{
    TB0CTL = TBSSEL_2 + MC_0 + ID_3;        // SMCLK, stop, /8
    P1IE |= BIT1;
}

//Interrupt vector for button.
#pragma vector=PORT1_VECTOR         //Set interrupt routine
__interrupt void Port_1(void) {

    P1IFG &= ~BIT1;               //P1.3 IFG cleared
    P1IE &= ~BIT1;
    TB0CTL = TBSSEL_2 + MC_1 + ID_3;        // SMCLK, UP DOWN, /8
    if (num == 0)
          {
            P1OUT |= BIT0;                  //Sets LED
            num  = 1;
            P1IES &= ~BIT1;                 // Clear edge detect to rising edge
          }
    else
          {
            P1OUT &= ~BIT0;               //Resets LED
            num  = 0;
            P1IES |= BIT1;           // Set edge detect to rising edge
          }
}
