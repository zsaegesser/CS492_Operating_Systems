#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include "lfile.h"
#include <queue>
using namespace std;


class Node {
public:
  char * name;
  int size;
  time_t timestamp;
  Node * parent;
  std::vector<Node *> children;
  bool type; //0 for directory, 1 for file
  Lfile * f;

  Node(bool node_type, char * node_name, int node_size, time_t node_timestamp, Lfile * lfile){
    type = node_type;

    name = new char[strlen(node_name)];
    strcpy(name, node_name);

    size = node_size;
    timestamp = node_timestamp;
    f = lfile;
  }

  void update_size(){
    size = f->size;
  }
  //set parent of node function was called on
  void set_parent(Node * node_parent){
    parent = node_parent;
  }

  // add child (new node) to the node funciton was called on
  void add_child(Node * new_node_child){
    children.push_back(new_node_child);
  }

  // called on root, given name of node you want
  Node* find_node_by_name(char * search_name){
    if((strcmp(name, search_name) == 0)){
      // cout << "Found Node" << endl;
      return this;
    }
      for(int i = 0; i < children.size(); i++){
        Node * p = children[i]->find_node_by_name(search_name);
        if(p != NULL){
          return p;
        }
      }
      return NULL;
  }

  //increase the size of a file
  void increase_size(int inc_size){
    size += inc_size;
  }

  void decrease_size(int dec_size){
    size -= dec_size;
  }
  //increase the size of all parents of the parent_node, CALLED ON ROOT
  //UNTESTED
  void increase_all_parents_size(Node * parent_node, int inc_size){
    if(parent_node != NULL){
      parent_node->increase_size(inc_size);
      increase_all_parents_size(parent_node->parent, inc_size);
    }
  }

  void decrease_all_parents_size(Node * parent_node, int dec_size){
    if(parent_node != NULL){
      parent_node->decrease_size(dec_size);
      decrease_all_parents_size(parent_node->parent, dec_size);
    }
  }

};

//print the tree in a readable format
static void print_tree(Node &curr_node, int level){
  for(int i = 0; i < level; i++){
    cout << "   ";
  }
  cout << curr_node.name << " size: " << curr_node.size << endl;

  for(int i = 0; i < curr_node.children.size(); i++){
    print_tree(*curr_node.children[i], level+1);
  }
}

static void print_treeB(Node * root_node){
  queue<Node*>  print_queue;  //queue of nodes
  print_queue.push(root_node);//add root to queue
  while (print_queue.size() != 0) {/*queue isn't empty */
    Node * curr_node = print_queue.front();
    for (size_t i = 0; i < curr_node->children.size(); i++) {/*size of children vector at head of queue*/
      //push_back child node to queue
      print_queue.push(curr_node->children[i]);

    }
    //print current node
    std::cout << curr_node->name << '\n' <<flush;
    //pop queue
    print_queue.pop();
  }
}

static long total_fragmentation(Node * root_node){
  queue<Node*>  print_queue;  //queue of nodes
  print_queue.push(root_node);//add root to queue
  long j =0;
  while (print_queue.size() != 0) {/*queue isn't empty */
    Node * curr_node = print_queue.front();
    for (size_t i = 0; i < curr_node->children.size(); i++) {/*size of children vector at head of queue*/
      //push_back child node to queue
      print_queue.push(curr_node->children[i]);

    }
    //print current node
    //std::cout << curr_node->name << '\n' <<flush;
    if (curr_node->type) {
      //is a file
      j = j+ curr_node->f->frag_count();
    }
    //pop queue
    print_queue.pop();
  }
  return j;
}

static void print_files(Node * root_node){
  queue<Node*>  print_queue;  //queue of nodes
  print_queue.push(root_node);//add root to queue
  // long j =0;
  while (print_queue.size() != 0) {/*queue isn't empty */
    Node * curr_node = print_queue.front();
    for (size_t i = 0; i < curr_node->children.size(); i++) {/*size of children vector at head of queue*/
      //push_back child node to queue
      print_queue.push(curr_node->children[i]);

    }
    //print current node
    //std::cout << curr_node->name << '\n' <<flush;
    if (curr_node->type) {
      //is a file
      cout << "Path: " << curr_node->name << "  Size: " << curr_node->size << endl << flush;
      curr_node->f->print_block_addresses();
      cout << endl << flush;
    }
    //pop queue
    print_queue.pop();
  }
}


// // is_some_child(a, b) => returns TRUE if a is a child of b, false otherwise
// static bool is_some_child(Node * test_node, Node * curr_node){
//   // cout << "In is_some_child" << endl << flush;
//   // cout << curr_node->name << endl;
//   std::vector<Node *> curr_children = curr_node->children;
//   // cout << "Got children" << endl << flush;
//   for(int i =0; i < curr_children.size(); i++){
//     if(strcmp(test_node->name, curr_children[i]->name) == 0){
//       return true;
//     }
//   }
//   for(int i = 0; i < curr_children.size(); i++){
//     if(!curr_children[i]->children.empty()){
//       return is_some_child(test_node, curr_children[i]);
//     }
//   }
//   return false;
// }

static void delete_tree(Node * curr_node, int level){
  cout << curr_node.name << " size: " << curr_node.size << endl;

  while (curr_node.children.size() >=0) {//while children exist
    for (size_t i = 0; i < curr_node.children.size(); i++) {
      delete_tree(curr_node.children[i]);
    }
  }
  delete_node(curr_node);
  return;
}

static void delete_node(Node* old_node){
  Node * parent = old_node->parent;
  old_node->decrease_all_parents_size(parent, old_node->size);
  // cout << "Parent Pointer: " << parent << endl << flush;
  // std::vector<Node*> parent->children = parent->children;
  // cout << "Children vect: " << parent->children << endl << flush;
  // cout << "Parent: " << parent->name << endl;
  // cout << "Size of children BEFORE: " << parent->children.size()<< endl;
  for(int i =0; i < parent->children.size(); i++){
    if(strcmp(parent->children[i]->name, old_node->name)==0){
      parent->children.erase(parent->children.begin()+i);
    }
  }
  // cout << "Size of children AFTER: "<< parent->children.size() << endl;
  if(old_node->type){
    delete old_node->f;
  }
  old_node->children.clear();
  vector<Node*>().swap(old_node->children);
  delete old_node->name;
  delete old_node;
}
