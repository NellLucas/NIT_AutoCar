#Gamepad.py
from inputs import get_gamepad
import math
import threading
import time

import socket

server_ip = '192.168.146.248' 
server_port = 25565

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.connect((server_ip, server_port))

class XboxController(object):
    MAX_TRIG_VAL = math.pow(2, 8)
    MAX_JOY_VAL = math.pow(2, 15)

    def __init__(self):

        self.LeftJoystickY = 0
        self.LeftJoystickX = 0
        self.RightJoystickY = 0
        self.RightJoystickX = 0
        self.LeftTrigger = 0
        self.RightTrigger = 0
        self.LeftBumper = 0
        self.RightBumper = 0
        self.A = 0
        self.X = 0
        self.Y = 0
        self.B = 0
        self.LeftThumb = 0
        self.RightThumb = 0
        self.Back = 0
        self.Start = 0
        self.LeftDPad = 0
        self.RightDPad = 0
        self.UpDPad = 0
        self.DownDPad = 0

        self._monitor_thread = threading.Thread(target=self._monitor_controller, args=())
        self._monitor_thread.daemon = True
        self._monitor_thread.start()


    def read(self): 
        degree = self.LeftJoystickX
        accel = self.LeftJoystickY
        bw = self.RightBumper
        fw = self.LeftBumper
        stop1 = self.RightTrigger
        stop2 = self.LeftTrigger

        auto = self.Start
        hand = self.Back
        return [degree, bw, fw, accel, stop1, stop2, hand, auto]


    def _monitor_controller(self):
        while True:
            events = get_gamepad()
            for event in events:
                if event.code == 'ABS_Y':
                    self.LeftJoystickY = event.state / XboxController.MAX_JOY_VAL # normalize between -1 and 1
                elif event.code == 'ABS_X':
                    self.LeftJoystickX = event.state / XboxController.MAX_JOY_VAL # normalize between -1 and 1
                elif event.code == 'ABS_RY':
                    self.RightJoystickY = event.state / XboxController.MAX_JOY_VAL # normalize between -1 and 1
                elif event.code == 'ABS_RX':
                    self.RightJoystickX = event.state / XboxController.MAX_JOY_VAL # normalize between -1 and 1
                elif event.code == 'ABS_Z':
                    self.LeftTrigger = event.state / XboxController.MAX_TRIG_VAL # normalize between 0 and 1
                elif event.code == 'ABS_RZ':
                    self.RightTrigger = event.state / XboxController.MAX_TRIG_VAL # normalize between 0 and 1
                elif event.code == 'BTN_TL':
                    self.LeftBumper = event.state
                elif event.code == 'BTN_TR':
                    self.RightBumper = event.state
                elif event.code == 'BTN_SOUTH':
                    self.A = event.state
                elif event.code == 'BTN_NORTH':
                    self.Y = event.state #previously switched with X
                elif event.code == 'BTN_WEST':
                    self.X = event.state #previously switched with Y
                elif event.code == 'BTN_EAST':
                    self.B = event.state
                elif event.code == 'BTN_THUMBL':
                    self.LeftThumb = event.state
                elif event.code == 'BTN_THUMBR':
                    self.RightThumb = event.state
                elif event.code == 'BTN_SELECT':
                    self.Back = event.state
                elif event.code == 'BTN_START':
                    self.Start = event.state
                elif event.code == 'BTN_TRIGGER_HAPPY1':
                    self.LeftDPad = event.state
                elif event.code == 'BTN_TRIGGER_HAPPY2':
                    self.RightDPad = event.state
                elif event.code == 'BTN_TRIGGER_HAPPY3':
                    self.UpDPad = event.state
                elif event.code == 'BTN_TRIGGER_HAPPY4':
                    self.DownDPad = event.state


control = XboxController()
gear = 0
mode = 1
speed=0
while True:
    value = control.read()
    handle = round(value[0], 4)
    back = value[1]
    forward = value[2]
    accel = round(value[3], 4)
    stop1 = value[4]
    stop2 = value[5]
    hand = value[6]
    auto = value[7]

    if -0.05<accel and accel<0.05:
        accel=0
    if -0.05<handle and handle<0.05:
        handle=0
            
    if back==1:
        gear = -1
    if forward==1:
        gear = 1
    if stop1>=0.5 or stop2>=0.5:
        gear = 0

    if hand==1:
        mode=1
    if auto==1:
        mode=2

    if gear==-1:
        speed = speed - accel
        if speed<-100:
            speed=-100
        if speed>0:
            speed=0
    if gear==1:
        speed = speed + accel
        if speed>100:
            speed=100
        if speed<0:
            speed=0

    data = str(mode)+'%05d' % ((handle*10000)+10000)+'%03d' % (speed+100)
    
    msg = data
    socket.sendall(msg.encode(encoding='utf-8'))
    dt = socket.recv(1024)
    msg = dt.decode() 
    #print('echo msg:', msg)
    
    if msg == '/end':
        break

    #print(gear, handle, accel, mode, speed, data)
    print(data)
    time.sleep(0.05)

socket.close()