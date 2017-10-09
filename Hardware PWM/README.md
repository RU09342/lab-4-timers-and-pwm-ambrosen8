# Software Debouncing

## Purpose
To change the PWM of an LED with a button press on all boards using hardware.

## Theory
The pulse width modulation (PWM) is caused by changing the width of the signal sent by altering when the Timer interrupt. The PWM relies on the CCR# causing the interrupt when the timer reaches that point. The timer is also set to output mode 7 to set and reset when this point is reached thus causing the width of the signal.
## Board Features
All boards work the same. When a button is pressed the Red LED will turn on/off.
