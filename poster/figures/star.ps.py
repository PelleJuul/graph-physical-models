import pstricks as ps
from math import *

ps.init()
ps.dotSize = 0.2

numArms = 6
phi = (pi - 2 * pi / 6) / 2

a0 = pi / 2.0
x0 = 2.0
y0 = 2.0


def drawStar(x0, y0, numArms, skip=[]):
    for i in range(numArms):

        a = a0 + i * 2 * pi / numArms
        x = x0 + cos(a)
        y = y0 + sin(a)
        if i in skip:
            ps.dot(x, y, fillcolor='lightgray')
            ps.zigzag(x, y, x0, y0, linecolor='lightgray')
        else:
            ps.dot(x, y);
            ps.zigzag(x, y, x0, y0)
    ps.dot(x0, y0)
drawStar(x0, y0, numArms)
ps.text(x0, y0 - 2.5, 'a)')

ps.arc(x0, y0, 1.2, 0, -90, arrowEnd=True)
a = - pi / 4
l = 1.6
ps.text(x0 + l * cos(a), y0 + l * sin(a), '$N-1$')

x0 += 1.5
ps.line(x0, y0, x0 + 1, y0, arrowEnd=True)

x0 += 2.5
y0 -= 0.5

drawStar(x0, y0, numArms, skip=[5])
ps.text(x0, y0 - 2.0, 'b)')
ps.zigzag(x0, y0 + 1, x0, y0 + 2)
ps.dot(x0, y0 + 2)

ps.arc(x0, y0, 1.2, 0, -90, arrowEnd=True)
a = - pi / 4
l = 1.6
ps.text(x0 + l * cos(a), y0 + l * sin(a), '$N-2$')
ps.framebox(x0 + 0.5, y0, '$-1$')
ps.framebox(x0 + 1.4, y0 + 0.5, '$-2$')
ps.framebox(x0 + 0.5, y0 + 1, '$+2$')
ps.framebox(x0 + 0.5, y0 + 2, '$+2$')

ps.end()
ps.render()
