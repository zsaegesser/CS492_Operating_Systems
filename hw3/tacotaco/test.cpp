#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include "../node.h"
using namespace std;

int main ( int argc, char *argv[] ){
  time_t timer;
  time(&timer);
  Node * this_node = Node(0,argv[1], 0, timer);

  std::cout << this_node.name << '\n';
}
