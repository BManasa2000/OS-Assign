# -*- coding: utf-8 -*-
"""OSAssignment_Graphs.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1_Mvd0xy1OjVrvTpmSbBoGNmi6yqv4Vms
"""

import matplotlib.pyplot as plt
 
# line 1 points
x1 = [10,25,50,75,100,250,500,750,1000,2500,5000, 7500,10000,25000,50000,75000,100000]
y1 = [16366,15967,8951,13176,15627,12943,15296,95559,14874,6075,35669,75530,19271,37960,9800,18579,18146]
# plotting the line 1 points
plt.plot(x1, y1, label = "TAT 1")

# line 2 points
x2 =[10,25,50,75,100,250,500,750,1000,2500,5000, 7500,10000,25000,50000,75000,100000]
y2 = [13411,15285,8991,10083,13533,13076,18941,94317,20440,47766,87668,140704,132153,307929,602854,827067,1191415]
# plotting the line 2 points
plt.plot(x2, y2, label = "TAT 2")

# line 3 points
x3 =[10,25,50,75,100,250,500,750,1000,2500,5000, 7500,10000,25000,50000,75000,100000]
y3 = [14258,13201,8995,10731,17437,10390,12092,79591,8403,15492,44050,84009,44166,79204,170644,131004,246405]
# plotting the line 2 points
plt.plot(x3, y3, label = "TAT 3")

 
# naming the x axis
plt.xlabel('Workload Size')
# naming the y axis
plt.ylabel('TAT')
# giving a title to my graph
plt.title("RR-Graph for TAT of 3 processes")
 
# show a legend on the plot
plt.legend()

 
# function to show the plot
plt.show()

import matplotlib.pyplot as plt
 
# line 1 points
x1 = [10,25,50,75,100,250,500,750,1000,2500,5000, 7500,10000,25000,50000,75000,100000]
y1 = [13142,13786,8180,12048,15247,10703,13757,82353,14141,6016,34585,68765,17683,34387,9163,17802,17826]
# plotting the line 1 points
plt.plot(x1, y1, label = "WT 1")

# line 2 points
x2 =[10,25,50,75,100,250,500,750,1000,2500,5000, 7500,10000,25000,50000,75000,100000]
y2 = [12381,13549,8405,8921,12521,10278,14818,80203,13352,32122,68501,108006,88537,217152,419878,565777,826206]
# plotting the line 2 points
plt.plot(x2, y2, label = "WT 2")

# line 3 points
x3 =[10,25,50,75,100,250,500,750,1000,2500,5000, 7500,10000,25000,50000,75000,100000]
y3 = [12096,12222,8067,10048,16945,9381,11260,68009,7643,14056,40282,72739,36198,66269,146440,103874,200307]
# plotting the line 2 points
plt.plot(x3, y3, label = "WT 3")

 
# naming the x axis
plt.xlabel('Workload Size')
# naming the y axis
plt.ylabel('WT')
# giving a title to my graph
plt.title("RR-Graph for WT of 3 processes")
 
# show a legend on the plot
plt.legend()

 
# function to show the plot
plt.show()

import matplotlib.pyplot as plt
 
# line 1 points
x1 = [10,25,50,75,100,250,500,750,1000,2500,5000,7500,10000,25000,50000,75000,100000]
y1 = [49,39,44,49,48,41,42,54,44,58,179,941,1075,1140,1144,1203,1295]
# plotting the line 1 points
plt.plot(x1, y1, label = "TAT 1")

# line 2 points
x2 =[10,25,50,75,100,250,500,750,1000,2500,5000,7500,10000,25000,50000,75000,100000]
y2 = [649,696,2092,1114,1024,1852,3537,5527,7951,14158,20467,33428,43099,88152,178035,259951,346950]
# plotting the line 2 points
plt.plot(x2, y2, label = "TAT 2")

# line 3 points
x3 =[10,25,50,75,100,250,500,750,1000,2500,5000, 7500,10000,25000,50000,75000,100000]
y3 = [2637,1393,5051,3406,3571,3457,6444,9045,13235,23091,34274,54126,68412,138102,287604,416803,558498]
# plotting the line 2 points
plt.plot(x3, y3, label = "TAT 3")

 
# naming the x axis
plt.xlabel('Workload Size')
# naming the y axis
plt.ylabel('TAT')
# giving a title to my graph
plt.title("FCFS-Graph for TAT of 3 processes")
 
# show a legend on the plot
plt.legend()

 
# function to show the plot
plt.show()

import matplotlib.pyplot as plt
 
# line 1 points
x1 = [10,25,50,75,100,250,500,750,1000,2500,5000,7500,10000,25000,50000,75000,100000]
y1 = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
# plotting the line 1 points
plt.plot(x1, y1, label = "WT 1")

# line 2 points
x2 =[10,25,50,75,100,250,500,750,1000,2500,5000,7500,10000,25000,50000,75000,100000]
y2 = [49,39,44,49,48,41,42,54,44,58,179,941,1075,1140,1144,1203,1295]
# plotting the line 2 points
plt.plot(x2, y2, label = "WT 2")

# line 3 points
x3 =[10,25,50,75,100,250,500,750,1000,2500,5000, 7500,10000,25000,50000,75000,100000]
y3 = [649,696,2092,1114,1024,1852,3537,5527,7951,14158,20467,33428,43099,88152,178035,259951,346950]
# plotting the line 2 points
plt.plot(x3, y3, label = "WT 3")

 
# naming the x axis
plt.xlabel('Workload Size')
# naming the y axis
plt.ylabel('WT')
# giving a title to my graph
plt.title("FCFS-Graph for WT of 3 processes")
 
# show a legend on the plot
plt.legend()

 
# function to show the plot
plt.show()
