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
// #include "ldisk.h"
// #include "lfile.h"

using namespace std;

//TODO
//deallocate on exit





std::vector<Node> globals;

Node * curr_node;
// Lfile * lfile;
Ldisk * ldisk;
int block_size = 0;
void cd_up(){
  if(strcmp(curr_node->name, globals[0].name)==0){
    cout << "Already in the Root directory" << endl << flush;
  }
  else{
      curr_node = curr_node->parent;
  }

}

void cd(char * directory){
  if(strcmp(directory, "..")==0){
    cd_up();
  }
  else{
    char search_char [sizeof(directory)+sizeof(curr_node->name)+2] = {};
    strcat(search_char, curr_node->name);
    strcat(search_char, "/");
    strcat(search_char, directory);
    Node * new_node = curr_node->find_node_by_name(search_char);
    if(new_node != NULL){
      curr_node = new_node;
    }
    else {
      cout << "Directory not found" << endl << flush;
    }
  }
}

void ls(){
  for(int i =0; i< curr_node->children.size(); i++){
    string curr_name = string(curr_node->children[i]->name);
    int j = curr_name.length();
    while (curr_name[j] != '/') {
      j--;
    }
    std::cout << curr_name.substr(j+1) << ' ' << flush;
    //cout << curr_node->children[i]->name << endl;
  }
  std::cout << '\n' << flush;
}

void mkdir(char * directory){
  char search_char [sizeof(directory)+sizeof(curr_node->name)+2]= {};
  strcat(search_char, curr_node->name);
  strcat(search_char, "/");
  strcat(search_char, directory);

  time_t timer;
  time(&timer);
  //create new node
  Node * new_node = new Node(0, search_char, 0, timer, NULL);
  curr_node->add_child(new_node);
  new_node->set_parent(curr_node);
}

void create_function(char * directory){
  char search_char [sizeof(directory)+sizeof(curr_node->name)+2] = {};
  strcat(search_char, curr_node->name);
  strcat(search_char, "/");
  strcat(search_char, directory);

  time_t timer;
  time(&timer);
  //create new node
  Node * new_node = new Node(1, search_char, 0, timer, new Lfile(0,NULL, block_size));
  //Don't need to update size here because it will always be 0
  curr_node->add_child(new_node);
  new_node->set_parent(curr_node);
}

void append_function(char * file, char * bytes){
  char search_char [sizeof(file)+sizeof(curr_node->name)+2] = {};
  strcat(search_char, curr_node->name);
  strcat(search_char, "/");
  strcat(search_char, file);

  Node * node_to_append = curr_node->find_node_by_name(search_char);
  if(node_to_append == NULL){
    cout << "File not found" << endl << flush;
  }
  else {
    if(!node_to_append->type){
      cout << "Cannot append to directory" << endl << flush;
    }
    else{
      node_to_append->f->add_bytes(atoi(bytes), ldisk);
      node_to_append->update_size();
      globals[0].increase_all_parents_size(curr_node, atoi(bytes));
      time_t timer;
      time(&timer);
      node_to_append->timestamp = timer;
    }
  }
}

void remove_function(char * file, char * bytes){
  char search_char [sizeof(file)+sizeof(curr_node->name)+2] = {};
  strcat(search_char, curr_node->name);
  strcat(search_char, "/");
  strcat(search_char, file);

  Node * node_to_append = curr_node->find_node_by_name(search_char);

  if(node_to_append == NULL){
    cout << "File not found" << endl << flush;
  }
  else {
    if(!node_to_append->type){
      cout << "Cannot append to directory" << endl << flush;
    }
    else{
      node_to_append->f->remove_bytes(atoi(bytes), ldisk);
      node_to_append->update_size();
      globals[0].decrease_all_parents_size(curr_node, atoi(bytes));
      time_t timer;
      time(&timer);
      node_to_append->timestamp = timer;
    }
  }
}




void delete_function(char * file_directory){
  char search_char [sizeof(file_directory)+sizeof(curr_node->name)+2] = {};
  strcat(search_char, curr_node->name);
  strcat(search_char, "/");
  strcat(search_char, file_directory);

  Node * new_node = curr_node->find_node_by_name(search_char);

  if(new_node != NULL){

    if(new_node->children.size() != 0){
      cout << "Can only delete empty directories" << endl <<flush;
    }
    else{
      long size = new_node->size;
      new_node->f->remove_bytes(size, ldisk);
      delete_node(new_node);

    }

  }
  else {
    cout << "Directory not found" << endl << flush;
  }
}


