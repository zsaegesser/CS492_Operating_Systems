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
  long block_size;


public:
  block_group *head;
  long ldisk_size; //total number of blocks

  //write constructor that sets up empty Ldisk
  Ldisk(long disk_size,long tblock_size){
    //The total number of blocks is equal to the disk size divided by the block size input parameter (from pdf)
    block_size = tblock_size;
    ldisk_size = ceil(disk_size/tblock_size);
    block_group * starter = new block_group(0, pair<int,int> (0,ldisk_size-1),NULL);
    head = starter;
  }

  //fetch finds the next available block_group (returning it's address), unmarks it as 'used' and refreshes ldisk by combining the surrounding node groups
  long fetch(){
    //iterate through ldisk finding first unused space
    block_group * previous = NULL;
    block_group * current = this->head;
    long i = 0;
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

          return i*this->block_size;
        }else{
            //Current size is 1 and at front of Ldisk
          if(current->range.first == current->range.second && this->head == current) {
            //std::cout << "SECOND CONDITION" << '\n';
            //decrease beginning of next
            current->next->range.first-=1;
            this->head = current->next;
            //delete current node
            delete current;
            return i*this->block_size;
          }else if (current->range.first == current->range.second) {
            //std::cout << "THIRD CONDITION" << '\n';
            //increase previous used
            previous->range.second = current->next->range.second;
            //make previous->next set to current next
            previous->next = current->next->next;

            //delete current node?
            delete current->next;
            delete current;
            return i*this->block_size;
          }else{
            //std::cout << "FOURTH CONDITION" << '\n';
            previous->range.second +=1;
            current->range.first +=1;
            return i*this->block_size;
          }
        }

      //  int current_id = current->id;
      }else{
        //keep goin
        previous = current;
        current = current->next;
      }
      i++;
    }
    std::cout << "Ldisk was full!" << '\n';
    return -1;
  }

  void remove(long address){
    //find block_group containing block
    long block_number = address/this->block_size;
    block_group * previous = NULL;
    block_group * current = this->head;
    long i = 0;
    while (current!=NULL) {
      if(current->range.first <= block_number && curr->range.second >= block_number){
        //FOUND BLOCK
        if (!current->used) {
          std::cout << "THAT BLOCK IS FREE" << '\n' <<flush;
        }else if (current->range.first == current->range.second) {/* block group is of size 1*/
          if (previous == NULL && current->next == NULL) {
            //change it to freeeeeeeeeeeeeeeeeee!
            std::cout << "CONDITION 1" << '\n' << flush;
            current->used = 1;
          }else if(previous == NULL) { /*block group is at beginning of ldisk*/
            std::cout << "CONDITION 2" << '\n' << flush;
            current->next->range.first -1;
            //update head to current->next
            this->head = current->next;
            delete current;
          }else if(){/*block group at end of ldisk*/
            std::cout << "CONDITION 3" << '\n' << flush;
            previous->range.second +1;
            previous->next = null;
            delete current;
          }else{
            std::cout << "CONDITION 4" << '\n' << flush;
            //in middle of ldisk
            previous->range.second = current->next->range.second;
            previous->next = current->next->next;
            delete current->next;
            delete current;
          }
        }else if (current->range.first == block_number){
          //block found at head of used block_group
          if (previous == NULL) {
            std::cout << "CONDITION 5" << '\n' << flush;
            //block group is first block group
            //create new block group
            block_group new_group = new block_group(0,pair<int,int>(0,0),current);
            this->head = new_group;
            //update current range
            current->range.first +=1;
          }else{
            std::cout << "CONDITION 6" << '\n' << flush;
            //head of block group but not head of ldisk
            //update previous range.second by 1
            previous->range.second +=1;
            //increase current range.first by 1
            current->range.first +=1;
          }
        }else if(current->range.second == block_number){
          if (current->next== NULL ){
            std::cout << "CONDITION 7" << '\n' << flush;
            /* last block in block group at end of ldisk*/
            //create new block group
              //set its next to null
              //set it to not used
              // range from range.second of current to range.second of current
            block_group new_group = new block_group(0,pair<int,int>(block_number,block_number),NULL);
            //decrement current range.second by 1
            current->range.second -=1;
            //current->next = new node
            current->next = new_group;
          }else{
            std::cout << "CONDITION 8" << '\n' << flush;
            //last block in block group in middle of ldisk
            current->range.second -=1;
            current->next->range.first -=1;
          }
        }else{
          std::cout << "CONDITION 9" << '\n' << flush;
          //Double Middle: middle of Ldisk and block in middle of block group
          //create two new nodes
            //new used node
              //range from block_number+1 to current->range.second
              // used node -> next = current->next
            block_group new_used = new block_group(1,pair<int,int>(block_number+1,current->range.second),current->next);
            //new free middle node
              //range from block_number to block_number
              //next equal to other new node
            block_group new_free = new block_group(0,pair<int,int>(block_number,block_number),new_used);

            current->range.second = block_number-1;
            current->next = new_free;
        }
      }else{
        //keep goin
        previous = current;
        current = current->next;
        i++;
      }

    }
  }

  //write function to print entire Ldisk
  void print(){
    block_group * current = this->head;
    int i = 0;
    std::cout << "HEAD: " << this->head << '\n' << flush;
    while (current!=NULL) {
      std::cout << "Block Group: " << i <<"| Used: " << current->used << "| Range: " << current->range.first << ", " << current->range.second <<'\n' << flush;
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
