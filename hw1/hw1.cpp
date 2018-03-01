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
pthread_mutex_t pidMutex;
pthread_mutex_t queueMutex;

/* Super special critical section stuff */
int pidCount = 0; //pid count for creating new product id's
std::queue<Product> pq; // Product Queue with no fixed size
int totProducts;


void *producer(void *threadid){
  int producerId;
  int  productId;
  producerId = (int)threadid;

  //mutex protected access to global pidCount
  pthread_mutex_lock(&pidMutex);
  while(pidCount != totProducts){
    productId = pidCount;
    pidCount++;
    pthread_mutex_unlock(&pidMutex); //allows other threads to make products

    Product p = Product(productId);
    pthread_mutex_lock(&queueMutex);
    pq.push(p);//add to queue
    cout << "I'm a producer: " << producerId << " and I'm adding product: " << p.get_id() << " to the queue, now size: " << pq.size() << endl;
    pthread_mutex_unlock(&queueMutex);
    usleep(100);
    pthread_mutex_lock(&pidMutex); // protects condition
  }
  pthread_mutex_unlock(&pidMutex);

  pthread_exit(NULL);
}

void *consumer0(){
//First come first serve
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

  if(!(s1 >> numCons)){
    cout << "Invalid Number" << endl;
    return -1;
  }
  if(!(s2 >> numProducer)){
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

  //cout << numCons  << ", " << numProducer << ", " << totalProducts << ", " << qSize << ", " << schedAlgo << ", " << quantum << ", " << seed << endl;

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
