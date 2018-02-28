import socket

import hexdump

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', 2345))

while True:
    x, addr = s.recvfrom(1024)
    print("{} sent".format(addr))
    hexdump.hexdump(x)