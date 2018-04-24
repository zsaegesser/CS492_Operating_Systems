#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include "lfile.h"

using namespace std;


int main(int argc, char * const argv[]){
  Lfile * lfile = new Lfile(0, NULL, 8);
  // lfile->add_disk_block(0);
  // lfile->add_disk_block(8);
  // lfile->add_disk_block(16);
  // lfile->remove_last_disk_block();
  // lfile->remove_last_disk_block();
  long temp = 20;
  lfile->add_bytes(temp);
  // lfile->add_bytes(temp);
  lfile->print_lfile();
}