int main(int argc, char * const argv[]){

  // istringstream iss;
  int file_list_index;
  int dir_list_index;
  int disk_size = 0;

  //initialize globals  ----REMEMBER TO DELETE
  time_t timer;
  time(&timer);
  char * temp_char = new char[1];
  strcat(temp_char, "");
  globals.push_back(Node(0, temp_char, 0, timer, NULL));
  // globals.push_back(Node(0, "", 0, timer));
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
  ldisk = new Ldisk(disk_size, block_size);
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
            char * root_char = new char[1];
            strcat(root_char, ".");
            globals[0] = Node(0, root_char, 0, timer, NULL);
            //set the roots parent
            globals[0].set_parent(NULL);

            continue;
          }else{ //First level node
            //create timer
            time_t timer;
            time(&timer);
            //create new node
            Node * temp_node = new Node(0, line_char, 0, timer, NULL);

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
        Node * new_node = new Node(0, line_char, 0, timer, NULL);

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

      char *parent_char = new char[parent.length() + 1  ];
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
          Node * temp_node = new Node(1, eleven_char, seven, timer, new Lfile(0,NULL, block_size));
          temp_node->f->add_bytes(seven, ldisk);
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
      Node * new_node = new Node(1, eleven_char, seven, timer, new Lfile(0,NULL, block_size));
      new_node->f->add_bytes(seven, ldisk);
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

  //for testing, print the built tree
  // print_tree(globals[0],0);


  //Open terminal stuff
  //set the current directory to the root
  // globals[1] = globals[0];
  curr_node = &globals[0];
  while(true){
    cout << "cats&dogs:  " << curr_node->name << " >>> " << flush;
    char input[128]; //input char array
    cin.getline(input, 128); //get input from user
    vector<char *> command_line_inputs;

    //quit now if exit command
    if(strcmp(input, "exit") == 0){
      delete_tree(&globals[0]);
      delete ldisk;
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
      if(command_line_inputs.size() != 2){
        cout << "Wrong number of parameters" << endl << flush;
      }
      else{
        cd(command_line_inputs[1]);
      }

    }
    else if(strcmp(command_line_inputs[0], "ls") == 0){      //ls
      ls();
    }
    else if(strcmp(command_line_inputs[0], "mkdir") == 0){   //mkdir
      if(command_line_inputs.size() != 2){
        cout << "Wrong number of parameters" << endl << flush;
      }
      else {
        mkdir(command_line_inputs[1]);
      }

    }
    else if(strcmp(command_line_inputs[0], "create") == 0){   //create
      if(command_line_inputs.size() != 2){
        cout << "Wrong number of parameters" << endl << flush;
      }
      else {
        create_function(command_line_inputs[1]);
      }

    }
    else if(strcmp(command_line_inputs[0], "append") == 0){   //append
      if(command_line_inputs.size() != 3){
        cout << "Wrong number of parameters" << endl << flush;
      }
      else {
        append_function(command_line_inputs[1], command_line_inputs[2]);
      }
    }
    else if(strcmp(command_line_inputs[0], "remove") == 0){   //remove
      if(command_line_inputs.size() != 3){
        cout << "Wrong number of parameters" << endl << flush;
      }
      else {
        remove_function(command_line_inputs[1], command_line_inputs[2]);
      }
    }
    else if(strcmp(command_line_inputs[0], "delete") == 0){   //delete
      if(command_line_inputs.size() != 2){
        cout << "Wrong number of parameters" << endl << flush;
      }
      else {
        delete_function(command_line_inputs[1]);
      }
    }
    else if(strcmp(command_line_inputs[0], "dir") == 0){   //dir
      print_treeB(&globals[0]);
    }
    else if(strcmp(command_line_inputs[0], "prfiles") == 0){   //prfiles
      print_files(&globals[0]);
    }
    else if(strcmp(command_line_inputs[0], "prdisk") == 0){   //prdisk
      ldisk->print_footprint();
      cout <<"fragmentation: " << total_fragmentation(&globals[0]) << " bytes" <<endl << flush;
    }
    else {
      cout << "Command not recognized" << endl << flush;
    }

  }
  return 0;
}
