#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include "ldisk.h"

using namespace std;

struct disk_block{
  long block_address;
  disk_block * next;

  disk_block(long block_addr, disk_block * next){
    this->block_address = block_addr;
    this-> next = next;
  }
};

class Lfile{
public:
  long size; //number of bytes in the file
  long block_size;
  disk_block *head;
  //write constructor that sets up empty lfile

  Lfile(long size, disk_block * head, int block_size){
    this->size = size;
    this->head = head;
    this->block_size = block_size;
  }

  //get the number of blocks in the Lfile (length)
  int get_number_of_blocks(){
    int number = 0;
    disk_block * current = head;
    while(current != NULL){
      number+=1;
      current = current->next;
    }
    return number;
  }

  //the function that corresponds to append in the shell
  //takes number of bytes and a pointer to the Ldisk
  //adds that many bytes to the Lfile
  //adds bytes by adding disk_blocks
  //gets the memory address of the blocks with ldisk->fetch()
  void add_bytes(long bytes, Ldisk * ldisk){
    long local_bytes = bytes;    //local storage of bytes
    if(this->get_number_of_blocks()*this->block_size >= (size+bytes)){
      // case where do NOT need to add new disk_block
      if(size ==0){
        //create block
        this->add_disk_block(ldisk->fetch());
      }
      //add to the size of lfile
      size += bytes;
    }
    else {
      //case where you need to add at least one block
      while(local_bytes != 0){
        //this condition checks if the current Ldisk has fragmentation, if it does then fill the fragmentation first
        if((this->block_size-(this->size-((this->get_number_of_blocks()-1)*(this->block_size))))>0){
          //reduce local_bytes and increase size
          local_bytes -= (this->block_size-(this->size-((this->get_number_of_blocks()-1)*(this->block_size))));
          size += (this->block_size-(this->size-((this->get_number_of_blocks()-1)*(this->block_size))));
        }
        else if(local_bytes < this->block_size){ //checks if we only need to add one more block
          //create last new block
          this->add_disk_block(ldisk->fetch());
          //add to size then set local_bytes to 0 to trip while loop condition
          size+= local_bytes;
          local_bytes = 0;
        }
        else{ //this condition is when you are adding a NON last block (or last block that is filled perfectly)
          //create block
          this->add_disk_block(ldisk->fetch());
          //reduce local bytes and increase size of ldisk
          local_bytes -= block_size;
          size+= block_size;
        }
      }

    }
  }

  //function that corresponds to remove in the shell
  //removes bytes from lfile
  //takes the number of bytes to remove and the ldisk
  //removes blocks if necessary and deletes the blocks from ldisk
  void remove_bytes(long bytes, Ldisk * ldisk){
    if(size < bytes){
      cout << "You can't remove more bytes than the file has" << endl << flush;
      return;
    }
    if(size-((this->get_number_of_blocks()-1)*(this->block_size)) > bytes){
      //case where you do NOT need to remove a disk_block
      if(this->size == 0){
        cout << "The file is already empty" << endl << flush;
      }
      else{
        if(this->size < bytes){
          ldisk->remove(this->remove_last_disk_block());
          size = 0;
        }
        else {
          //reduce the size
          size -= bytes;
        }
      }
    }
    else {
      //case where you need to remove a disk_block
      long local_bytes = bytes; //set local storage of bytes
      while(local_bytes != 0){

        if(local_bytes < size-((this->get_number_of_blocks()-1)*(this->block_size))){
          //this condition is if you have removed enough blocks and now just need to remove some that is less than the block size
          size -= local_bytes;
          local_bytes = 0;
        }
        else {
          //case that removes a block
          //decrease local bytes by the appropriate amount, if first remove, then this is fragmentation, else this is just the block size
          local_bytes -= size-((this->get_number_of_blocks()-1)*(this->block_size));
          //reduce the size by the same amount
          size -= size-((this->get_number_of_blocks()-1)*(this->block_size));
          //remove block
          ldisk->remove(this->remove_last_disk_block());
        }
      }
    }
  }

  //function that adds a disk block to the linked list lfile
  //takes the block address of the new block
  void add_disk_block(long blk_addr){
    if(blk_addr < 0){
      //this case is if the Ldisk is full, ldisk->fetch returns -1 if the ldisk is full
      //do nothing, the ldisk->fetch function prints an error
      return;
    }
    //create a tempory node pointer to the head of the linked list,
    //this is essentially the iterative way to append a node to a single linked list
    disk_block * current = this->head;
    if(current == NULL){
      //if the linked list is empty, then the head is the new node
      this->head = new disk_block(blk_addr, NULL);
    }
    else{
      while(current->next != NULL){
        //this case is if you need continue to the next node, essentially current is not the last node in the linked list
        current = current->next;
      }
      //found the last node, now append the new node to the end of lfile
      current->next = new disk_block(blk_addr, NULL);
    }

  }

  //removes the last block in the lfile linked list
  //returns the block address of the removed disk_block
  long remove_last_disk_block(){
    //need to keep track of both the current node and the previous node
    disk_block * current = this->head;
    disk_block * prev = NULL;
    long block_address = 0;
    //iterative way to get to the end of the linked list
    while(current->next != NULL){
      //set up the prev and current for next iteration
      prev = current;
      current = current->next;
    }
    //when find the end, loop breaks

    //get teh block address of the node you are about to delete
    block_address = current->block_address;

    //if its not the first node then the previous now points to NULL (meaning it is the end of the lfild), otherwise the head now points to null
    if(prev != NULL){
      prev->next = NULL;
    }
    else {
      this->head = NULL;
    }
    // delete the node you removed
    delete current;
    return block_address;
  }

//Testing function used to print the lfile
  void print_lfile(){
    disk_block * current = this->head;
    int blocks = 0;
    cout << "Size: " << this->size << endl << flush;
    cout << "# Blocks: " << this->get_number_of_blocks() << endl << flush;
    while(current != NULL){
      cout << "Block " << blocks << " Addr: " << current->block_address <<endl << flush;
      current = current->next;
      blocks +=1;
    }
  }

  //corresponds to the prfiles shell command
  //prints out the addresses of all of the disk_blocks in lfile
  void print_block_addresses(){
    //iteratively scroll through the lfile
    disk_block * current = this->head;
    while(current != NULL){
      //print the block address
      cout << current->block_address << " " << flush;
      //set up next iteration
      current=current->next;
    }
  }

  //function that helps the prdisk shell command
  //returns the fragmentation of the lfile
  long frag_count(){
    //the fragmentation is essentially,
    //the fragmentaiton is esssentially the difference between the block size and how many bytes are stored in the last block
    //this math does that,    block size      -      (  (num_blocks-1)           *            block_size)
    //                      constant block size      # blocks not including last block       constant block size
    return (this->block_size-(this->size-((this->get_number_of_blocks()-1)*(this->block_size))));
  }
};
