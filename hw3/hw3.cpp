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
#include <fstream>

using namespace std;


std::vector<Node> globals;


void cd(char * directory){
  // Node new_node = find_node_by_name(directory);
  // if(is_some_child(curr_node, new_node)){
  //   curr_node = new_node;
  // }
  cout << directory << endl;
}




int main(int argc, char * const argv[]){

  // istringstream iss;
  int file_list_index;
  int dir_list_index;
  int disk_size = 0;
  int block_size = 0;

  //initialize globals  ----REMEMBER TO DELETE
  time_t timer;
  time(&timer);
  globals.push_back(Node(0, "", 0, timer));
  globals.push_back(Node(0, "", 0, timer));
  // Basic error checking, more for us to not make silly mistake
  if(argc != 9){
    cout << "Wrong number of argumnets";
    return 1;
  }

  // Argument parsing
  for(int i = 1; i<9; i+=2){
    string arg(argv[i]);
    if(arg.compare("-f") == 0){
      //string file_list(argv[i+1]);
      file_list_index = i+1;
    }
    else if(arg.compare("-d") == 0){
      //string dir_list(argv[i+1]);
      dir_list_index = i+1;
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

  //Read from Directory and File Lists to populate directory tree
  string line ="";
  ifstream dir_list(argv[dir_list_index]);
  if (dir_list.is_open())
  {
      while (dir_list>>line) //find size of current directory name
      {
        int i = line.length();
        while (line[i] != '/') {
          i--;
        }
        string parent = line.substr(0,i);

        // convert to char * for use with Node class
        char *line_char = new char[line.length() + 1];
        strcpy(line_char, line.c_str());

        char *parent_char = new char[parent.length() + 1];
        strcpy(parent_char, parent.c_str());


        if (parent.compare(".")==0) { // if node is either root or first level
          if (line.compare("./")==0) { // if node is root
            //create timestamp
            time_t timer;
            time(&timer);
            //create root node
            globals[0] = Node(0, line_char, 0, timer);
            //set the roots parent
            globals[0].set_parent(NULL);

            continue;
          }else{ //First level node
            //create timer
            time_t timer;
            time(&timer);
            //create new node
            Node * temp_node = new Node(0, line_char, 0, timer);

            //set parent of new node to the root
            temp_node->set_parent(&globals[0]);
            //add new node to child of root
            globals[0].add_child(temp_node);

            continue;
          }
          //delete the line char *
          delete [] line_char;
        }

        //find the parent node of the current node adding
        Node * parent_node = globals[0].find_node_by_name(parent_char);


        time_t timer;
        time(&timer);
        Node * new_node = new Node(0, line_char, 0, timer);

        //add child to the parent
        parent_node->add_child(new_node);
        //set it's parent
        new_node->set_parent(parent_node);

        //globals[0].increase_all_parents_size(parent_node, 1);

        //delete all the char *
        delete [] line_char;
        delete [] parent_char;
      }
      dir_list.close();
  }
  //print_tree(globals[0],0);

  // read from file_list and add all files to directory tree
  ifstream file_list(argv[file_list_index]);
  string one,two,three,four,five,six,eight,nine,ten,eleven;
  int seven;
  if (file_list.is_open())
  {
    while (file_list>>one>>two>>three>>four>>five>>six>>seven>>eight>>nine>>ten>>eleven)
    {
      int i = eleven.length();
      while (eleven[i] != '/') {
        i--;
      }
      string parent = eleven.substr(0,i);

      // convert to char * for use with Node class
      char *eleven_char = new char[eleven.length() + 1];
      strcpy(eleven_char, eleven.c_str());

      char *parent_char = new char[parent.length() + 1];
      strcpy(parent_char, parent.c_str());


      if (parent.compare(".")==0) { // if node is either root or first level
        if (eleven.compare("./")==0) { // if node is root
          // do nothing for root (already exists)
          continue;
        }else{ //First level node
          //create timer
          time_t timer;
          time(&timer);
          //create new node
          Node * temp_node = new Node(0, eleven_char, seven, timer);

          //set parent of new node to the root
          temp_node->set_parent(&globals[0]);
          //add new node to child of root
          globals[0].add_child(temp_node);

          globals[0].increase_all_parents_size(&globals[0], seven);

          continue;
        }
        delete [] eleven_char;
      }

      //find the parent node of the current node adding
      Node * parent_node = globals[0].find_node_by_name(parent_char);

      time_t timer;
      time(&timer);
      Node * new_node = new Node(0, eleven_char, seven, timer);

      //add child to the parent
      parent_node->add_child(new_node);
      //set it's parent
      new_node->set_parent(parent_node);

      globals[0].increase_all_parents_size(parent_node, seven);

      //delete all the char *
      delete [] eleven_char;
      delete [] parent_char;
    }
    file_list.close();
  }

  print_tree(globals[0],0);
  //ZACH: If you are wondering why file_list size is zero, open file_list!

  //Open terminal stuff

  while(true){
    char input[128]; //input char array
    cin.getline(input, 128); //get input from user
    vector<char *> command_line_inputs;

    //quit now if exit command
    if(strcmp(input, "exit") == 0){
      break;
    }
    if(strcmp(input, "") == 0){
      cout << "Not Valid Command" << endl;
      continue;
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
