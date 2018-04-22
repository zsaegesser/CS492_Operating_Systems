#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>
#include <vector>
#include "node.h"
#include <time.h>

using namespace std;


// std::vector<Node> v;


void cd(char * directory){
  // Node new_node = find_node_by_name(directory);
  // if(is_some_child(curr_node, new_node)){
  //   curr_node = new_node;
  // }
  cout << directory << endl;
}




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

  while(true){
    char input[128]; //input char array
    cin.getline(input, 128); //get input from user
    vector<char *> command_line_inputs;

    //quit now if exit command
    if(strcmp(input, "exit") == 0){
      break;
    }

    //split the input by spaces
    char * split = strtok(input, " ");

    //go through the split input and save to a vector of char*'s
    while( split != NULL){
      command_line_inputs.push_back(split);
      split = strtok(NULL, " ");
    }
    if(strcmp(command_line_inputs[0], "cd") == 0){           //cd
      cout << "Hit cd" << endl;
      cd(command_line_inputs[1]);
    }
    else if(strcmp(command_line_inputs[0], "ls") == 0){      //ls
      cout << "hit ls" << endl;
      // ls();
    }
    else if(strcmp(command_line_inputs[0], "mkdir") == 0){   //mkdir
      cout << "Hit mkdir" << endl;
      // mkdir(command_line_inputs[1]);
    }
    else if(strcmp(command_line_inputs[0], "create") == 0){   //create
      cout << "Hit create" << endl;
      // create(command_line_inputs[1]);
    }
    else if(strcmp(command_line_inputs[0], "append") == 0){   //append
      cout << "Hit append" << endl;
      // append(command_line_inputs[1], command_line_inputs[2]);
    }
    else if(strcmp(command_line_inputs[0], "remove") == 0){   //remove
      cout << "Hit remove" << endl;
      // remove(command_line_inputs[1], command_line_inputs[2]);
    }
    else if(strcmp(command_line_inputs[0], "delete") == 0){   //delete
      cout << "Hit delete" << endl;
      // delete(command_line_inputs[1]);
    }
    else if(strcmp(command_line_inputs[0], "dir") == 0){   //dir
      cout << "Hit dir" << endl;
      // dir();
    }
    else if(strcmp(command_line_inputs[0], "prfiles") == 0){   //prfiles
      cout << "Hit prfiles" << endl;
      // prfiles();
    }
    else if(strcmp(command_line_inputs[0], "prdisk") == 0){   //prdisk
      cout << "Hit prdisk" << endl;
      // prdisk();
    }
    else {
      cout << "Command not recognized" << endl;
    }

  }




  return 0;

}
