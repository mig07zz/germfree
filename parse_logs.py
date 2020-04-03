import pandas as pd
import matplotlib.pyplot as plt
import os

file_name = "relaxing_sitting.log"
filename, file_extension = os.path.splitext(file_name)
data = pd.read_csv(file_name,sep=",", header = None)
data.columns = ["acc_x", "acc_y", "acc_z", "gyro_x", "gyro_y", "gyro_z"]


figure = plt.figure()
plt.title(filename)
plt.xlabel("index")
plt.ylabel("accelerometer data")
plt.plot(data.index,data['acc_x'],color='red',label='acc_x')
plt.plot(data.index,data['acc_y'],color='blue',label='acc_y')
plt.plot(data.index,data['acc_z'],color='green',label='acc_z')
plt.legend()

figure = plt.figure(2)
plt.title(filename)
plt.xlabel("index")
plt.ylabel("Gyroscope data")
plt.plot(data.index,data['gyro_x'],color='red',label='gyro_x')
plt.plot(data.index,data['gyro_y'],color='green',label='gyro_y')
plt.plot(data.index,data['gyro_z'],color='blue',label='gyro_z')
plt.legend()

plt.show()