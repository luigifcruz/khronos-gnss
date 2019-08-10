import serial
import struct
from time import time
import ctypes as c


def monitor_serial():
    ser = serial.Serial(
        port='/dev/ttyUSB0',
        baudrate=115200
    )

    ser.isOpen()
    while 1:
        n = input("Send? ")

        with open("/spiffs/rx_cache/small-" + n + ".pb", "rb") as f:
            ser.write(f.read())
            ser.write(bytearray([0x1A, 0xCF, 0xFC, 0x1D]))


monitor_serial()
