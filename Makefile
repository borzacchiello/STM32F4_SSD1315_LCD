build:
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake -B build/Release -G Ninja
	cmake --build build/Release

flash:
	sudo STM32_Programmer_CLI -c port=usb1 -w build/Release/STM32F4_SSD1315_LCD.bin 0x08000000 -v

clean:
	rm -rf build
