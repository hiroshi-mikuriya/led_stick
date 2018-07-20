import FaBo9Axis_MPU9250
import time

mpu9250 = FaBo9Axis_MPU9250.MPU9250()

while True:
  accel = mpu9250.readAccel()
  print(str(accel))
  time.sleep(0.1)
