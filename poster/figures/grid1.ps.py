import pstricks as ps

ps.init()
ps.dotSize = 0.2
ps.dots([(0,0), (1,0), (0, 1), (1, 1), (0, 2), (1, 2)])

# Buttom

ps.lineBeginIncrease = 0.1
ps.lineEndDecrease = 0.1

# Horizontal
ps.line(0, 0, 1, 0, arrowBegin=True)
ps.line(0, 1, 1, 1, arrowBegin=True,arrowEnd=True)
ps.line(1, 2, 0, 2, arrowBegin=True) 

# Right vertical
ps.line(1, 0, 1, 1, arrowBegin=True)
ps.line(1, 1, 1, 2, arrowBegin=True)

# Left vertical
ps.line(0, 0, 0, 1, arrowEnd=True)
ps.line(0, 1, 0, 2, arrowEnd=True)

ps.end()
ps.render()
