# LCD SSD1315 STM32F411CEU6

Demo project for the microcontroller STM32F411CEU6 w/ SSD1315 LCD

Driver used for SSD1315: https://github.com/STMicroelectronics/stm32-ssd1315

I2C layer for the driver is [here](./Core/Src/ssd1315_port.c)

## Build

```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake -B build/Release -G Ninja
cmake --build build/Release
```

## Flash

Assumes that the constroller is in BOOT mode on usb1 (change based on your environment)

```
sudo STM32_Programmer_CLI -c port=usb1 -w build/Release/STM32F4_SSD1315_LCD.bin 0x08000000 -v
```

## Pins

#### I2C

B6<>SCK

B7<>SDA

#### UART

A9<>TX

A10<>RX
