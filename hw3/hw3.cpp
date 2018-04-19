#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

using namespace std;


int main(int argc, char * const argv[]){

  // istringstream iss;
  string file_list_file;
  string dir_list_file;
  int disk_size = 0;
  int block_size = 0;

  // Basic error checking, more for us to not make silly mistake
  if(argc != 9){
    cout << "Wrong number of argumnets";
    return 1;
  }

  // Argument parsing
  for(int i = 1; i<9; i+=2){
    string arg(argv[i]);
    if(arg.compare("-f") == 0){
      string file_list(argv[i+1]);
      file_list_file = file_list;
    }
    else if(arg.compare("-d") == 0){
      string dir_list(argv[i+1]);
      dir_list_file = dir_list;
    }
    else if(arg.compare("-s") == 0){
      disk_size = atoi(argv[i+1]);
    }
    else if(arg.compare("-b") == 0){
      block_size = atoi(argv[i+1]);
    }
    else {
      cout << "Argument parameter " << arg << " is not recognized" << endl;
      return 1;
    }
  }

  // Testing argument parsing
  // cout << "File List: " << file_list_file << endl;
  // cout << "Dir List: " << dir_list_file << endl;
  // cout << "Disk Size: " << disk_size << endl;
  // cout << "Block Size: " << block_size << endl;

  //Do file stuff

  //Open terminal stuff

  




  return 0;

}
