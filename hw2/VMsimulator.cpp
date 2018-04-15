#include <cstdlib>
#include <sstream>
#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <fstream>
#include <vector>
#include "process.h"

using namespace std;

//Main Memory (global so I could make my algorithms into functions)
//Not sure if this is nessisary but the pdf explicitly said to make a structure for main memory
std::vector<pair<int,int> > main_memory;

//Vector of Processes from plist
std::vector<Process> all_procs;

//Global counter used to check access time for pages in main memory
int count = 1;



void fifo(pair<int,int> pageID) {
  //find oldest page
  int min = all_procs[main_memory[0].first].page_table[main_memory[0].second].last_accessed;
  int min_i;
  for (size_t i = 1; i < main_memory.size(); i++) {
    int pid = main_memory[i].first;
    int page_num = main_memory[i].second;
    //std::cout << all_procs[pid].page_table[page_num].last_accessed << '\n';
    if (all_procs[pid].page_table[page_num].last_accessed <= min) {
      min = all_procs[pid].page_table[page_num].last_accessed;
      min_i = i;
    }
  }

  //swap oldest page
  if (min != 0) { //0 means it was intially unused
    all_procs[main_memory[min_i].first].page_table[main_memory[min_i].second].valid_bit = 0; //un-validate old page
  }

  //put new page in main memory
  main_memory[min_i] = pageID;

  //validate & time new page
    all_procs[pageID.first].page_table[pageID.second].valid_bit = 1;
    all_procs[pageID.first].page_table[pageID.second].last_accessed = count;

  //std::cout << min_i << '\n';
}

int main ( int argc, char *argv[] ){
  FILE * plist;
  FILE * ptrace;
  int page_size;
  int page_algo;
  bool pre_flag;

  //Step 0: Arg Parsing/input error Checking:
  if ( argc != 6)
    cout<<"usage: "<< argv[0] <<" <plist> <ptrace> <page size> <page algorithm> <Pre-page flag +/->\n"; //update
  else {
    plist = fopen(argv[1],"r");
    ptrace = fopen(argv[2],"r");
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
      pre_flag = 1;
    }else if(arg5.compare("-")==0){
      pre_flag = 0;
    }else{
      std::cout << "Error: Invalid Pre-Paging Flag" << '\n';
      exit(-1);
    }
  }

  // Step1: Simulate a paging system

  //memory size is divided by page size because each page represents multiple memory locations
  main_memory.resize(512/page_size);

  //Read from plist and create pages / tables
  int pID; //process ID
  int totalMemory; //Total Memory accessed by process
  ifstream myfile(argv[1]);
  if (myfile.is_open())
  {
      int i=0;
      while ( myfile>>pID>>totalMemory)
      {
        //Create page table for each process in plist
        //all_tables.push_back(createTable(pID,totalMemory,page_size));
        Process p;
        p.pid=i;
        p.createTable(pID,totalMemory,page_size);
        all_procs.push_back(p);
        i++;
      }
      myfile.close();
  }

  //DEFAULT LOADING OF MAIN MEMORY:
  int proc_count = all_procs.size();
  int partition = main_memory.size()/proc_count; //divide main memory into even spaces

  for (size_t i = 0; i < proc_count; i++) {
    //std::cout << i*partition << '\n';
    for (size_t j = 0 ; j < partition; j++) {
      int current_spot = i*partition+j;
      //Smaller than default load:
      if (j == all_procs[i].page_table.size()) { //end of page table
        break;
      }
      //fill each "partition" of main memory with pages from each process
      main_memory[current_spot] = all_procs[i].page_table[j].page_id;

      //toggle valid bit in pages and add to timer
      all_procs[i].page_table[j].valid_bit = 1;
      all_procs[i].page_table[j].last_accessed = count;
      count++;
    }
  }

  // Part 2: Implement three different page replacement algorithms
  pair<int,int> test = make_pair(0,26);
  fifo(test);

  //Testing Printout:
  for (size_t i = 0; i < proc_count; i++) {
    all_procs[i].printout();
  }
  // for (size_t i = 0; i < main_memory.size(); i++) {
  //   std::cout << i << ": (" << main_memory[i].first << "," << main_memory[i].second << ") "<< all_procs[main_memory[i].first].page_table[main_memory[i].second].last_accessed << '\n';
  // }



  //TODO:


  // 2) Implement three different page replacement algorithms
  // 3) Implement a variable page size
  // 4) Implement demand/pre-paging
  // 5) Record page swaps during a run
}
