#include <cstdlib>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#include <ctime>
#include <chrono>
#include <sys/time.h>
#include "products.h"
using namespace std;

//global condition variables & mutexes
pthread_cond_t queueCount;
//pthread_mutex_t pidMutex;   //used for making ensuring only one thread accessing pid at once
pthread_mutex_t producerMutex; //used to ensure FCFS in producers
pthread_mutex_t queueMutex; //used for protecting insertion and poping of the queue
pthread_mutex_t consumerMutex;


/* Super special critical section stuff */
int pidCount = 0; //pid count for creating new product id's
std::queue<Product> pq; // Product Queue with no fixed size
int pConsumed =0;
int consumerCountID;
// Global variables
int gQSize;
int totProducts;
int gQuantum;
int totalConsumers;
bool gTestFlag;
//timing
struct timeval tv;
double start;



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
  int productId;
  producerId = (intptr_t)threadid;


  //mutex protected
  pthread_mutex_lock(&producerMutex);
  while(pidCount != totProducts){ //access pidCount inside producerMutex
    productId = pidCount;
    pidCount++;

    Product p = Product(productId); //create product inside producerMutex

    pthread_mutex_lock(&queueMutex); //about to access Queue so need queue mutex
    while(pq.size() == gQSize){   //condition variable, if the queue is full, wait to insert product
        pthread_cond_wait(&queueCount, &queueMutex);
    }
    pq.push(p); //add to queue
    if(!gTestFlag){
      cout << "I'm a producer: " << producerId << " and I'm adding product: " << p.get_id() << " to the queue, now size: " << pq.size() << endl << flush;
    }
    pthread_mutex_unlock(&queueMutex); //done insering into queue, release queue mutex
    pthread_cond_signal(&queueCount); //queue size has been increased, signal all waiting consumers
    pthread_mutex_unlock(&producerMutex); //release producer mutex so that during sleep other producers can execute
    usleep(100000);
    pthread_mutex_lock(&producerMutex); // protects pidCount
  }
  pthread_mutex_unlock(&producerMutex); //done, release everything so other producer threads can exit

  pthread_exit(NULL); //exit
}




void *consumer0(void *threadid){

  int consumerID = (intptr_t)threadid;
  pthread_mutex_lock(&consumerMutex); //acqure the consumer mutex to ensure only one consumer executing at a time
  while(pConsumed < totProducts){     //continue consuming products until consumed total amount of products
    pthread_mutex_lock(&queueMutex);  //acqure queue mutex to ensure only one thread (consumer or producer) is accessing the queue at once
    while(pq.size() == 0){            //conditional wait, if the queue is empty, then wait to be signaled by the producers when a product is added
      pthread_cond_wait(&queueCount, &queueMutex);
    }
    Product currProduct = pq.front(); //get the product at the front of the queue (at this point we are guarenteed there will be a product in the queue)
    pq.pop();                         //remove the first item in the queue, front only retrieves, doesnt pop
    pthread_mutex_unlock(&queueMutex);//done with queue manipulation, release it so a producer may now add to the queue
    pthread_cond_signal(&queueCount); //signal any producers waiting on a full queue that the queue is no longer full
    int dontcare = 0;
    for(int i = 0; i < currProduct.get_life(); i++){
      dontcare = fn(10);              //run down the life of the product completely, dont care about the result
    }
    pConsumed++;                      //increase the number of total producers,
    if(!gTestFlag){
      cout << "I'm Consumer: " << consumerID << " and I consumed product: " << currProduct.get_id() << endl << flush;
    }
    //DELETE PRODUCT?????
    pthread_mutex_unlock(&consumerMutex); //release the consumer mutex so that other consumers can begin consuming
    usleep(100000);
    pthread_mutex_lock(&consumerMutex);   //acqure the consumer mutex to ensure the FCFS scheduling
  }
  pthread_mutex_unlock(&consumerMutex);
  pthread_exit(NULL);
}



