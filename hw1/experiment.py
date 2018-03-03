import os
import time
import numpy as np
import random
import matplotlib.pyplot as plt
import subprocess

def responceAnalysis(responce, numprocesses, totalTime):
    waittimes = np.zeros((numprocesses), dtype=float)
    turnaroundtimes = np.zeros((numprocesses), dtype=float)
    responce = responce[:-1]
    arr = responce.split("\n")
    for i,elem in enumerate(arr):
        arr[i] = elem.split(",")
    for i in range(len(responce.split("\n"))):
        if int(arr[i][0]) == 0:
            np.put(waittimes,int(arr[i][1]), waittimes[int(arr[i][1])]+(float(arr[i+1][2])-float(arr[i][2])))
        if int(arr[i][0]) == 2:
            np.put(turnaroundtimes, int(arr[i][1]),(float(arr[i][3])-float(arr[i][2])))
    for index, elem in enumerate(waittimes):
        np.put(waittimes, index, totalTime-elem)
    return np.mean(waittimes), np.mean(turnaroundtimes)

waits = np.array([])
turns = np.array([])
initialOptions = np.array([], dtype=float)
print("Begining Executions...")
for i in np.arange(200, 1100, 100, dtype=int):
    temparr = np.array([])
    tempwaits = np.array([])
    tempturns = np.array([])
    for j in range(5):
        print("Executing quantum = ", i, "j = ", j)
        start = time.time()
        # responce = str(os.system('./hw1 4 4 100 30 1 '+str(i)+' '+str(random.randint(1,300))+' 7'))
        responce = subprocess.run(['./hw1','50','50','5000','100','1', str(i), str(random.randint(1,300)),'7'], stdout=subprocess.PIPE)
        end = time.time()
        print("Finished executing quantum = ", i, "j = ", j)
        print("Analyzing Responce...")
        # print("The responce was: ", responce.stdout)
        #print(responce.stdout.decode('utf-8'))
        currwait, currturn = responceAnalysis(responce.stdout.decode('utf-8'), 5000, 1000*(start-end))
        tempwaits = np.append(waits, (start-end)-currwait)
        tempturns = np.append(turns, currturn)
        temparr = np.append(temparr,1000*(end-start))
    initialOptions = np.append(initialOptions, np.mean(temparr))
    waits = np.append(waits, np.mean(tempwaits))
    turns = np.append(turns, np.mean(tempturns))
print("50 Consumers, 50 Producers, 5000 Products, Queue Size 30 (quantum 10-510 inc 50): ", initialOptions)

print("Displaying Graph...")
plt.plot(np.arange(200, 1100, 100, dtype=int), waits, '-r^')
plt.xlabel("Quantum (fn(10))")
plt.ylabel("Time (ms)")
plt.title("Avg Wait Time vs Quantum")
plt.show()
plt.plot(np.arange(200, 1100, 100, dtype=int), turns, '-rs')
plt.title("Avg Turnaround Time vs Quantum")
plt.show()
plt.plot(np.arange(200, 1100, 100, dtype=int), initialOptions, '-ro',)
plt.title("Total Execution Time vs Quantum")
plt.show()
