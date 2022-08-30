import numpy as np
import scipy
import scipy.stats
import matplotlib.pyplot as plt

val = input("Enter your value: ")

datain = np.loadtxt("processedin.txt", dtype=int)
dataout = np.loadtxt("processedout.txt", dtype=int)

# Scale here if necessary
datain=datain
dataout=dataout

nzdatain = np.nonzero(datain)[0] 
nzdataout = np.nonzero(dataout)[0] 
nzsizein = nzdatain.size
nzsizeout = nzdataout.size

tsizein = datain.size
tsizeout = dataout.size

#print("Total Active Inbound Percentage")
#print(nzsizein/tsizein)
#print("Total Active Outbound Percentage")
#print(nzsizeout/tsizeout)

xin = np.arange(tsizein)/1000
xout = np.arange(tsizeout)/1000


print(tsizein)
print(tsizeout)

bw_dl = np.sum(datain)*60*60*1000*8/(tsizein*1024*1024*1024)
bw_ul = np.sum(dataout)*60*60*1000*8/(tsizeout*1024*1024*1024)


print(bw_dl)
print(bw_ul)

plt.figure()
plt.xlabel('Time (sec)')
plt.ylabel('Data sent (bytes)')
plt.plot(xin,datain[0:tsizein])
#plt.ylim([0,400])
#plt.show()
#plt.savefig(val + '_in' + '.png')

plt.figure()
plt.xlabel('Time (sec)')
plt.ylabel('Data sent (bytes)')
plt.plot(xout,dataout[0:tsizeout])
#plt.ylim([0,400])
#plt.savefig(val + '_out' + '.png')


