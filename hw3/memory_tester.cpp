#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include "ldisk.h"

using namespace std;

int main(int argc, char * const argv[]){
  Ldisk * t = new Ldisk(100,10);

  // block_group * new_3 = new block_group(0,pair<int,int> (8,9),NULL);
  // block_group * new_2 = new block_group(1,pair<int,int> (4,7),new_3);
  // block_group * new_1 = new block_group(0,pair<int,int> (0,3),new_2);
  // t->head = new_1;
  //t->print();
  t->remove(70);
  t->print();
}
