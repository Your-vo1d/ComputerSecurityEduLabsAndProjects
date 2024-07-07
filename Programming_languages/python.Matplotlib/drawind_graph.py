import math

import matplotlib.pyplot as plt


def f(x):
    x = math.radians(x)
    result = math.exp(math.cos(x)) + math.log(math.sin(0.8 * x) + 1) * math.cos(x)
    return result


def y(x):
    x = math.radians(x)
    result = -math.log((math.cos(x) + math.sin(x)) ** 2 + 1.7) + 2
    return result


start = -240
finish = 360
step = 1

x_list = [start + i for i in range(740)]
f_list = [f(x) for x in x_list]
y_list = [y(x) for x in x_list]

line_f = plt.plot(x_list, f_list, label="f(x)")
line_y = plt.plot(x_list, y_list, label="y(x)")

plt.setp(line_f, color="blue", linewidth=2)
plt.setp(line_y, color="red", linewidth=2)

plt.gca().spines["left"].set_position("zero")
plt.gca().spines["bottom"].set_position("zero")
plt.gca().spines["top"].set_visible(False)
plt.gca().spines["right"].set_visible(False)

plt.legend()
plt.title("Графики функций")

plt.show()
