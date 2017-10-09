# Software Debouncing

## Purpose
To change the PWM of an LED with a button press on all boards.

## Theory
The pulse width modulation (PWM) is caused by changing the width of the signal sent by altering when the Timer interrupt. Since the timer interrupts when the value of the CRR# register is reached. That value of the CCR register can be changed each time a button is pressed by 10%. Then when the PWM reached 100% the PWM will then go to 0% and restart.
## Board Features
All boards work the same. When a button is pressed the Red LED will turn on/off.
