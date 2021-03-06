#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include "node.h"

using namespace std;

// //Node * g;
//
// class Node {
// public:
//   char * name;
//   int size;
//   time_t timestamp;
//   Node * parent;
//   std::vector<Node *> children;
//   bool type; //0 for directory, 1 for file
//
//   Node(){
//     type = 0;
//   }
//   Node(bool node_type, char * node_name, int node_size, time_t node_timestamp){
//     type = node_type;
//     name = node_name;
//     size = node_size;
//     timestamp = node_timestamp;
//   }
//
//   void set_parent(Node * node_parent){
//     parent = node_parent;
//   }
//
//   void add_child(Node * new_node_child){
//     // cout << "Added " << new_node_child.name << " to " << name << "Children Size: " << children.size() <<endl;
//     children.push_back(new_node_child);
//   }
//
//   // called on root, given name of node you want
//   Node find_node_by_name(char * search_name){
//     // cout << name << " " << search_name <<  endl;
//     if((strcmp(name, search_name) == 0)){
//       // cout << "got here" << name << endl;
//       return *this;
//     }
//     else if(!children.empty()){
//       for(int i = 0; i < children.size(); i++){
//         return children[i]->find_node_by_name(search_name);
//         // if(strcmp(temp.name, search_name) == 0){
//         //   return temp;
//         // }
//       }
//     }
//     else{
//       time_t timer;
//       time(&timer);
//       cout << "DIDNT FIND NODE" << endl;
//       return Node(0,"", 0, timer);
//     }
//   }
//
//
//
//   // void remove_child(char * node_name){
//   //   //remove the child with name node_name
//   // }
// };
// static void print_tree(Node &curr_node, int level){
//   for(int i = 0; i < level; i++){
//     cout << "   ";
//   }
//   cout << curr_node.name << endl;
//   // cout << curr_node->children.size() << endl;
//   for(int i = 0; i < curr_node.children.size(); i++){
//     print_tree(*curr_node.children[i], level+1);
//   }
// }
//
//
// // is_some_child(a, b) => returns TRUE if a is a child of b, false otherwise
// bool is_some_child(Node test_node, Node curr_node){
//   std::vector<Node *> curr_children = curr_node.children;
//   for(int i =0; i < curr_children.size(); i++){
//     if(strcmp(test_node.name, curr_children[i]->name) == 0){
//       // cout << test_node.name << curr_children[i]->name << endl;
//       // cout << "Got here" << endl;
//       return true;
//     }
//   }
//   for(int i = 0; i < curr_children.size(); i++){
//     if(!curr_children[i]->children.empty()){
//       return is_some_child(test_node, *curr_children[i]);
//     }
//   }
//   return false;
// }



int main(int argc, char * const argv[]){
  Ldisk * ldisk;
  time_t timer;
  time(&timer);
  //Node * one = new Node(0, "2", 12, timer, ldisk);
  // Node * two = new Node(0, "4", 18, timer, ldisk);
  // Node * three = new Node(0,"5", 18, timer);
  // Node * four = new Node(0, "3", 18, timer);
  // Node * five = new Node(0, "6", 18, timer);
  Node * root = new Node(0, "1", 12, timer, new Lfile(0,NULL, 10));
  // root->add_child(one);
  // root->add_child(four);
  // //
  // one->set_parent(root);
  // four->set_parent(root);
  // //
  // one->add_child(two);
  // one->add_child(three);
  // // // std::cout << "Size of one Children: "<< one.children.size() << endl;
  // four->add_child(five);
  // two->set_parent(one);
  // three->set_parent(one);
  // five->set_parent(four);
  //
  //print_tree(&root, 0);
  std::cout << "PRE" << '\n';
  print_treeB(root);
  std::cout << "POST" << '\n';
  delete root;
  print_treeB(root);
  // print(is_some_child(root, one))
  //cout << is_some_child(five, four) << endl;
  // Node temp = root.find_node_by_name("name");
  // cout << temp.name;
  return 0;

}
