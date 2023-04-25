import matplotlib.pyplot as plt
import numpy as np

rms = float(input())
num_line = int(input())
fig = plt.figure()
line_eq, partitions = list(), list()
cur_col = 0
col_arr = ['r', 'b']
x_min , x_max = 10 ** 10, -10 ** 10
y_min, y_max = 10 ** 10, -10 ** 10
for _ in range(num_line) : 
    num_pts = int(input())
    part = list()
    for pts in range(num_pts) : 
        x , y = map(float , input().split())
        x_max = max(x_max , x)
        x_min = min(x_min , x)
        y_max = max(y_max , y)
        y_min = max(y_min , y)
        part.append((x, y))
    partitions.append(part)
    a , b , c = map(float , input().split())
    line_eq.append((a,b,c))

ax = fig.add_axes([0, 0, 1 , 1])
plt.xlabel('x')
plt.ylabel('y')
for _ in range(len(partitions)) :
    x, y = list(), list()
    for i in range(len(partitions[_])) :
        x.append(partitions[_][i][0])
        y.append(partitions[_][i][1])
    ax.scatter(x , y , color = col_arr[cur_col])
    a, b, c = line_eq[_]
    if b == 0 : 
        plt.axvline(x = -c, color = col_arr[cur_col])
    else : 
        y_max_p = a * x_max + c
        y_min_p = a * x_min + c
        x_now = [x_min, x_max]
        y_now = [y_min_p, y_max_p]
        ax.plot(x_now, y_now, color = col_arr[cur_col])
    cur_col ^= 1
plt.show()