//MSP430F5529
#include <msp430.h>
#include <msp430F5529.h>


int num = 0;
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
    //--------------------------LED---------------------------------------------
       P1DIR |= BIT0;                  //Set P1.0 (LED) as an output
       P1OUT &= ~BIT0;                 // LED off

       //-------------------------BUTTON-------------------------------------------
       P2DIR &= ~BIT1;                 //Button as input
       P2REN |= BIT1;                  //Enable the pull resistor

       P2IE |= BIT1;                 //Enable interrupt
       P2IES |= BIT1;                //high low
       P2IFG &= ~BIT1;               //Clear the interrupt flag register
       P2OUT |= BIT1;
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
       P2IE |= BIT1;
   }

   //Interrupt vector for button.
   #pragma vector=PORT2_VECTOR         //Set interrupt routine
   __interrupt void Port_2(void) {

       P2IFG &= ~BIT1;               //P1.3 IFG cleared
       P2IE &= ~BIT1;
       TA0CTL = TASSEL_2 + MC_1 + ID_3;        // SMCLK, UP DOWN, /8
       if (num == 0)
             {
               P1OUT |= BIT0;                  //Sets LED
               num  = 1;
               P2IES &= ~BIT1;                 // Clear edge detect to rising edge
             }
       else
             {
               P1OUT &= ~BIT0;               //Resets LED
               num  = 0;
               P2IES |= BIT1;           // Set edge detect to rising edge
             }
   }
