all: build flash

build:
	mkspiffs -c files/ -b 4096 -p 256 -s 0x200000 spiffs.bin

flash:
	esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 921600 write_flash -z 0x110000 spiffs.bin 