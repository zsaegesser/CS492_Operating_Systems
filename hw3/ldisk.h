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
  pair<int,int> range; //range of blocks in block group
  block_group *next; //ponter to next group in linked list
  bool used; //To free or not to free

  //Constructor
  block_group( bool isused, pair<int,int> brange, block_group *nextb){
    used = isused;
    range = brange;
    next = nextb;
  }
};

class Ldisk{ //Class for creation of total Linked List
private:
  long block_size;

public:
  block_group *head;
  long ldisk_size; //total number of blocks

  //constructor that sets up empty Ldisk
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
    long result = 0;
    while (current!=NULL) {
      if(!current->used) {
        //found free space
        if (current->range.first == current->range.second) {
          //Current group is of size 1
          if (previous == NULL && current->next == NULL) {
            //only 1 block in single group!
            current->used = 1;
            return 0;
          } if (previous==NULL) {
            result = current->range.first*block_size;
            //Current group is head and size 1
            current->next->range.first-=1;
            this->head = current->next;
            delete current;
            return result;
          }else if (current->next == NULL) {
            //size 1 tail of ldisk
            result = current->range.first*block_size;
            previous->range.second +=1;
            previous->next = NULL;
            delete current;
            return result;
          }else{
            //size 1 center of ldisk
            result = current->range.first*block_size;
            previous->next = current->next->next;
            previous->range.second = current->next->range.second;
            delete current->next;
            delete current;
            return result;
          }
        }else if(previous==NULL){
          //head of ldisk and greater than 1
          result = current->range.first*block_size;
          //create new node range: 0,0 with next->current;
          block_group * new_group = new block_group(1, pair<int,int> (0,0),current);
          //increase current range.first
          current->range.first +=1;
          //update head
          this->head = new_group;
          return result;
        }else{
          //tail of ldisk or center of ldisk with group greater than 1
          result = current->range.first*block_size;
          previous->range.second +=1;
          current->range.first+=1;
          return result;
        }
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
    //find block_group containing block
    long block_number = address/this->block_size;
    block_group * previous = NULL;
    block_group * current = this->head;
    long i = 0;
    while (current!=NULL) {
      if(current->range.first <= block_number && current->range.second >= block_number){
        //FOUND BLOCK
        if (!current->used) {
          // std::cout << "THAT BLOCK IS FREE" << '\n' <<flush;
          return;
        }else if (current->range.first == current->range.second) {/* block group is of size 1*/
          if (previous == NULL && current->next == NULL) {
            //size 1 ldisk and 1 block
            current->used = 0;
            return;
          }else if(previous == NULL) { /*size 1 block group is at beginning of ldisk*/
            current->next->range.first -= 1;
            this->head = current->next;
            delete current;
            return;
          }else if(current->next == NULL){/* size 1 block group at end of ldisk*/
            previous->range.second +=1;
            previous->next = NULL;
            delete current;
            return;
          }else{
            //size 1 in middle of ldisk
            previous->range.second = current->next->range.second;
            previous->next = current->next->next;
            delete current->next;
            delete current;
            return;
          }
        }else if (current->range.first == block_number){
          //block found at head of used block_group
          if (previous == NULL){
            //block group is first block group
            //create new block group
            block_group * new_group = new block_group(0,pair<int,int>(0,0),current);
            this->head = new_group;
            //update current range
            current->range.first +=1;
            return;
          }else{
            //head of block group but not head of ldisk
            //update previous range.second by 1
            previous->range.second +=1;
            //increase current range.first by 1
            current->range.first +=1;
          }
        }else if(current->range.second == block_number){
          if (current->next== NULL ){
            /* last block in block group at end of ldisk*/
            //create new block
            block_group * new_group = new block_group(0,pair<int,int>(block_number,block_number),NULL);
            //decrement current range.second by 1
            current->range.second -=1;
            //current->next = new node
            current->next = new_group;
            return;
          }else{
            //last block in block group in middle of ldisk
            current->range.second -=1;
            current->next->range.first -=1;
            return;
          }
        }else{
          //Double Middle: middle of Ldisk and block in middle of block group
          //create two new nodes
            block_group * new_used = new block_group(1,pair<int,int>(block_number+1,current->range.second),current->next);
            block_group * new_free = new block_group(0,pair<int,int>(block_number,block_number),new_used);

            current->range.second = block_number-1;
            current->next = new_free;
            return;
        }
      }else{
        //keep goin
        previous = current;
        current = current->next;
        i++;
      }

    }
  }

  //write function to print entire Ldisk (for testing)
  void print(){
    block_group * current = this->head;
    int i = 0;
    std::cout << "HEAD: " << this->head->range.first << ", " << this->head->range.second << '\n' << flush;
    while (current!=NULL) {
      std::cout << "Block Group: " << i <<"| Used: " << current->used << "| Range: " << current->range.first << ", " << current->range.second <<'\n' << flush;
      current = current->next;
      i++;
    }
  }

  //printout of ldisk for footer
  void print_footprint(){
    block_group * current = this->head;
    int i = 0;
    while (current!=NULL) {
      if (current->used) {
        std::cout << "In use: " << current->range.first << "-" << current->range.second << '\n' << flush;
      }else{
        std::cout << "Free: " << current->range.first << "-" << current->range.second << '\n' << flush;
      }
      current = current->next;
      i++;
    }
  }
};
