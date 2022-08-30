import socket
import numpy as np
from time import sleep
import sys
import math
import random

print("The client sending to:", sys.argv[1], " on port:" , sys.argv[2])

datainalexa = np.loadtxt("processedin.txt", dtype=int)
dataoutalexa = np.loadtxt("processedout.txt", dtype=int)

dataineco = np.loadtxt("processedineco.txt", dtype=int)
dataouteco = np.loadtxt("processedouteco.txt", dtype=int)

datainhub = np.loadtxt("processedinhub.txt", dtype=int)
dataouthub = np.loadtxt("processedouthub.txt", dtype=int)

datainplg = np.loadtxt("processedinplg.txt", dtype=int)
dataoutplg = np.loadtxt("processedoutplg.txt", dtype=int)

datain=datainalexa+dataineco+datainhub+datainplg
dataout=dataoutalexa+dataouteco+dataouthub+dataoutplg

msgFromClient       = "Hello UDP Server"
bytesToSend         = str.encode(msgFromClient)
serverAddressPort   = (sys.argv[1], int(sys.argv[2]))
bufferSize          = 2048

# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM) 

t=int(sys.argv[3])

#for i in range(len(s)):
#    if type(s[i]) != int and type(s[i]) != np.int64:
#        print("{}: {} ({})\n".format(i, s[i], type(s[i])))

if sys.argv[4] == "DL":
    s = datain*t
    print("DL")
else:
    s = dataout*t
    print("UL")

while True:
    for x in s:
        if x != 0:
            if x > 65000:
                div = math.floor(x/65000)
                rem = x % 65000
                for i in range(div):
                    #print(65000)
                    UDPClientSocket.sendto(bytearray([1] * 65000), serverAddressPort)
                #print(rem)
                UDPClientSocket.sendto(bytearray([1] * rem), serverAddressPort)
            else:    
                #print(s)
                UDPClientSocket.sendto(bytearray([1] * x), serverAddressPort)
            sleep(0.001)
        else:
            UDPClientSocket.sendto(bytearray([0]), serverAddressPort)
            sleep(0.001)


msgFromServer = UDPClientSocket.recvfrom(bufferSize)
msg = "Message from Server {}".format(msgFromServer[0])
