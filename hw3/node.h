#include <vector>
#include <time>

using namespace std;


class Node {
public:
  char * name;
  int size;
  time_t timestamp;
  Node parent;
  std::vector<Node> children;
  bool type; //0 for directory, 1 for file

  Node(char * node_name, int node_size, time_t node_timestamp){
    this.name = node_name;
    this.size = node_size;
    this.timestamp = node_timestamp;
  }

  void set_parent(Node node_parent){
    this.parent = node_parent;
  }

  void add_child(Node new_node_child){
    this.children.push_back(new_node_child);
  }

  // void remove_child(char * node_name){
  //   //remove the child with name node_name
  // }
}