void *consumer1(void *threadid){
  int consumerID = (intptr_t)threadid;
  pthread_mutex_lock(&consumerMutex);
  while(pConsumed < totProducts){
    pthread_mutex_lock(&queueMutex);
    while(pq.size() == 0){
      pthread_cond_wait(&queueCount, &queueMutex);
    }
    Product *currProduct = &(pq.front());
    pthread_mutex_unlock(&queueMutex);
    if(gTestFlag){
      gettimeofday(&tv, NULL);
      double t1=tv.tv_sec+(tv.tv_usec/1000000.0);
      cout << "0," << currProduct->get_id() << "," << 1000*(t1-start) << "\n" << flush;
    }
    if(gQuantum < currProduct->get_life()){
      for(int i = 0; i < gQuantum; i++){
        int dontcare = fn(10);
      }
      currProduct->sub_life(gQuantum);
      if(!gTestFlag){
        cout << "Consumer: " << consumerID << " consumed " << gQuantum << " life of product " << currProduct->get_id() << " life left "<< currProduct->get_life()<< endl << flush;
      }
      else {
        gettimeofday(&tv, NULL);
        double t1=tv.tv_sec+(tv.tv_usec/1000000.0);
        cout << "1," << currProduct->get_id() << "," << 1000*(t1-start) << "\n" << flush;

      }
    }
    else {
      if(gTestFlag){
        gettimeofday(&tv, NULL);
        double t1=tv.tv_sec+(tv.tv_usec/1000000.0);
        cout << "0," << currProduct->get_id() << "," << 1000*(t1-start) << "\n" << flush;
      }
      for(int i = 0; i < currProduct->get_life(); i++){
        int dontcare = fn(10);
      }
      if(gTestFlag){
        gettimeofday(&tv, NULL);
        double t1=tv.tv_sec+(tv.tv_usec/1000000.0);
        cout << "1," << currProduct->get_id() << "," << 1000*(t1-start) << "\n" << flush;
      }
      if(!gTestFlag){
        cout << "Consumer: " << consumerID << " consumed " << currProduct->get_life() << " life of product " << currProduct->get_id() << endl << flush;
        cout << "Product: " << currProduct->get_id() << " completed" << endl << flush;
      }
      else {
        gettimeofday(&tv, NULL);
        double t1=tv.tv_sec+(tv.tv_usec/1000000.0);
        double prodStart = currProduct->get_time_stamp();
        cout << "2,"<< currProduct->get_id() << "," << 1000*(prodStart-start) << "," << 1000*(t1-start) << "\n" << flush;
      }
      pthread_mutex_lock(&queueMutex);
      pq.pop();
      pthread_cond_signal(&queueCount);
      pthread_mutex_unlock(&queueMutex);
      pConsumed++;
    }
    consumerCountID++;
    if(consumerCountID == totalConsumers){
      consumerCountID = 0;
    }

    pthread_mutex_unlock(&consumerMutex);
    usleep(100000);
    pthread_mutex_lock(&consumerMutex);
  }
  pthread_mutex_unlock(&consumerMutex);
  pthread_exit(NULL);
}



int main(int argc,char* argv[]){

  int numCons = 0;        //Param1 Number of Consumer Threads
  int numProducer = 0;    //Param2 Number of Producer Threads
  int totalProducts = 0;  //Param3 Number of Total Products to be produced and consumed
  int qSize = 0;          //Param4 Size of the queue, if 0 then unlimited queue size
  int schedAlgo = 0;      //Param5 0 -> FCFS   1->Round Robin
  int quantum = 0;        //Param6 Quantum Value for RR algo
  int seed = 0;           //Param7 Seed for random num generation

  if(argc != 8 && argc !=9){
    cout << "Must Have all 7 arguments" << endl;
    return -1;
  }
  if(argc == 9){
    int testNum;
    istringstream s8(argv[8]);
    if(!(s8 >> testNum)){
      cout << "Must have only 7 arguments" << endl;
      return -1;
    }
    if(testNum == 7){
        gTestFlag = true;
    }
    else{
      cout << "Bad arguments" << endl;
    }
  }
  else{
    gTestFlag =false;
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
  struct timeval tim;
  gettimeofday(&tim, NULL);
  start = tim.tv_sec+(tim.tv_usec/1000000.0);
  /*Set the seed of all random numbers in the program*/
  srand(seed);
  gQuantum = quantum;
  gQSize = qSize;
  totalConsumers = numCons;
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
  /*FCFS create threads consumers */
  pthread_t cThreads[numCons];
  if(schedAlgo == 0){
    int rc2;
    int i2;
    for (i2 = 0; i2 < numCons; i2++){

      rc2 = pthread_create(&cThreads[i2], NULL, consumer0, (void *)i2);

      if (rc2) {
        cout << "Error:unable to create thread," << rc2 << endl;
        exit(-1);
      }

    }
  }
  else if(schedAlgo == 1){ //Roud Robin create threads consumers
    int rc2;
    int i2;
    for (i2 = 0; i2 < numCons; i2++){

      rc2 = pthread_create(&cThreads[i2], NULL, consumer1, (void *)i2);

      if (rc2) {
        cout << "Error:unable to create thread," << rc2 << endl;
        exit(-1);
      }

    }
  }

  pthread_exit(NULL);



  return 0;
}
