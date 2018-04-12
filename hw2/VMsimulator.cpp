#include <cstdlib>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <fstream>


using namespace std;

int main ( int argc, char *argv[] ){
  //ERROR CHECKING:
  int page_size;
  int page_algo;
  bool pre_flag;


  if ( argc != 6)
    cout<<"usage: "<< argv[0] <<" <plist> <ptrace> <page size> <page algorithm> <Pre-page flag +/->\n"; //update
  else {
    FILE *plist = fopen(argv[1],"r");
    FILE *ptrace = fopen(argv[2],"r");
    //Check Files exist
    if (plist == NULL) perror ("Error opening <plist>");
    if (ptrace == NULL) perror ("Error opening <ptrace>");

    page_size = atoi(argv[3]);
    if (page_size == 0) { //atoi turns anything other than an int to 0
       std::cout << "Error: Invalid Page Size" << '\n';
       exit(-1);
    }

    std::string arg4(argv[4]);
    if(arg4.compare("FIFO") == 0){
      page_algo = 1;
    }else if(arg4.compare("LRU") == 0){
      page_algo = 2;
    }else if(arg4.compare("Clock") == 0){
      page_algo = 3;
    }else{
      std::cout << "Error: Invalid Page Algorithm" << '\n';
      exit(-1);
    }

    std::string arg5(argv[5]);
    if(arg5.compare("+")==0){
      pre_flag = true;
    }else if(arg5.compare("-")==0){
      pre_flag == false;
    }else{
      std::cout << "Error: Invalid Pre-Paging Flag" << '\n';
      exit(-1);
    }
  }

  //TODO:
  // 1) Simulate a paging system
  // 2) Implement three different page replacement algorithms
  // 3) Implement a variable page size
  // 4) Implement demand/pre-paging
  // 5) Record page swaps during a run
}
