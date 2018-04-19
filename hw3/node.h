#include <vector>

using namespace std;


class Node {
public:
  char * name;
  int size;
  int timestamp; //should probably be an actual timestamp (time_val)
  Node parent;
  Node[] children;
  bool type; //0 for directory, 1 for file
}
