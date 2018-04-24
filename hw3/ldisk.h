#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include <math.h>

using namespace std;

struct block_group{
  int id;
  pair<int,int> range;//inclusive
  block_group *next;
  bool used;

  block_group(int bid, bool isused, pair<int,int> brange, block_group *nextb){
    id = bid;
    used = isused;
    range = brange;
    next = nextb;
  }
};

class Ldisk{
private:


public:
  block_group *head, *tail;
  int ldisk_size; //total number of blocks

  //write constructor that sets up empty Ldisk
  Ldisk(int disk_size,int block_size){
    //The total number of blocks is equal to the disk size divided by the block size input parameter (from pdf)
    ldisk_size = ceil(disk_size/block_size);
    block_group * starter = new block_group(0,0, pair<int,int> (0,ldisk_size),NULL);
    head = starter;
    tail = starter;
  }

  //fetch finds the next available block_group (returning it's address), unmarks it as 'used' and refreshes ldisk by combining the surrounding node groups
  long fetch(){
    //iterate through ldisk finding first unused space
    block_group * previous = NULL;
    block_group * current = this->head;
    while (current!=NULL) {
      if (!current->used) {
        //found free space
        if (current == this->head) {
          std::cout << "FIRST CONDITION" << '\n';
          //create new block that is used
            //range would be 0-0
            //set used
            //set next to current block_group
          block_group * new_block_group = block_group(0,1,pair<int,int> (0,0),previous);

          //update head to be the new block
          this->head = new_block_group;
          //update current range to be +1 to size
          current->range.first += 1;
          //update current block_group id
          current->id +=1;
        }
        // else if (/*full*/) {
        //   /* ... */
        // }else{
        //   if (/*current size is 1*/) {
        //
        //   }else{
        //     previous->range.second +=1;
        //     current->range.first +=1;
        //   }
        // }

      //  int current_id = current->id;
      }else{
        //keep goin
        previous = current;
        current = current->next;
      }
    }
    //calculate first address in free block_group
    //smush surrounding block_groups
      //make preceding group point to the group after ...
    return 0;
  }

  //write function to print entire Ldisk
  void print(block_group * start){
    if (start->range.second == ldisk_size) { //checks if top of range is equal to ldisk_size
      std::cout << start->id << '\n';
      return;
    }else{
      std::cout << start->id << '\n';
      this->print(start->next);
    }
  }
};
