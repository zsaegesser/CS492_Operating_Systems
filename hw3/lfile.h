#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
// #include "ldisk.h"

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
  void add_bytes(long bytes, Ldisk * ldisk){
    long local_bytes = bytes;
    //TESTING
    // long temp = 8;
    // if(bytes > ldisk->size*ldisk->disk_size){
    //   return;
    // }
    if(this->get_number_of_blocks()*this->block_size >= (size+bytes)){
      // case where do NOT need to add new disk_block
      if(size ==0){
        //create block
        this->add_disk_block(ldisk->fetch());
        //TESTING
        // this->add_disk_block(temp);
      }
      size += bytes;
    }
    else {
      //normal case to add new blocks
      while(local_bytes != 0){
        if(local_bytes < this->block_size){
          //create last new block
          this->add_disk_block(ldisk->fetch());
          //TESTING
          // this->add_disk_block(temp);
          local_bytes = 0;
        }
        else{
          //create block
          this->add_disk_block(ldisk->fetch());
          //TESTING
          // this->add_disk_block(temp);
          local_bytes -= block_size;
        }
      }
      size+= bytes;
    }
  }

  void remove_bytes(long bytes, Ldisk * ldisk){
    if(size < bytes){
      cout << "You can't remove more bytes than the file has" << endl << flush;
      return;
    }
    if(size-(this->get_number_of_blocks()*(this->block_size-1)) > bytes){
      //case where you do NOT need to remove a disk_block
      if(this->size == 0){
        cout << "The file is already empty" << endl << flush;
      }
      else{
        if(this->size < bytes){
          cout << "You tried to remove more bytes than the file had. We set the file to size of 0, remove it if you like" << endl << flush;
          ldisk->remove(this->remove_last_disk_block());
          // TESTING
          // long temp = this->remove_last_disk_block();
          size = 0;
        }
        else {
          // cout << "MY guess is here" << endl << flush;
          size -= bytes;
        }
      }
    }
    else {
      //also check the (this->size < bytes, if so remove all nodes)
      //case where you need to remove a disk_block
      long local_bytes = bytes;
      while(local_bytes != 0){
        // cout << "Local Bytes: " << local_bytes << endl << flush;
        // this->print_lfile();
        if(local_bytes < size-((this->get_number_of_blocks()-1)*(this->block_size))){
          // cout << "Local Bytes: " <<  local_bytes << endl << flush;
          // cout << "First if" << endl << flush;
          //ldisk->remove(this->remove_last_disk_block());
          //TESTING
          // cout << "here" << endl << flush;
          // this->print_lfile();
          // long temp = this->remove_last_disk_block();
          // cout << "here2"  << endl << flush;
          // cout << "Case 1" << endl << flush;
          size -= local_bytes;
          local_bytes = 0;

        }
        else {
          // cout << "Case 2" << endl << flush;
          // cout << "Second if" << endl << flush;
          // cout <<"Bytes to Subtract: " << size-((this->get_number_of_blocks()-1)*(this->block_size)) << endl <<flush;
          local_bytes -= size-((this->get_number_of_blocks()-1)*(this->block_size));
          size -= size-((this->get_number_of_blocks()-1)*(this->block_size));
          ldisk->remove(this->remove_last_disk_block());
          //TESTING
          // cout << "good" << endl << flush;
          // long temp = this->remove_last_disk_block();
          // cout << "good2" << endl << flush;
        }
      }


    }
  }

  void add_disk_block(long blk_addr){
    if(blk_addr < 0){
      // cout << "You tried to add more bytes than were left in Ldisk, we filed the Ldisk then stopped" << endl << flush;
      return;
    }
    disk_block * current = this->head;
    if(current == NULL){
      this->head = new disk_block(blk_addr, NULL);
    }
    else{
      while(current->next != NULL){
        current = current->next;
      }
      current->next = new disk_block(blk_addr, NULL);
    }

  }

  //returns the block address of the removed disk_block
  long remove_last_disk_block(){

    disk_block * current = this->head;
    disk_block * prev = NULL;
    long block_address = 0;
    while(current->next != NULL){
      // cout << "Shouldnt have gotten here, last step" << endl <<flush;
      prev = current;
      current = current->next;
    }
    block_address = current->block_address;
    if(prev != NULL){
      prev->next = NULL;
    }
    else {
      this->head = NULL;
    }
    delete current;
    return block_address;
  }

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

};
