import sys
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5 import QtCore, QtWidgets
import pyqtgraph as pg
import numpy as np

from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg

import collections
import random
import time
import math
import numpy as np

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib.figure import Figure

import serial
import time

from pyqtgraph.Qt import QtGui, QtCore


import collections
import random
import math


class MyApp():
    def __init__(self):
        self.init_control_window()
        self.init_plot_window()
        self.init_serial()

        self.est_dev = 50 * [10]

    def init_control_window(self):
        self.control_win = QMainWindow()
        self.control_win.setWindowTitle("Telemetry Controls")

        self.control_win.OptionsWidget = QWidget(self.control_win)

        self.control_win.OptionsWidget.toggle_tel = QPushButton(
            'Toggle Telemetry')
        self.control_win.OptionsWidget.toggle_tel.clicked.connect(
            self.send_t)
        self.control_win.OptionsWidget.toggle_tel.resize(300, 200)

        self.control_win.OptionsWidget.en_cont = QPushButton(
            'Enable Controller')
        self.control_win.OptionsWidget.en_cont.clicked.connect(
            self.send_c)
        self.control_win.OptionsWidget.en_cont.resize(300, 200)

        self.control_win.OptionsWidget.en_mot = QPushButton('Enable Motor')
        self.control_win.OptionsWidget.en_mot.clicked.connect(
            self.send_m)
        self.control_win.OptionsWidget.en_mot.resize(300, 200)

        self.control_win.OptionsWidget.dis_mot = QPushButton('Disable Motor')
        self.control_win.OptionsWidget.dis_mot.clicked.connect(
            self.send_space)
        self.control_win.OptionsWidget.dis_mot.resize(300, 200)

        # set options VBox
        options_layout = QVBoxLayout()
        options_layout.addWidget(self.control_win.OptionsWidget.toggle_tel)
        options_layout.addWidget(self.control_win.OptionsWidget.en_cont)
        options_layout.addWidget(self.control_win.OptionsWidget.en_mot)
        options_layout.addWidget(self.control_win.OptionsWidget.dis_mot)
        self.control_win.OptionsWidget.setLayout(options_layout)

        self.control_win.MainGrid = QGridLayout()
        self.control_win.MainGrid.addWidget(self.control_win.OptionsWidget)

        self.control_win.MainWidget = QWidget(self.control_win)
        self.control_win.MainWidget.setLayout(self.control_win.MainGrid)
        self.control_win.setCentralWidget(self.control_win.MainWidget)
        self.control_win.show()

    def init_plot_window(self):
        self.plot_win = pg.GraphicsLayoutWidget()
        self.plot_win.resize(1000, 600)
        self.plot_win.setWindowTitle('Plots')

        pg.setConfigOptions(antialias=True)
        self.est_dev_plot = self.plot_win.addPlot(title="Estimated Deviation")
        self.est_dev_curve = self.est_dev_plot.plot(pen='r')

        # move this probably
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.update_plot)
        self.timer.timeout.connect(self.recv)
        self.timer.start(50)

        self.plot_win.show()

    def init_serial(self):
        addr = "/dev/rfcomm0"
        baud = 9600

        # self.ser = serial.Serial(port=addr, baudrate=baud,
        #                     bytesize=serial.EIGHTBITS,
        #                     parity=serial.PARITY_NONE,
        #                     stopbits=serial.STOPBITS_ONE)
        # self.ser.flushOutput()

    def recv(self):
        # if self.ser.readable():
        # read_data = self.ser.readline()
        # print(read_data)
        print("recv called")

    def send(self, msg):
        # self.ser.write(msg.encode())
        print("\nmsg: {}\n".format(msg.encode()))

    def send_c(self):
        self.send('c')

    def send_m(self):
        self.send('m')

    def send_space(self):
        self.send(' ')

    def send_t(self):
        self.send('t')

    def update_plot(self):
        self.est_dev_curve.setData(self.est_dev)


if __name__ == '__main__':

    app = QApplication(sys.argv)

    myapp = MyApp()

    sys.exit(app.exec_())
