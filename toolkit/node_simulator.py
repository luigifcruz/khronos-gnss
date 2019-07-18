import serial
import struct
from time import time
import ctypes as c


sender = bytearray(b"PU2SPY\0")
receiver = bytearray(b"PU2NVX\0")
newline = bytearray(b"\r\n")


def dummy_packet(packet):
    t = struct.pack("<i", int(time()))
    l = struct.pack("<i", int(len(packet) + 1))
    header = sender + receiver + t + l
    return header + str.encode(packet + "\0") + newline


def monitor_serial():
    ser = serial.Serial(
        port='/dev/ttyUSB2',
        baudrate=9600
    )

    ser.isOpen()
    while 1:
        p = input("Send? ")
        ser.write(dummy_packet(p))


monitor_serial()
