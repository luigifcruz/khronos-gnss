from websocket_server import WebsocketServer
import thread
import time
import serial
import sys
import base64
import re

bytes = 0;

def printUsage():
    time.sleep(1)
    while True:
        print("[USBCONNECT] Data transferred from serial is {} bytes.".format(bytes))
        time.sleep(60)

def newClient(client, server):
    print("[USBCONNECT] New client connected!")

def newUpdate(base):
    try:
        update = base64.b64decode(base)
        server.send_message_to_all(update)
    except:
        print("[USBCONNECT] Invalid Base64. This is normal at startup.")

server = WebsocketServer(8080, host='127.0.0.1')
command = ""

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=921600
)

server.set_fn_new_client(newClient)
thread.start_new_thread(server.run_forever, ())
thread.start_new_thread(printUsage, ())

ser.isOpen()
while 1:
    line = ser.readline();
    bytes += len(line)

    if line.find("Khronos SN0001") <= 0:
        command += line.rstrip()


        if "[PAYLOAD=" and "]" in command:
            newUpdate(command[9:-1])
            command = ""
    else:
        sys.stdout.write(line)
    
