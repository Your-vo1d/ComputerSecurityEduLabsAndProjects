import numpy as np

def get_trend(x, a):
    y = a[0] * x **2 + a[1]* x + a[2]
    return y


coord_x = np.array(list(map(float, input().split())))
coord_y = np.array(list(map(float, input().split())))

coord_target_x = float(input())
coord_target_y = float(input())

a = np.polyfit(coord_target_x, coord_target_y)