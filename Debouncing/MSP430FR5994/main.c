//MSP430FR5994
#include <msp430.h>
#include <msp430FR5994.h>


int num = 0;


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
    PM5CTL0 &= ~LOCKLPM5;       //Stop Default High-Impedance Mode
    //--------------------------LED---------------------------------------------
       P1DIR |= BIT0;                  //Set P1.0 (LED) as an output
       P1OUT &= ~BIT0;                 // LED off

       //-------------------------BUTTON-------------------------------------------
       P5DIR &= ~BIT6;                 //Button as input
       P5REN |= BIT6;                  //Enable the pull resistor

       P5IE |= BIT6;                 //Enable interrupt
       P5IES |= BIT6;                //high low
       P5IFG &= ~BIT6;               //Clear the interrupt flag register
       P5OUT |= BIT6;
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
       P5IE |= BIT6;
   }

   //Interrupt vector for button.
   #pragma vector=PORT5_VECTOR         //Set interrupt routine
   __interrupt void Port_5(void) {

       P5IFG &= ~BIT6;               //P1.3 IFG cleared
       P5IE &= ~BIT6;
       TA0CTL = TASSEL_2 + MC_1 + ID_3;        // SMCLK, UP DOWN, /8
       if (num == 0)
             {
               P1OUT |= BIT0;                  //Sets LED
               num  = 1;
               P5IES &= ~BIT6;                 // Clear edge detect to rising edge
             }
       else
             {
               P1OUT &= ~BIT0;               //Resets LED
               num  = 0;
               P5IES |= BIT6;           // Set edge detect to rising edge
             }
   }
