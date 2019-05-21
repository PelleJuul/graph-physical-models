from math import *

psText = ""
dotSize = 0.1
xMin = 0
yMin = 0
xMax = 0
yMax = 0
cropFigure = True
lineEndDecrease = None
lineBeginIncrease = None

def clear():
    global psText
    psText = ''

def init():
    global psText
    global dotSize
    global xMin
    global yMin
    global xMax
    global yMax
    dotSize = 0.1
    xMin = inf 
    yMin = inf
    xMax = -inf
    yMax = -inf
    psText = ''

def end():
    global xMin
    global yMin
    global xMax
    global yMax
    global psText
    psText = f'\\psscalebox{{1.0 1.0}} {{\n\\begin{{pspicture}}({xMin},{yMin})({xMax}, {yMax})\n' + psText
    psText += '\\end{pspicture} }'

def updateMinMax(x, y):
    global xMin
    global yMin
    global xMax
    global yMax
    xMin = min(x, xMin)
    yMin = min(y, yMin)
    xMax = max(x, xMax)
    yMax = max(y, yMax)

def put(s):
    global psText
    psText += s

def text(x, y, s):
    global psText
    updateMinMax(x, y)
    # \rput(4, 2.5){$u_{l-1}$}
    psText += f'\\rput({x},{y}){{{s}}}'

def framebox(x, y, text, arc=0.3):
    global psText
    updateMinMax(x, y)
    psText += f'\\rput({x},{y}){{\\psframebox*[framearc={arc}]{{{text}}} }}\n'

def render():
    global psText
    print(psText)

def dot(x, y, fillcolor='black'):
    global psText
    global dotSize
    updateMinMax(x, y)
    # \psdot[linecolor=blue,fillcolor=red, dotstyle=o,dotsize=24pt]
    psText += f'\\psdot[linecolor={fillcolor}, dotsize={dotSize}]({x},{y})\n'
 
def dots(xys):
    global psText
    global dotSize
    cords = ''.join([f'({xy[0]},{xy[1]})' for xy in xys])
    psText += f'\\psdots[dotsize={dotSize}]{cords}\n'

def zigzag(x1, y1, x2, y2, arrowBegin=False, arrowEnd=False, beginIncrease=None, endDecrease=None, linecolor='black'):
    global psText
    global lineBeginIncrease
    global lineEndDecrease
    updateMinMax(x1, y1)
    updateMinMax(x2, y2)

    if beginIncrease == None:
        beginIncrease = lineBeginIncrease

    if endDecrease == None:
        endDecrease = lineEndDecrease

    if endDecrease != None:
        dx = x2 - x1
        dy = y2 - y1
        if dx == 0:
            if y2 < y1:
                y2 += endDecrease
            else:
                y2 -= endDecrease
        else:
            l = sqrt(dx**2 + dy**2)
            x2 = x1 + dx * ((l - endDecrease) / l)
            a = dy / dx
            y2 = y1 + a * (x2 - x1)

    if beginIncrease != None:
        dx = x2 - x1
        dy = y2 - y1
        if dx == 0:
            if y2 < y1:
                y1 -= beginIncrease
            else:
                y1 += beginIncrease
        else:
            l = sqrt(dx**2 + dy**2)
            d = (beginIncrease / l)
            x1 = x1 + dx * d
            y1 = y1 + dy * d

    arrow = ''

    if arrowEnd and arrowBegin:
        arrow = '{<->}'
    elif arrowEnd:
        arrow = '{->}'
    elif arrowBegin:
        arrow = '{<-}'
    # \pszigzag[linecolor=gray](8,1.25)(10,0.5)
    psText += f'\\pszigzag[coilarm=0.3, coilwidth=0.15, coilheight=0.4, linecolor={linecolor}]{arrow}({x1},{y1})({x2},{y2})\n'

def arc(x, y, r, a1, a2, arrowBegin=False, arrowEnd=False):
    global psText
    updateMinMax(x, y)
    arrow = ''

    if arrowEnd and arrowBegin:
        arrow = '{<->}'
    elif arrowEnd:
        arrow = '{->}'
    elif arrowBegin:
        arrow = '{<-}'

    if a2 > a1:
        psText += f'\\psarc{arrow}({x},{y}){{{r}}}{{{a1}}}{{{a2}}}\n'
    else:
        psText += f'\\psarcn{arrow}({x},{y}){{{r}}}{{{a1}}}{{{a2}}}\n'

def line(x1, y1, x2, y2, arrowBegin=False, arrowEnd=False, beginIncrease=None, endDecrease=None):
    global psText
    global lineBeginIncrease
    global lineEndDecrease
    updateMinMax(x1, y1)
    updateMinMax(x1, y1)

    if beginIncrease == None:
        beginIncrease = lineBeginIncrease

    if endDecrease == None:
        endDecrease = lineEndDecrease

    if endDecrease != None:
        dx = x2 - x1
        dy = y2 - y1
        if dx == 0:
            if y2 < y1:
                y2 += endDecrease
            else:
                y2 -= endDecrease
        else:
            l = sqrt(dx**2 + dy**2)
            x2 = x1 + dx * ((l - endDecrease) / l)
            a = dy / dx
            y2 = y1 + a * (x2 - x1)

    if beginIncrease != None:
        dx = x2 - x1
        dy = y2 - y1
        if dx == 0:
            if y2 < y1:
                y1 -= beginIncrease
            else:
                y1 += beginIncrease
        else:
            l = sqrt(dx**2 + dy**2)
            d = (beginIncrease / l)
            x1 = x1 + dx * d
            y1 = y1 + dy * d

    arrow = ''

    if arrowEnd and arrowBegin:
        arrow = '{<->}'
    elif arrowEnd:
        arrow = '{->}'
    elif arrowBegin:
        arrow = '{<-}'

    psText += f'\\psline{arrow}({x1}, {y1})({x2}, {y2})\n'
