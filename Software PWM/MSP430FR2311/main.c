//MSP130FR2311
#include <msp430.h>
#include <msp430FR2311.h>

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer
    PM5CTL0 &= ~LOCKLPM5;       //Stop Default High-Impedance Mode
    //---------------------------LED---------------------------------------------
        P1DIR |= BIT0;          //LED output
        P2DIR |= BIT0;
        P1OUT |= (BIT0);
        P2OUT |= BIT0;
    //-------------------------BUTTON-------------------------------------------
        P1DIR &= ~BIT1;                 //Button as input
        P1REN |= BIT1;                  //Enable the pull resistor on P1.3
        P1IE |= BIT1;                 //Enable interrupt on P1.3
        P1IES |= BIT1;                //high low
        P1IFG &= ~BIT1;               //Clear the interrupt flag register on P1.3
        P1OUT |= BIT1;
    //-------------------------Timer A0------------------------------------------
        TB0CTL = TBSSEL_2 + MC_1;//SMCLK, UP
        TB0CCTL1 = CCIE;
        TB0CCR0 = 1000;//PWM width 1kHz
        TB0CCR1 = 500;//PWM Duty Cycle 50%
    //-------------------------Timer A1--------------------------------------------
    /*
        TB1CCTL0 = CCIE;
        TB1CTL = TBSSEL_2 + MC_0 + ID_3;        // SMCLK, stop, /8
        TB1CCR0 = 400;                //10Hz
    */
    //---------------------------Etc-------------------------------------------------
        _BIS_SR(LPM0_bits + GIE);

        }
    //-----------------Timer A0 Interrupt--------------------------------------------
    #pragma vector = TIMER0_B0_VECTOR
    __interrupt void TB0_ISR(void)
        {
        P1OUT ^= BIT0;
        TB0CCTL0 &= ~BIT0;
        }
    //-----------------Timer A1 Interrupt--------------------------------------------

    #pragma vector = TIMER0_B1_VECTOR
    __interrupt void TB1_ISR(void)
        {
        P1OUT ^= BIT0;
        TB0CCTL1 &= ~BIT0;
        }

    //-----------------------Button Interrupt---------------------------------------
    #pragma vector=PORT1_VECTOR         //Set the port 1 interrupt routine
    __interrupt void Port_1(void) {

        if ((P1IN & BIT1) == 0x00) //if button pressed
        {
        P1IES &= ~BIT1; //trigger on rising edge
        P2OUT &= ~BIT0; //LED off
        }
        else //if button released
        {
        P1IES |= BIT1; //trigger on falling edge
        P2OUT |= BIT0; // LED On
        }
        if (TB0CCR1 < 1000)
        {
        TB0CCR1 += 100;
        }
        else
        {
        TB0CCR1 = 20;
        }
        P1IFG &= ~BIT1;               //P1.3 IFG cleared
        }
