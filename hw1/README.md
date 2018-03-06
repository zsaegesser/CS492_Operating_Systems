# HEllO ZACHARY

#### Ok so I didn't really work on this that much after dinner but heres the progress I made after talking to you on the phone:

Fuck trying to make pthread_create pass multiple arguments to the threads. It was getting waaaay too complicated just to send arguments. I had to make a struct that contained the args which wasn't working because it was being sent casted as a void pointer which lead to ownership problems that would have required dozens of extra lines to fix.... so I made another global instead to pass the max products. I had your arg parsing section set the global instead of the local which was probably a bad idea.

Also I know about the compiler warning, I don't really know what to do about that.



### Current printout on my end :

(and render the markdown to see my gif)
~~~
➜  hw1 git:(master) ✗ ./hw1 1 4 12 4 1 6 7
I'm a producer: 2 and I'm adding product: 2 to the queue, now size: 1
I'm a producer: 0 and I'm adding product: 1 to the queue, now size: 2
I'm a producer: 1 and I'm adding product: 0 to the queue, now size: 3
I'm a producer: 3 and I'm adding product: 3 to the queue, now size: 4
I'm a producer: 2 and I'm adding product: 4 to the queue, now size: 5
I'm a producer: 0 and I'm adding product: 5 to the queue, now size: 6
I'm a producer: 1 and I'm adding product: 6 to the queue, now size: 7
I'm a producer: 3 and I'm adding product: 7 to the queue, now size: 8
I'm a producer: 2 and I'm adding product: 8 to the queue, now size: 9
I'm a producer: 0 and I'm adding product: 9 to the queue, now size: 10
I'm a producer: 1 and I'm adding product: 10 to the queue, now size: 11
I'm a producer: 3 and I'm adding product: 11 to the queue, now size: 12
~~~

The next step would be to use the queueCount condition variable to hault the producers when the queue is full but that brings up the dilemma of whether or not we should make the queue size a global as well! This is getting pretty ugly


# GOOD MORNING CHRISTOPHER 3/1 Update

### Why is there pthread_exit(NULL) at tje bottom of the main???

#### Producers
I changed up the producer slightly, he wants us to create products on a FCFS basis so we have to force only one producer at once for all aspects of creating products.
##### Queue in Producers
The condition variable stuff for inserting into the queue is done. I tested it by makeing the queue size less than the total products and it dead locks the program when the queue hits that size (which is what should happen cause there are no consumers).

#### Consumer FCFS
I think I finished consumers on FCFS basis. Double check there arent any huge flaws in this traces please. Here is the printout:
~~~
╭─zach@pendrive ~/Documents/CS492/gitCS492/hw1  ‹master*›
╰─$ ./hw1 10 10 20 100 0 10 1
I'm a producer: 0 and I'm adding product: 0 to the queue, now size: 1
I'm a producer: 1 and I'm adding product: 1 to the queue, now size: 2
I'm a producer: 2 and I'm adding product: 2 to the queue, now size: 3
I'm a producer: 3 and I'm adding product: 3 to the queue, now size: 4
I'm a producer: 4 and I'm adding product: 4 to the queue, now size: 5
I'm a producer: 5 and I'm adding product: 5 to the queue, now size: 6
I'm a producer: 6 and I'm adding product: 6 to the queue, now size: 7
I'm a producer: 7 and I'm adding product: 7 to the queue, now size: 7
I'm Consumer: 0 and I consumed product: 0
I'm a producer: 8 and I'm adding product: 8 to the queue, now size: 8
I'm a producer: 9 and I'm adding product: 9 to the queue, now size: 8
I'm Consumer: 1 and I consumed product: 1
I'm Consumer: 2 and I consumed product: 2
I'm Consumer: 3 and I consumed product: 3
I'm Consumer: 4 and I consumed product: 4
I'm Consumer: 7 and I consumed product: 5
I'm Consumer: 5 and I consumed product: 6
I'm Consumer: 6 and I consumed product: 7
I'm Consumer: 8 and I consumed product: 8
I'm Consumer: 9 and I consumed product: 9
I'm a producer: 0 and I'm adding product: 10 to the queue, now size: 1
I'm a producer: 2 and I'm adding product: 11 to the queue, now size: 2
I'm a producer: 1 and I'm adding product: 12 to the queue, now size: 3
I'm a producer: 3 and I'm adding product: 13 to the queue, now size: 4
I'm a producer: 4 and I'm adding product: 14 to the queue, now size: 5
I'm a producer: 5 and I'm adding product: 15 to the queue, now size: 6
I'm a producer: 6 and I'm adding product: 16 to the queue, now size: 7
I'm a producer: 7 and I'm adding product: 17 to the queue, now size: 8
I'm Consumer: 0 and I consumed product: 10
I'm a producer: 8 and I'm adding product: 18 to the queue, now size: 8
I'm a producer: 9 and I'm adding product: 19 to the queue, now size: 9
I'm Consumer: 1 and I consumed product: 11
I'm Consumer: 2 and I consumed product: 12
I'm Consumer: 3 and I consumed product: 13
I'm Consumer: 4 and I consumed product: 14
I'm Consumer: 7 and I consumed product: 15
I'm Consumer: 5 and I consumed product: 16
I'm Consumer: 6 and I consumed product: 17
I'm Consumer: 8 and I consumed product: 18
I'm Consumer: 9 and I consumed product: 19
~~~

1. 3 at low quantum value low product
5 5 10 10 1 10 10
  * Producer Throughput:
    10.1568
    10.1783
    10.148
  * FCFS Throughput:
    10.148
    10.2113
    10.1638
  * RR Throughput:
    540.672
    540.952
    540.926
2. 3 high quantum low products
  10 10 10 10 1 5000 10
  * Producer Throughput:
    10.2221
    10.1807
    10.2256
  * FCFS Throughput:
    unchanged
  * RR Throughput:
   10.177
   10.2132
   10.2413

3. 3 low quantum high product
  10 10 100 10 1 10 10
  * Producer Throughput:
    439.769
    439.638
    439.727
  * FCFS Throughput:
    100.38
    100.431
    100.419
  * RR Throughput:
    4858.28
    4857.2
    4857.83
4. 3 high product high quantum
  * Producer Throughput:
    10.0358
    10.035
    10.0423
  * FCFS Throughput:

  * RR Throughput:
  100.4
  100.477
  100.477


## NEXT STEPS
Create the Consumer RR algo, the hard part.

![hp1](https://media.giphy.com/media/Cipj5jNsyTcxG/giphy.gif)

![mad potter](https://media.giphy.com/media/FWankyZyB90Ji/giphy.gif)
