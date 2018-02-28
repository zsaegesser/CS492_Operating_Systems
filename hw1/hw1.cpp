#include <cstdlib>
#include <sstream>
#include <iostream>
using namespace std;

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
  if(!(s3 >> totalProducts)){
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
  if(numCons <= 0 || numProducer <=0 || totalProducts <= 0 || quantum <= 0 || seed < 0 || qSize < 0){
    cout << "Inputs cannot be negative" << endl;
    return -1;
  }
  if(schedAlgo != 0 && schedAlgo != 1){
    cout << "schedule param must be 0 or 1" << endl;
    return -1;
  }
  /*Error Checking done*/ // I think


  //cout << numCons  << ", " << numProducer << ", " << totalProducts << ", " << qSize << ", " << schedAlgo << ", " << quantum << ", " << seed << endl;
  return 0;
}
