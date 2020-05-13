# python 3.7 conversion July 28, 2019
# coding: utf-8

# # EECS192 Spring 2018 Track Finding from 1D line sensor data

# In[6]:

# changed to use 8 bit compressed line sensor values
# data format: 128 comma separated values, last value in line has space, not comma
# line samples are about 10 ms apart
#  csv file format time in ms, 128 byte array, velocity
# note AGC has already been applied to data, and camera has been calibrated for illumination effects


# In[8]:

import csv
import numpy as np
# import scipy as sp
import matplotlib.pyplot as plt

# import scipy.ndimage as ndi  # useful for 1d filtering functions
plt.close("all")   # try to close all open figs

# In[9]:

# Graphing helper function


def setup_graph(title='', x_label='', y_label='', fig_size=None):
    fig = plt.figure()
    if fig_size != None:
        fig.set_size_inches(fig_size[0], fig_size[1])
    ax = fig.add_subplot(111)
    ax.set_title(title)
    ax.set_xlabel(x_label)
    ax.set_ylabel(y_label)


# Line scan plotting function.
#

# In[10]:

def plot_frame(linearray):
    nframes = np.size(linearray)/128
    n = range(0, 128)
    print('number of frames', nframes)
    print('size of line', np.size(linearray[0, :]))
    for i in range(0, nframes-1):
        setup_graph(title='$x[n]$', x_label='$n$',
                    y_label='row'+str(i)+' $ xa[n]$', fig_size=(15, 2))
        plt.subplot(1, 3, 1)
        _ = plt.plot(n, linearray[0, :])
        plt.subplot(1, 3, 2)
        _ = plt.plot(n, linearray[i, :])
    # plot simple difference between frame i and first frame
        plt.subplot(1, 3, 3)
        _ = plt.plot(n, linearray[i, :] - linearray[0, :])
        plt.ylabel('Frame n - Frame 0')


# ### grayscale plotting of line function:
#

# In[11]:

CAMERA_LENGTH = 128
INTENSITY_MIN = 0
INTENSITY_MAX = 255


def plot_gray(fig, camera_data):
    # x fencepost positions of each data matrix element
    x_mesh = []
    for i in range(0, len(camera_data)+1):
        x_mesh.append([i-0.5] * (CAMERA_LENGTH + 1))
    x_mesh = np.array(x_mesh)

    # y fencepost positions of each data matrix element
    y_array = range(0, CAMERA_LENGTH + 1)
    y_array = list(map(lambda x: x - 0.5, y_array))
    y_mesh = np.array([y_array] * (len(camera_data)+1))

    data_mesh = np.array(camera_data)
    vmax1 = np.max(data_mesh)
    data_mesh = INTENSITY_MAX * data_mesh/vmax1  # normalize intensity

    fig.set_xlim([-0.5, len(camera_data) - 0.5])
    fig.set_ylim([-8.5, CAMERA_LENGTH - 0.5])

    fig.pcolorfast(x_mesh, y_mesh, data_mesh,
                   cmap='gray', vmin=INTENSITY_MIN, vmax=INTENSITY_MAX,
                   interpolation='None')


# In[12]:

# inputs:
# linescans - An array of length n where each element is an array of length 128. Represents n frames of linescan data.

# outputs:
# track_center_list - A length n array of integers from 0 to 127. Represents the predicted center of the line in each frame.
# track_found_list - A length n array of booleans. Represents whether or not each frame contains a detected line.
# cross_found_list - A length n array of booleans. Represents whether or not each frame contains a crossing.

def find_track(linescans):
    n = len(linescans)
    track_center_list = n * [64]  # set to center pixel in line for test case
    track_found_list = n * [100]
    cross_found_list = n * [10]
    #  track_center_list[0] = np.argmax(linescans[0])
    # place holder function this is not robust
    last_center = np.argmax(linescans[0])

    for i in range(0, n):  # for each frame
        thresh = 128 * [0]
        diff = 128 * [0]
        line_starts = 20 * [0]
        line_ends = 20 * [127]
        line_num = 0
        upper_bound = 0.6*max(linescans[i])
        abs_min_thresh = 30
        for j in range(0, 128):
            if linescans[i][j] >= upper_bound and linescans[i][j] >= abs_min_thresh:
                thresh[j] = 1
            else:
                thresh[j] = 0

        for j in range(1, 128):
            diff[j] = thresh[j] - thresh[j-1]

        for j in range(0, 128):
            if diff[j] > 0:
                line_starts[line_num] = j
            elif diff[j] < 0:
                line_ends[line_num] = j-1
                line_num += 1

        line_starts[line_num:-1] = []
        line_ends[line_num:-1] = []
        line_starts.pop(-1)
        line_ends.pop(-1)

        num_lines = len(line_starts)
        centers = num_lines * [64]
        center_diffs = num_lines * [64]
        min_diff_index = 0
        min_diff = 256
        for j in range(0, num_lines):
            centers[j] = (line_starts[j] + line_ends[j]) / 2
            center_diffs[j] = centers[j] - last_center
            if abs(center_diffs[j]) < min_diff:
                min_diff_index = j
                min_diff = abs(center_diffs[j])

        # print(line_starts)
        # print(line_ends)
        # print(i)
        # print("\n")

        if centers == [] and i > 0:
            track_center_list[i] = track_center_list[i-1]
            track_found_list[i] = 10
        elif centers == [] and i <= 0:
            track_center_list[i] = 64
        else:
            track_center_list[i] = centers[min_diff_index]
            track_found_list[i] = 100
            if len(centers) > 1:
                cross_found_list[i] = 100

        last_center = track_center_list[i]

        # track_found_list = n * [100]  # 100 for true
        # track_found_list[200:300] = 10 * np.ones(100)  # 10 for false
        # cross_found_list = n * [10]  # 10 for false
        # throw in a few random cross founds
        # cross_found_list[1000:1100] = 100 * np.ones(100)
        # Code to be added here
        ###
        ###

    return track_center_list, track_found_list, cross_found_list


