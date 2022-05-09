# Pinout
## About 
Pinout of our setup (STM32L412KBU6U microcontroller). Generated from cubesat-adcs.ioc.  
```
Warning: Pins written here are pins as stated on the microcontroller, and NOT as stated on the Nucleo board!
```

## UART1
- TX: PA9
- RX: PA10

## LED
- PB3
- TODO: add rest of the leds
	
## I2C3
- SCL: PA7
- SDA: PB4

## PWM
- TIM1_CH1: PA8

## Reaction Wheel Direction
- PA11

## Encoder
- TODO: add encoder pinout

## Debug Serial Wire
- CLK: PA14
- DIO: PA13
- These pins are reserved on the Nucleo board by itself

## High Speed Crystal
- IN: PA0
- OUT: (No out pin, idk why)
- These pins are reserved on the Nucleo board by itself
