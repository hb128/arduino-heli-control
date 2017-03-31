import tkinter as tk
import serial

root = tk.Tk()

def motion(event):
    x, y = event.x, event.y
    print('{}, {}'.format(x, y))


ser = serial.Serial(port='/dev/ttyUSB0', baudrate=115200)  # open first serial port
print(ser.portstr)       # check which port was really used
#ser.write('000')      # write a string
ser.write(b"999999")

'''
def task():
    #print("hello")
    print(ser.read())

root.bind('<Motion>', motion)
root.after(0, task)
root.mainloop()

ser.close()             # close port
'''
