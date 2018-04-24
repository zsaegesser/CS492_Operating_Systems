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
  pair<int,int> range;//inclusive
  block_group *next;
  bool used;

  block_group( bool isused, pair<int,int> brange, block_group *nextb){
    used = isused;
    range = brange;
    next = nextb;
  }
};

class Ldisk{
private:


public:
  block_group *head;
  int ldisk_size; //total number of blocks

  //write constructor that sets up empty Ldisk
  Ldisk(int disk_size,int block_size){
    //The total number of blocks is equal to the disk size divided by the block size input parameter (from pdf)
    ldisk_size = ceil(disk_size/block_size);
    block_group * starter = new block_group(0, pair<int,int> (0,ldisk_size),NULL);
    head = starter;
  }

  //fetch finds the next available block_group (returning it's address), unmarks it as 'used' and refreshes ldisk by combining the surrounding node groups
  long fetch(){
    //iterate through ldisk finding first unused space
    block_group * previous = NULL;
    block_group * current = this->head;
    while (current!=NULL) {
      if(!current->used) {
        //found free space
        if (current == this->head && current->next == NULL) {
          //std::cout << "FIRST CONDITION" << '\n';
          //create new block that is used
          block_group * new_block_group = new block_group(1,pair<int,int> (0,0),current);

          //update head to be the new block
          this->head = new_block_group;
          //update current range to be +1 to size
          current->range.first += 1;

          return 0;
        }else{
            //Current size is 1 and at front of Ldisk
          if(current->range.first == current->range.second && this->head == current) {
            //std::cout << "SECOND CONDITION" << '\n';
            //decrease beginning of next
            current->next->range.first-=1;
            this->head = current->next;
            //delete current node
            delete current;
            return 0;
          }else if (current->range.first == current->range.second) {
            //std::cout << "THIRD CONDITION" << '\n';
            //increase previous used
            previous->range.second = current->next->range.second;
            //make previous->next set to current next
            previous->next = current->next->next;

            //delete current node?
            delete current->next;
            delete current;
            return 0;
          }else{
            //std::cout << "FOURTH CONDITION" << '\n';
            previous->range.second +=1;
            current->range.first +=1;
            return 0;
          }
        }

      //  int current_id = current->id;
      }else{
        //keep goin
        previous = current;
        current = current->next;
      }
    }
    std::cout << "Ldisk was full!" << '\n';
    return -1;
  }

  void remove(long address){

  }

  //write function to print entire Ldisk
  void print(){
    block_group * current = this->head;
    int i = 0;
    while (current!=NULL) {
      std::cout << "Block Group: " << i <<" Used: " << current->used << " Range: " << current->range.first << ", " << current->range.second <<'\n' << flush;
      current = current->next;
      i++;
    }
    // if (start->range.second == ldisk_size) { //checks if top of range is equal to ldisk_size
    //   std::cout << start->id << '\n';
    //   return;
    // }else{
    //   std::cout << "Block Group: " << start->id << " Range: " << start->range.first << ", " << start->range.second <<'\n' << flush;
    //   this->print(start->next);
    // }
  }
};
