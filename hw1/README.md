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



![mad potter](https://media.giphy.com/media/FWankyZyB90Ji/giphy.gif)
