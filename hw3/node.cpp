#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>

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
    name = node_name;
    size = node_size;
    timestamp = node_timestamp;
  }

  void set_parent(Node * node_parent){
    parent = node_parent;
  }

  void add_child(Node * new_node_child){
    // cout << "Added " << new_node_child.name << " to " << name << "Children Size: " << children.size() <<endl;
    children.push_back(new_node_child);
  }

  // called on root, given name of node you want
  Node find_node_by_name(char * search_name){
    if(strcmp(name, search_name) == 0){
      return *this;
    }
    else if(!children.empty()){
      for(int i = 0; i < children.size(); i++){
        children[i]->find_node_by_name(search_name);
      }
    }
    else{
      cout << "DIDNT FIND NODE" << endl;
      return *this;
    }
  }



  // void remove_child(char * node_name){
  //   //remove the child with name node_name
  // }
};
static void print_tree(Node &curr_node, int level){
  for(int i = 0; i < level; i++){
    cout << "   ";
  }
  cout << curr_node.name << endl;
  // cout << curr_node->children.size() << endl;
  for(int i = 0; i < curr_node.children.size(); i++){
    print_tree(*curr_node.children[i], level+1);
  }
}

int main(int argc, char * const argv[]){
  time_t timer;
  time(&timer);
  Node one = Node(0, "name/of", 12, timer);
  Node two = Node(0, "name/of/this", 18, timer);
  Node three = Node(0,"name/of/that", 18, timer);
  Node four = Node(0, "name/it", 18, timer);
  Node five = Node(0, "name/it/cat", 18, timer);
  Node root = Node(0, "name", 12, timer);
  root.add_child(&one);
  root.add_child(&four);
  //
  one.set_parent(&root);
  four.set_parent(&root);
  //
  one.add_child(&two);
  one.add_child(&three);
  // // std::cout << "Size of one Children: "<< one.children.size() << endl;
  four.add_child(&five);
  two.set_parent(&one);
  three.set_parent(&one);
  five.set_parent(&four);
  //
  print_tree(root, 0);

  return 0;

}
