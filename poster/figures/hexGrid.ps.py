import pstricks as ps
from math import *

numRows = 2
numCols = 7
phi = (pi - 2 * pi / 6) / 2

ps.init()
ps.dotSize = 0.2

def hexagon(x0, y0, a0 = 2 * pi / 12, n = 6):
    lastX = 0
    lastY = 0

    for i in range(n):
        a = a0 + i * 2.0 * pi / 6
        x = x0 + cos(a)
        y = y0 + sin(a)
        ps.dot(x, y)

        if i > 0:
            ps.zigzag(x, y, lastX, lastY)

        lastX = x
        lastY = y

r = cos(2 * pi / 12);
hexagon(2, 3)
hexagon(2+2*r, 3)
hexagon(2+4*r, 3, n=7)

h = -cos(pi / 2 + pi / 6)
hexagon(2+r, 4 + h, n = 4)
hexagon(2+r+2*r, 4 + h, n=5, a0 = -2 * pi / 12)

h = cos(pi / 2 + pi / 6)
hexagon(2+r, 1 - h, n = 4, a0 = 5 * 2 * pi / 12)
hexagon(2+r+2*r, 1 - h, n=5, a0 = 5 * 2 * pi / 12)

ps.end();
ps.render();

