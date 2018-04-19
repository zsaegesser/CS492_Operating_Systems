import numpy as np
import matplotlib.pyplot as plt
import subprocess

# Test 1
# FIFO w/ pre-page flag off
# test1 = np.array([])
# for i in [1,2,4,8,16]:
#     responce = subprocess.run(['./VMsimulator','plist.txt', 'ptrace.txt',  str(i), 'FIFO', '-'], stdout=subprocess.PIPE) #run the code, get the print out
#     page_faults = responce.stdout.decode('utf-8') #turn print out into a string
#     # page_faults = page_faults.split(" ")[2] #the third part of the string is the number
#     page_faults = int(page_faults) #turn it into an int
#     test1 = np.append(test1,page_faults)
#

# Test 2
# LRU w/ pre-page flag off
# test2 = np.array([])
# for i in [1,2,4,8,16]:
#     responce = subprocess.run(['./VMsimulator','plist.txt', 'ptrace.txt',str(i), 'LRU', '-'], stdout=subprocess.PIPE) #run the code, get the print out
#     page_faults = responce.stdout.decode('utf-8') #turn print out into a string
#     print(page_faults)
#
#     # page_faults = string.split(" ")[2] #the third part of the string is the number
#     page_faults = int(page_faults) #turn it into an int
#     test2 = np.append(test2,page_faults)

# Test 3
# Clock w/ pre-page flag off
# test3 = np.array([])
# for i in [1,2,4,8,16]:
#     responce = subprocess.run(['./VMsimulator',str(i), 'FIFO', '-'], stdout=subprocess.PIPE) #run the code, get the print out
#     string = responce.stdout.decode('utf-8') #turn print out into a string
#     page_faults = string.split(" ")[2] #the third part of the string is the number
#     page_faults = int(page_faults) #turn it into an int
#     test3 = np.append(page_faults)


# Test 1
# FIFO w/ pre-page flag on
# test4 = np.array([])
# for i in [1,2,4,8,16]:
#     responce = subprocess.run(['./VMsimulator', 'plist.txt', 'ptrace.txt',str(i), 'FIFO', '+'], stdout=subprocess.PIPE) #run the code, get the print out
#     page_faults = responce.stdout.decode('utf-8') #turn print out into a string
#     # page_faults = page_faults.split(" ")[2] #the third part of the string is the number
#     page_faults = int(page_faults) #turn it into an int
#     test4 = np.append(test4,page_faults)


# Test 5
# LRU w/ pre-page flag on
# test5 = np.array([])
# for i in [1,2,4,8,16]:
#     responce = subprocess.run(['./VMsimulator','plist.txt', 'ptrace.txt',str(i), 'LRU', '+'], stdout=subprocess.PIPE) #run the code, get the print out
#     page_faults = responce.stdout.decode('utf-8') #turn print out into a string
#     # page_faults = string.split(" ")[2] #the third part of the string is the number
#     page_faults = int(page_faults) #turn it into an int
#     test5 = np.append(test5,page_faults)

# Test 6
# Clock w/ pre-page flag on
# test6 = np.array([])
# for i in [1,2,4,8,16]:
#     responce = subprocess.run(['./VMsimulator',str(i), 'FIFO', '+'], stdout=subprocess.PIPE) #run the code, get the print out
#     string = responce.stdout.decode('utf-8') #turn print out into a string
#     page_faults = string.split(" ")[2] #the third part of the string is the number
#     page_faults = int(page_faults) #turn it into an int
#     test6 = np.append(page_faults)


#Graphing
# test1 graph
# plt.plot([1,2,4,8,16], test1, '-r^')
# plt.xlabel("Page Size")
# plt.ylabel("Page Faults")
# plt.title("FIFO w/o Pre-Paging")
# plt.show()

# test2 graph
# plt.plot([1,2,4,8,16], [185232,138341,110359,102616,97178], '-r^')
# plt.xlabel("Page Size")
# plt.ylabel("Page Faults")
# plt.title("LRU w/o Pre-Paging")
# plt.show()

# test3 graph
plt.plot([1,2,4,8,16], [195861,183282,194051,211127,246241], '-r^')
plt.xlabel("Page Size")
plt.ylabel("Page Faults")
plt.title("Clock w/o Pre-Paging")
plt.show()

# test4 graph
# plt.plot([1,2,4,8,16], test4, '-r^')
# plt.xlabel("Page Size")
# plt.ylabel("Page Faults")
# plt.title("FIFO w/ Pre-Paging")
# plt.show()

# test5 graph
# plt.plot([1,2,4,8,16], [99694,94233,93292,92950,106155], '-r^')
# plt.xlabel("Page Size")
# plt.ylabel("Page Faults")
# plt.title("LRU w/ Pre-Paging")
# plt.show()

# test6 graph
plt.plot([1,2,4,8,16], [276120,322540,381913,466830,480591], '-r^')
plt.xlabel("Page Size")
plt.ylabel("Page Faults")
plt.title("Clock w/ Pre-Paging")
plt.show()
