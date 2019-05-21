import pstricks as ps

# Setup
rows = 4
cols = 4


# ps.put('\psset { coilarm=0.5, coilwidth=0.3, coilheight=0.4 }\n')
ps.init()
ps.dotSize = 0.2
ps.text(1.3, 1.4, '$\overset{\scriptscriptstyle{2, 2}}{u}$');

for y in range(rows):
    for x in range(cols):
        ps.dot(x, y)
        if x == cols - 1 and y == rows - 2:
            ps.zigzag(x, y, x, y + 1) 
        else:
            if x < cols - 1:
                ps.zigzag(x, y, x+1, y)

            if y < rows - 1:
                if x == cols - 1:
                    ps.zigzag(x, y, x, y+1)
                else:
                    ps.zigzag(x, y, x, y+1)

ps.end();
ps.render();
