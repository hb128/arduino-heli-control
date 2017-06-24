#!/usr/bin/env python3
import tkinter as tk
import serial

root = tk.Tk()
root.resizable(width=False, height=False)
width = 1080
factor = width/35
root.geometry('{}x{}'.format(width, width))
ser = serial.Serial(port='/dev/ttyUSB0', baudrate=115200)

x = 17
y = 17

def motion(event):
    global x
    global y
    x, y = event.x, event.y
    x = x
    y = y
    x = int(x / factor)
    y = int(y / factor)
    print('{}, {}'.format(x, y))

def receiving():
    bytesToRead = ser.inWaiting()
    mes = ser.read(bytesToRead)
    if mes:
        print(mes.decode("utf-8"))
    root.after(50, receiving)

def sending():
    turnString = "<T," + str(x) + ",0.0>"
    ser.write(bytes(turnString.encode("UTF-8")))
    fwdString = "<F," + str(35-y) + ",0.0>"
    ser.write(bytes(fwdString.encode("UTF-8")))
    root.after(200, sending)

root.bind('<Motion>', motion)
root.after(0, receiving)
root.after(0, sending)
root.mainloop()
ser.close()
