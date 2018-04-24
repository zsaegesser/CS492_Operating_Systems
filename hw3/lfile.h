#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>

using namespace std;

struct block{
  long block_address;
  block *next;
}

class Lfile{
private:
  block *head, *tail;

public:
  int size;
  //write constructor that sets up empty lfile


}