################
# need to use some different tricks to read csv file


def read_file():
    filename = './natcar2016_team1.csv'
    # filename = 'natcar2016_team1_short.csv'
    # csvfile=open(filename, 'rb') Python 2.7
    csvfile = open(filename, 'rt')  # Python 3.7
    telemreader = csv.reader(csvfile, delimiter=',', quotechar='"')
    # Actual Spring 2016 Natcar track recording by Team 1.
    # change for python 3.7
   # telemreader.next() # discard first line
   # telemdata = telemreader.next() # format time in ms, 128 byte array, velocity
    telemdata = next(telemreader)  # discard first line
    # format time in ms, 128 byte array, velocity
    telemdata = next(telemreader)
   # linescans=[]  # linescan array
#  times=[]  # should be global
#  velocities=[]
    for row in telemreader:
        times.append(eval(row[0]))  # sample time
        velocities.append(eval(row[2]))  # measured velocity
        line = row[1]  # get scan data
        arrayline = np.array(eval(line))  # convert line to an np array
        linescans.append(arrayline)
    print('scan line0:', linescans[0])
    print('scan line1:', linescans[1])
    return np.array(linescans)


scan_range = [0, 2500]
linescans = []
times = []
velocities = []
filename = './grading/natcar2016_team1.csv'
linescans = read_file()
linescans = linescans[scan_range[0]:scan_range[1]]
track_center_list, track_found_list, cross_found_list = find_track(linescans)
# for i, (track_center, track_found, cross_found) in enumerate(zip(track_center_list, track_found_list, cross_found_list)):
#    print 'scan # %d center at %d. Track_found = %s, Cross_found = %s' %(i,track_center,track_found, cross_found)


############# plots ###########
# fig=plt.figure()
# fig = plt.figure(figsize=(16, 3))
# fig.set_size_inches(13, 4)
# fig.suptitle("%s\n" % (filename))
# ax = plt.subplot(1, 1, 1)
# plot_gray(ax, linescans[0:1000])  # plot smaller range if hard too see
# plot_gray(ax, linescans)


# plot of velocities
# fig = plt.figure(figsize = (8, 4))
# fig.set_size_inches(13, 4)
# fig.suptitle("velocities %s\n" % (filename))
# plt.xlabel('time [ms]')
# plt.ylabel('velocity (m/s)')
# plt.plot(times,velocities)

# plot of found track position

# fig = plt.figure(figsize = (8, 4))
# fig.set_size_inches(13, 4)
# fig.suptitle("track center %s\n" % (filename))
# plt.xlabel('time [ms]')
# plt.ylabel('track center')
# plt.plot(times,track_center_list)

######################################
# plot superimposed on grayscale
fig = plt.figure(figsize=(35, 15))
fig.clf()
fig.suptitle("HW1 Spring 2019")
idx0 = scan_range[0]
idx1 = scan_range[1]
ax = fig.add_subplot(411)
ax.imshow(linescans.T, cmap='gray')
# ax.get_yaxis().set_visible(False)
# ax.set_xlabel('Time')
ax.set_title('Track Section')
x_lim, y_lim = ax.get_xlim(), ax.get_ylim()

ax = fig.add_subplot(412)
ax.imshow(linescans.T, cmap='gray')
# ax.get_yaxis().set_visible(False)
ax.plot(track_center_list, 'r-')
# ax.set_xlabel('Time')
ax.set_title('Track Center')
ax.set_xlim(x_lim)
ax.set_ylim(y_lim)


ax = fig.add_subplot(413)
ax.plot(track_found_list)

ax.imshow(linescans.T, cmap='gray')
ax.plot(track_found_list, 'r-')
# ax.get_yaxis().set_visible(False)
# ax.set_xlabel('Time')
ax.set_title('Track Found')
ax.set_xlim(x_lim)
ax.set_ylim(y_lim)

ax = fig.add_subplot(414)
ax.imshow(linescans.T, cmap='gray')
# ax.get_yaxis().set_visible(False)
ax.plot(cross_found_list, 'r-')
ax.set_xlabel('Time')
ax.set_title('Cross Found')
ax.set_xlim(x_lim)
ax.set_ylim(y_lim)

plt.show()
