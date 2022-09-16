import serial
import time
import cv2 as cv
import math

lidar = serial.Serial('COM5', 115200)
output = cv.imread(r"output.png")


output = cv.resize(output, (1000, 1000))
angle = 0
while True:
    r = lidar.read_until()
    print (r)
    try:
        data = (str(r)[2:-5]).split()
        angle = int(data[0])
        leingh = float(data[1])

    except:
        leingh = 0

    if leingh > 10:
        x1 = 500 - leingh * math.cos(angle * 3.14 / 180)
        y1 = 500 - leingh * math.sin(angle * 3.14/ 180)
        output = cv.circle(output, (int(y1), int(x1)), 1, (255,255,255), -1)
     
    output = cv.circle(output, (500, 500), 2, (0,0,255), -1)
    cv.imshow('input2', output)
    if (angle == 359):
        cv.rectangle(output, (0,0), (1000, 1000), (0,0,0), -1)
    if cv.waitKey(1) == 27:
        break

lidar.close()
