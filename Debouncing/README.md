# Software Debouncing

## Purpose
To eliminate the button bounce. So when a button gets pressed the LED do not get switched on/off when it should not.

## Theory
When the button interrupt gets fired, the interrupt enable immediately gets switched off, not allowing the button to accidentally trigger the button switch interrupt. When the button initially fires the interrupt a timer starts. When the timer reaches its CCR value it reactivates the Interrupt enable of the button. So this allows the button to be pressed, the LED to turn on, then when the button is released the LED turns off. During this time the button does not activate thus not causing the LED to unnecessarily switch.
## Board Features
All boards work the same. When a button is pressed the Red LED will turn on/off.




## Demonstation

![alt text](https://github.com/RU09342/lab-4-timers-and-pwm-ambrosen8/blob/master/Debouncing/Assets/Debounce.gif)