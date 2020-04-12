import pandas as pd
import matplotlib.pyplot as plt
import os

# """
# Format for naming log files: hand_positionMotion_repetitions_timestamp

# hand values: L or R, for left and right respectively
# position : standing (St), sitting (Si), sleeping (Sl)
# motion: variable depends on start and end of motion of hand
# repetition: a number between 00 and 99 
# time stamp: YYMMDDHHmm , two digits for year, month , day , hour, minutes

# example. left hand, standing, hands hanging on the side and ending touching face, done 5 times, on april 10th 2020 at 1:23pm

#         file name : L_St_handsHanging-Face_05_2004101323.log

# """







class log_parser():
    # def __init__(self):

    def read_data_set(self,path):
        file_name, file_extension = os.path.splitext(path)
        data = pd.read_csv(path,sep=",", header = None)
        data.columns = ["acc_x", "acc_y", "acc_z", "gyro_x", "gyro_y", "gyro_z"]
        return data

    def interactive_plot(self,data,title="something"):
        self.fig, ax = plt.subplots()
        plt.title(title)
        plt.xlabel("index")
        plt.ylabel("accelerometer data")
        # accelerometer data
        acc_x, = plt.plot(data.index,data['acc_x'],color='red',label='acc_x')
        acc_y, =plt.plot(data.index,data['acc_y'],color='blue',label='acc_y')
        acc_z, =plt.plot(data.index,data['acc_z'],color='green',label='acc_z')

        # groscope data
        gyro_x, =plt.plot(data.index,data['gyro_x'],color='black',label='gyro_x')
        gyro_y, =plt.plot(data.index,data['gyro_y'],color='purple',label='gyro_y')
        gyro_z, =plt.plot(data.index,data['gyro_z'],color='brown',label='gyro_z')

        #adding a legend for every data set
        leg = ax.legend(loc='upper left', fancybox=True, shadow=True)
        leg.get_frame().set_alpha(0.4)

        # we will set up a dict mapping legend line to orig line, and enable
        # picking on the legend line
        lines = [acc_x, acc_y,acc_z,gyro_x,gyro_y,gyro_z]
        self.lined = dict()
        for legline, origline in zip(leg.get_lines(), lines):
            legline.set_picker(5)  # 5 pts tolerance
            self.lined[legline] = origline
        self.fig.canvas.mpl_connect('pick_event', self.onpick)

        plt.show()

    def onpick(self,event):
        # on the pick event, find the orig line corresponding to the
        # legend proxy line, and toggle the visibility
        legline = event.artist
        origline = self.lined[legline]
        vis = not origline.get_visible()
        origline.set_visible(vis)
        # Change the alpha on the line in the legend so we can see what lines
        # have been toggled
        if vis:
            legline.set_alpha(1.0)
        else:
            legline.set_alpha(0.2)
        self.fig.canvas.draw()

    # def data_set_subtraction(self,data1,data2):




if __name__=="__main__":
    PATH = "peymans_log/"
    idle_file_name = PATH+"L_St_idle_00_2004101515.log"
    idle_fac_file_name = PATH+"L_St_idle-face_05_2004101950.log"
    data_parser = log_parser()

    idle_data = data_parser.read_data_set(idle_file_name)
    idle_face_data = data_parser.read_data_set(idle_fac_file_name)
    subtracted_data = idle_face_data.subtract(idle_data) 
    print(subtracted_data)
    data_parser.interactive_plot(subtracted_data)