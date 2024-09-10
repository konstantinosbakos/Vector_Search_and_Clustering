
with open('10000_l.dat', 'r+b') as f:
    x = 0
    y = 10000
#    f.write(x.to_bytes(1,'big'))
    f.seek(4)
    f.write(y.to_bytes(4,'big'))
