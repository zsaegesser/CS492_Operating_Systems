#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>

using namespace std;


class Node {
public:
  char * name;
  int size;
  time_t timestamp;
  Node * parent;
  std::vector<Node *> children;
  bool type; //0 for directory, 1 for file

  Node(bool node_type, char * node_name, int node_size, time_t node_timestamp){
    type = node_type;

    name = new char[strlen(node_name)];
    strcpy(name, node_name);

    size = node_size;
    timestamp = node_timestamp;
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

  //increase the size of all parents of the parent_node, CALLED ON ROOT
  //UNTESTED
  void increase_all_parents_size(Node * parent_node, int inc_size){
    if(parent_node != NULL){
      parent_node->increase_size(inc_size);
      increase_all_parents_size(parent_node->parent, inc_size);
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

static void delete_node(Node* old_node){
  Node * parent = old_node->parent;
  cout << "Parent Pointer: " << parent << endl << flush;
  std::vector<Node*> the_childs = parent->children;
  cout << "Children vect: " << the_childs << endl << flush;
  cout << "Parent: " << parent->name << endl;
  cout << "Size of children BEFORE: " << the_childs.size()<< endl;
  for(int i =0; i < the_childs.size(); i++){
    if(strcmp(the_childs[i]->name, old_node->name)==0){
      the_childs.erase(the_childs.begin()+i);
    }
  }
  cout << "Size of children AFTER: "<< the_childs.size() << endl;
  // old_node->children.clear();
  // vector<Node*>().swap(old_node->children);
  // delete old_node->name;
  delete old_node;
}
