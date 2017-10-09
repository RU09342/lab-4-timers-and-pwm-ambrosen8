//MSP430FR5994
#include <msp430.h>
#include <msp430FR5994.h>


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
    PM5CTL0 &= ~LOCKLPM5;       //Stop Default High-Impedance Mode
    //---------------------------LED---------------------------------------------
        P1DIR |= BIT0;          //LED output
        P1DIR |= BIT1;
        P1OUT |= (BIT0);
        P1OUT |= BIT1;
    //-------------------------BUTTON-------------------------------------------
        P5DIR &= ~BIT6;                 //Button as input
        P5REN |= BIT6;                  //Enable the pull resistor on P1.3
        P5IE |= BIT6;                 //Enable interrupt on P1.3
        P5IES |= BIT6;                //high low
        P5IFG &= ~BIT6;               //Clear the interrupt flag register on P1.3
        P5OUT |= BIT6;
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
    #pragma vector=PORT5_VECTOR         //Set the port 1 interrupt routine
    __interrupt void Port_5(void) {

        if ((P5IN & BIT6) == 0x00) //if button pressed
        {
        P5IES &= ~BIT6; //trigger on rising edge
        P1OUT &= ~BIT1; //LED off
        }
        else //if button released
        {
        P5IES |= BIT6; //trigger on falling edge
        P1OUT |= BIT1; // LED On
        }
        if (TA0CCR1 < 1000)
        {
        TA0CCR1 += 100;
        }
        else
        {
        TA0CCR1 = 20;
        }
        P5IFG &= ~BIT6;               //P1.3 IFG cleared
        }
