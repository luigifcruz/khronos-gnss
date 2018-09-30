from websocket_server import WebsocketServer
import thread
import time
import serial
import sys
import base64
import re

def newClient(client, server):
    print("[USB-CONECT] New client connected!")

def newUpdate(base):
    update = base64.b64decode(base)
    server.send_message_to_all(update)

server = WebsocketServer(8080, host='127.0.0.1')
command = ""

ser = serial.Serial(
    port='/dev/tty.SLAB_USBtoUART',
    baudrate=921600
)

server.set_fn_new_client(newClient)
thread.start_new_thread(server.run_forever, ())

ser.isOpen()
while 1:
    line = ser.readline();

    if line.find("Khronos SN0001") <= 0:
        command += line.rstrip()

        if "[PAYLOAD=" and "]" in command:
            newUpdate(command[9:-1])
            command = ""
    else:
        sys.stdout.write(line)
    