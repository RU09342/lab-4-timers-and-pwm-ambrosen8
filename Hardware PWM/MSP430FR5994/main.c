//MSP430FR5994
#include <msp430.h>
#include <msp430FR5994.h>


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
         P1DIR |= BIT1;
     //-------------------------BUTTON-------------------------------------------
         P5DIR &= ~BIT6;                 //Button as input
         P5REN |= BIT6;                  //Enable the pull resistor on P1.3
         P5IE |= BIT6;                 //Enable interrupt on P1.3
         P5IES |= BIT6;                //high low
         P5IFG &= ~BIT6;               //Clear the interrupt flag register on P1.3
         P5OUT |= BIT6;
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
     #pragma vector=PORT5_VECTOR         //Set the port 1 interrupt routine
     __interrupt void Port_5(void) {

         if((P5IN & BIT6) == BIT6)
             P1OUT &=~BIT0;
         else
             P1OUT |= BIT0;
         if (TA0CCR1 < 1000)
         {
         TA0CCR1 += 100;
         }
         else
         {
         TA0CCR1 = 20;
         }
         P1OUT ^= BIT1;
         P5IFG &= ~BIT6;               //P1.3 IFG cleared
         }
