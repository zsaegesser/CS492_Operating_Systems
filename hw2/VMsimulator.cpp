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

//total number of swaps performed to measure performance
int swapcount = 0;

//Global counter used to check access time for pages in main memory
int count = 1;


//Funtion to complete fifo or lru depending on what is being stored in page
void fifo_lru(pair<int,int> pageID) {
  //find oldest page
  int min = all_procs[main_memory[0].first].page_table[main_memory[0].second].timestamp;
  int min_i;
  for (size_t i = 1; i < main_memory.size(); i++) {
    int pid = main_memory[i].first;
    int page_num = main_memory[i].second;
    //std::cout << all_procs[pid].page_table[page_num].timestamp << '\n';
    if (all_procs[pid].page_table[page_num].timestamp <= min) {
      min = all_procs[pid].page_table[page_num].timestamp;
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
  all_procs[pageID.first].page_table[pageID.second].timestamp = count;
  count++;
}

//Function to find next contiguous page for pre-paging
pair <int,int> next_contiguous(pair <int,int> page_id){
  int pid = page_id.first;
  int page_loc = page_id.second;
  //iterate through page table starting at pair with given page_id
  //return next non valid page id
  for (size_t i = page_loc+1; i < all_procs[pid].page_table.size(); i++) {
    if (!all_procs[pid].page_table[i].valid_bit) {
      return all_procs[pid].page_table[i].page_id;
    }
  }
  //sentinal value
  return page_id;
}

int main ( int argc, char *argv[] ){
  FILE * eplist;
  FILE * eptrace;
  int page_size;
  int page_algo;
  bool pre_flag;

  //Step 0: Arg Parsing/input error Checking:
  if ( argc != 6)
    cout<<"usage: "<< argv[0] <<" <plist> <ptrace> <page size> <page algorithm> <Pre-page flag +/->\n"; //update
  else {
    eplist = fopen(argv[1],"r");
    eptrace = fopen(argv[2],"r");
    //Check Files exist
    if (eplist == NULL) perror ("Error opening <plist>");
    if (eptrace == NULL) perror ("Error opening <ptrace>");

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
  ifstream plist(argv[1]); //nvm on fopen()
  if (plist.is_open())
  {
      int i=0;
      while (plist>>pID>>totalMemory)
      {
        //Create page table for each process in plist
        Process p;
        p.pid=i;
        p.createTable(pID,totalMemory,page_size);
        all_procs.push_back(p);
        i++;
      }
      plist.close();
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
      //make sure pages with timestamp 0 stay zero (so you don't check unused memory)
      // if (all_procs[i].page_table[j].timestamp == 0) {
      //   all_procs[i].page_table[j].timestamp = count;
      // }

      //this sets main meomry up with increasing timestamps to emulate a real system
      // that would take time to load each page
      all_procs[i].page_table[j].timestamp = count;
      count++; //if we wanted to have them all load at the same "time" we wouldn't increase the counter
    }
  }

  // Part 2: Implement three different page replacement algorithms

  //Read from ptrace and swap pages
  pID=0; //process ID
  int mem_loc; //memory location to be run
  pair<int,int> next; // contiguous page
  ifstream ptrace(argv[2]);
  if (ptrace.is_open())
  {
      int i=0;
      while ( ptrace>>pID>>mem_loc)
      {
        mem_loc = mem_loc - 1; //We made our pages start from 0 where ptrace starts at 1
        pair<int,int> pageID = make_pair(pID,mem_loc/page_size); //page containing read location
        std::cout << pID << " "<< mem_loc << " " << mem_loc/page_size << '\n';


        //check if page containing memory location has valid valid_bit
        if (all_procs[pID].page_table[mem_loc/page_size].valid_bit) {
          //Our page timestamp could represent access time or swap time
            //if it is lru this condition makes it access time.
          if (page_algo == 2) {
            all_procs[pID].page_table[mem_loc/page_size].timestamp = count;
            count++;
          }
          std::cout << "IN THER" << '\n';
        }else{
          //if no, PAGE FAULT!
            //add page fault!


          if (page_algo == 1 || page_algo == 2) {
            fifo_lru(pageID);
            swapcount++;
            if (pre_flag) { //prepaging
              next = next_contiguous(pageID);
              //if next equaled pageID that means there was no contiguous found
              if(next != pageID){
                std::cout << next.second << '\n';
                fifo_lru(next);
                swapcount++;
              }
            }
          }
          std::cout << "PAGE FAULT BABY" << '\n';
        }
        std::cin.ignore();
      }
      ptrace.close();
  }

  //Testing Printout:
  // for (size_t i = 0; i < proc_count; i++) {
  //   all_procs[i].printout();
  // }
  // for (size_t i = 0; i < main_memory.size(); i++) {
  //   std::cout << i << ": (" << main_memory[i].first << '\n';
  // }
  //std::cout << swapcount << '\n';



  //TODO:


  // 2) Implement three different page replacement algorithms
  // 3) Implement a variable page size
  // 4) Implement demand/pre-paging
  // 5) Record page swaps during a run
}
