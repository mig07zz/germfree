import pandas as pd
import matplotlib.pyplot as plt
import os


def onpick(event):
    # on the pick event, find the orig line corresponding to the
    # legend proxy line, and toggle the visibility
    legline = event.artist
    origline = lined[legline]
    vis = not origline.get_visible()
    origline.set_visible(vis)
    # Change the alpha on the line in the legend so we can see what lines
    # have been toggled
    if vis:
        legline.set_alpha(1.0)
    else:
        legline.set_alpha(0.2)
    fig.canvas.draw()


PATH = "peymans_log/"
file_name = PATH+"L_St_idle-face_00_2004101515.log"
print("got the file")
filename, file_extension = os.path.splitext(file_name)
print("read the title")
data = pd.read_csv(file_name,sep=",", header = None)
print ("parsed as .csv filessss")
data.columns = ["acc_x", "acc_y", "acc_z", "gyro_x", "gyro_y", "gyro_z"]

print("make it till line its time to print")
fig, ax = plt.subplots()
plt.title(filename)
plt.xlabel("index")
plt.ylabel("accelerometer data")
acc_x, = plt.plot(data.index,data['acc_x'],color='red',label='acc_x')
acc_y, =plt.plot(data.index,data['acc_y'],color='blue',label='acc_y')
acc_z, =plt.plot(data.index,data['acc_z'],color='green',label='acc_z')
# plt.legend()

# fig, ax = plt.subplots()
# plt.title(filename)
# plt.xlabel("index")
# plt.ylabel("Gyroscope data")
gyro_x, =plt.plot(data.index,data['gyro_x'],color='black',label='gyro_x')
gyro_y, =plt.plot(data.index,data['gyro_y'],color='purple',label='gyro_y')
gyro_z, =plt.plot(data.index,data['gyro_z'],color='brown',label='gyro_z')

leg = ax.legend(loc='upper left', fancybox=True, shadow=True)
leg.get_frame().set_alpha(0.4)
# plt.legend()

# plt.show()


# import numpy as np
# import matplotlib.pyplot as plt

# t = np.arange(0.0, 0.2, 0.1)
# y1 = 2*np.sin(2*np.pi*t)
# y2 = 4*np.sin(2*np.pi*2*t)

# fig, ax = plt.subplots()
# ax.set_title('Click on legend line to toggle line on/off')
# line1, = ax.plot(t, y1, lw=2, label='1 HZ')
# line2, = ax.plot(t, y2, lw=2, label='2 HZ')
# leg = ax.legend(loc='upper left', fancybox=True, shadow=True)
# leg.get_frame().set_alpha(0.4)


# we will set up a dict mapping legend line to orig line, and enable
# picking on the legend line
lines = [acc_x, acc_y,acc_z,gyro_x,gyro_y,gyro_z]
lined = dict()
for legline, origline in zip(leg.get_lines(), lines):
    legline.set_picker(5)  # 5 pts tolerance
    lined[legline] = origline


# def onpick(event):
#     # on the pick event, find the orig line corresponding to the
#     # legend proxy line, and toggle the visibility
#     legline = event.artist
#     origline = lined[legline]
#     vis = not origline.get_visible()
#     origline.set_visible(vis)
#     # Change the alpha on the line in the legend so we can see what lines
#     # have been toggled
#     if vis:
#         legline.set_alpha(1.0)
#     else:
#         legline.set_alpha(0.2)
#     fig.canvas.draw()

fig.canvas.mpl_connect('pick_event', onpick)

plt.show()