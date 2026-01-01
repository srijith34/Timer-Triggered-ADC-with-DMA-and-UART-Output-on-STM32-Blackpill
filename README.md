# STM32 ADC Triggered by Timer using DMA and UART

## Description
- TIM2 generates a 100 Hz trigger
- ADC1 conversion starts on timer trigger
- ADC data transferred via DMA
- DMA completion interrupt sends data over UART
- Fully bare-metal implementation (no HAL)

## Hardware
- STM32 Blackpill (STM32F401)
- ADC input: PA0
- UART2 TX: PA2
- Baud rate: 115200

## Output
ADC = xxxx (printed every 10 ms)

## Tools
- arm-none-eabi-gcc
- OpenOCD / ST-Link
- PuTTY for serial output
