#include <cstdlib>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#include "products.h"
using namespace std;

//global condition variables & mutexes
pthread_cond_t queueCount;
pthread_mutex_t pidMutex;   //used for making ensuring only one thread accessing pid at once
pthread_mutex_t producerMutex; //used to ensure FCFS in producers
pthread_mutex_t queueMutex; //used for protecting insertion and poping of the queue
pthread_mutex_t consumerMutex;

/* Super special critical section stuff */
int pidCount = 0; //pid count for creating new product id's
std::queue<Product> pq; // Product Queue with no fixed size
int totProducts;
int pConsumed =0;
// Global variables
int gQSize;


int fn(int n){
  if(n>1){
    return n*fn(n-1);
  }
  else {
    return 1;
  }
}

/*Producer:
FCFS Algorithm using Mutex to ensure only one producer is creating a Product at once
*/
void *producer(void *threadid){
  int producerId;
  int  productId;
  producerId = (intptr_t)threadid;


  //mutex protected access to global pidCount
  pthread_mutex_lock(&producerMutex);
  while(pidCount != totProducts){ //access pidCount inside pidMutex
    productId = pidCount;
    pidCount++;
    //pthread_mutex_unlock(&pidMutex); //no longer accessing or manipulating pidCont so we release

    //pthread_mutex_lock(&producerMutex);
    Product p = Product(productId); //create product inside producerMutex

    pthread_mutex_lock(&queueMutex); //about to access Queue so need queue mutex
    while(pq.size() == gQSize){   //condition variable, if the queue is full, wait to insert product
        pthread_cond_wait(&queueCount, &queueMutex);
    }
    pq.push(p); //add to queue
    cout << "I'm a producer: " << producerId << " and I'm adding product: " << p.get_id() << " to the queue, now size: " << pq.size() << endl;
    pthread_mutex_unlock(&queueMutex); //done insering into queue, release queue mutex
    pthread_cond_signal(&queueCount); //queue size has been increased, signal all waiting consumers
    pthread_mutex_unlock(&producerMutex); //release producer mutex so that during sleep other producers can execute
    //pthread_mutex_unlock(&pidMutex); //allows other threads to make products
    usleep(100);
    //pthread_mutex_lock(&producerMutex); //we are about to begin creating another product, must acquire producer mutex again to ensure FCFS
    pthread_mutex_lock(&producerMutex); // protects pidCount
  }
  //pthread_mutex_unlock(&pidMutex);
  pthread_mutex_unlock(&producerMutex); //done, release everything so other producer threads can exit

  pthread_exit(NULL); //exit
}




void *consumer0(void *threadid){
  int consumerID = (intptr_t)threadid;
  pthread_mutex_lock(&consumerMutex);
  while(pConsumed < totProducts){
    pthread_mutex_lock(&queueMutex);
    while(pq.size() == 0){
      pthread_cond_wait(&queueCount, &queueMutex);
    }
    Product currProduct = pq.front();
    pq.pop();
    pthread_mutex_unlock(&queueMutex);
    int dontcare = 0;
    for(int i = 0; i < currProduct.get_life(); i++){
      dontcare = fn(10);
    }
    pConsumed++;
    cout << "I'm Consumer: " << consumerID << " and I consumed product: " << currProduct.get_id() << endl;
    //DELETE PRODUCT?????
    pthread_mutex_unlock(&consumerMutex);
    usleep(100);
    pthread_mutex_lock(&consumerMutex);
  }
  pthread_mutex_unlock(&consumerMutex);
  pthread_exit(NULL);
}



void *Consumer1(){
//round robin
}
int main(int argc,char* argv[]){
  int numCons = 0;        //Param1 Number of Consumer Threads
  int numProducer = 0;    //Param2 Number of Producer Threads
  int totalProducts = 0;  //Param3 Number of Total Products to be produced and consumed
  int qSize = 0;          //Param4 Size of the queue, if 0 then unlimited queue size
  int schedAlgo = 0;      //Param5 0 -> FCFS   1->Round Robin
  int quantum = 0;        //Param6 Quantum Value for RR algo
  int seed = 0;           //Param7 Seed for random num generation

  if(argc != 8){
    cout << "Must Have all 7 arguments" << endl;
    return -1;
  }
  /*string streams for all parameters*/
  istringstream s1(argv[1]);
  istringstream s2(argv[2]);
  istringstream s3(argv[3]);
  istringstream s4(argv[4]);
  istringstream s5(argv[5]);
  istringstream s6(argv[6]);
  istringstream s7(argv[7]);

  /*conversions to ints and error checking that all inputs are numbers*/

  if(!(s1 >> numProducer)){
    cout << "Invalid Number" << endl;
    return -1;
  }
  if(!(s2 >> numCons)){
    cout << "Invalid Number" << endl;
    return -1;
  }
  //Had this change the global instead of the local
  if(!(s3 >> totProducts)){
    cout << "Invalid Number" << endl;
    return -1;
  }
  if(!(s4 >> qSize)){
    cout << "Invalid Number" << endl;
    return -1;
  }
  if(!(s5 >> schedAlgo)){
    cout << "Invalid Number" << endl;
    return -1;
  }
  if(!(s6 >> quantum)){
    cout << "Invalid Number" << endl;
    return -1;
  }
  if(!(s7 >> seed)){
    cout << "Invalid Number" << endl;
    return -1;
  }

  /*Error checking on specific mandatory conditions on the inputs*/
  if(numCons <= 0 || numProducer <=0 || totProducts <= 0 || quantum <= 0 || seed < 0 || qSize < 0){
    cout << "Inputs cannot be negative" << endl;
    return -1;
  }
  if(schedAlgo != 0 && schedAlgo != 1){
    cout << "schedule param must be 0 or 1" << endl;
    return -1;
  }
  /*Error Checking done*/ // I think

  /*Set the seed of all random numbers in the program*/
  srand(seed);
  gQSize = qSize;
  /* Creating Producers based on Param2*/
  pthread_t pThreads[numProducer];
  int rc;
  int i;
  for (i = 0; i < numProducer; i++){
    rc = pthread_create(&pThreads[i], NULL, producer, (void *)i);

    if (rc) {
      cout << "Error:unable to create thread," << rc << endl;
      exit(-1);
    }

  }
  pthread_exit(NULL);



  return 0;
}
