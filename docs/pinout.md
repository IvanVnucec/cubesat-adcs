# ADCS pinout setup

## About 
Pinout of our setup (STM32L412KBU6U microcontroller and/or NUCLEO-L412KB board).

## Warning
Pins that are not written in the parentheses are microcontroller pins, and pins written in the parentheses are pins as stated on the Nucleo board itself.  
For example, on Page 31/37, Figure 8 in [docs/um-nucleo-board.pdf](https://github.com/IvanVnucec/cubesat-adcs/blob/master/docs/um-nucleo-board.pdf) document, PA9 pin is pin on the microcontroller and at the same time pin D1 on the Nucleo board (Arduino).

## Pinout
### UART1
- TX: PA9  (D1)
- RX: PA10 (D0)

### LED
- PB3 (D13)
- TODO: add rest of the leds
	
### I2C3
- SCL: PA7 (A6)
- SDA: PB4 (D12)

### PWM
- TIM1_CH1: PA8 (D9)

### Reaction Wheel Direction
- PA11 (D10)

### Encoder
- TODO: add encoder pinout

### Debug Serial Wire
- NOTE: These pins are reserved on the microcontroller, and they are not routed on the Nucleo board (Arduino) pins.
- CLK: PA14
- DIO: PA13

### High Speed Crystal
- NOTE: These pins are reserved on the microcontroller, and they are not routed on the Nucleo board (Arduino) pins.
- IN: PA0
- OUT: (no out pin, idk why)
