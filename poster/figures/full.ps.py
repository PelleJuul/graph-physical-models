import pstricks as ps
from math import *

ps.init()
ps.dotSize = 0.2

numArms = 6
phi = (pi - 2 * pi / 6) / 2

a0 = pi / 2.0
x0 = 3.0
y0 = 3.0
r = 1.0

def drawStar(x0, y0, numArms, skip=[]):
    for i in range(numArms):

        a = a0 + i * 2 * pi / numArms
        x = x0 + r* cos(a)
        y = y0 + r*sin(a)
        ps.dot(x, y)

        for j in range(1, numArms):
            a1 = a + j * 2 * pi / numArms
            x1 = x0 + r*cos(a1)
            y1 = y0 + r*sin(a1)

            ps.dot(x1, y1);
            ps.line(x, y, x1, y1)

drawStar(x0, y0, numArms)
ps.end()
ps.render()
