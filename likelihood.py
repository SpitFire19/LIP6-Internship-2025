#!/usr/bin/env python 

from random import randint, choice, seed
from tkinter import *
seed(2)
def calc_poly(polynomial, p0, q0):
    L = 1
    probas = [p0, 1 - p0, q0, 1 - q0]
    return p0**3 * (1-p0)**25 * q0 ** 8 * (1-q0) ** 5

n = 8
A, B = -100, 100
gameboard = [randint(A, B) for _ in range(n)]
strats = ["G", "O"] # greedy, optimal
fp_strategy = "GGO"*10
"""
for move in fp_strategy:
    move = choice(strats)
"""
p = [0.01 * k for k in range(1, 100)]
q = [0.01 * k for k in range(1, 100)]

polynomial = [0] * (len(fp_strategy)-1)

"""
G -> O = 0 p
G -> G = 1 1 - p
O -> G = 2 q
O -> O = 3 1 - q
"""

for i in range(1, len(fp_strategy)):
    if fp_strategy[i-1] == "G":
        if fp_strategy[i] == "O":
            polynomial[i-1] = 0
        else:
            polynomial[i-1] = 1
    if fp_strategy[i-1] == "O":
        if fp_strategy[i] == "G":
            polynomial[i-1] = 2
        else:
            polynomial[i-1] = 3



k = 99
likelihood = [[0] * k] * k

for i in range(k):
    for j in range(k):
        likelihood[i][j] = calc_poly(polynomial, p[i], q[j]) # ] 0, 1[ -> [0, 255]

mx = max([max(likelihood[i]) for i in range(k)])
mn = min([min(likelihood[i]) for i in range(k)])

for i in range(k):
    for j in range(k):
        likelihood[i][j] = likelihood[i][j]
        # print(mx, " ", likelihood[i][j], " ", 255 * (1 - likelihood[i][j] / mx), end='\n')
    # print("")

root = Tk()

C = Canvas(root, bg="blue", 
           height = 495, width = 495)
rect_height, rect_width = 5, 5


# print(calc_poly(polynomial, 0.01, 0.05))

"""
print("\n\n")
for i in range(10):
    for j in range(10):
        calc_poly(polynomial, 0.3, 0.7)
    print("\n")
"""
print(mx, mn)
for i in range(k):
    for j in range(k):
        color_hex = hex(int(255 * (1 - likelihood[i][j] / mx)))[2:]
        rect_color = "#" + 3 * color_hex
        C.create_rectangle(rect_width * i, rect_width * j,
                           rect_width * (i + 1), rect_width * (j + 1),
                           fill=rect_color)

C.pack()

C.mainloop()
