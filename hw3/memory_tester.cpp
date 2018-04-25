#include <iostream>
#include <vector>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include "ldisk.h"
#include "lfile.h"

using namespace std;

int main(int argc, char * const argv[]){
  Ldisk * d = new Ldisk(100,10); // (disk_size, block_size)
  Lfile * f = new Lfile(0, NULL, 10);
  // cout << "initialize" <<endl << flush;

  f->add_bytes(5, d);
  d->print();
  f->print_lfile();
  f->add_bytes(6,d);
  d->print();
  f->print_lfile();
  f->add_bytes(46,d);
  d->print();
  f->print_lfile();
  cout << " FRAG: " <<f->frag_count() << endl << flush;
  f->remove_bytes(3,d);
  d->print();
  f->print_lfile();
  f->remove_bytes(5,d);
  d->print();
  f->print_lfile();
  cout << " FRAG: " <<f->frag_count() << endl << flush;
  // cout << "Added" <<  endl << flush;
  // d->print();
  // f->print_lfile();

  // f->remove_bytes(5, d);
  // f->remove_bytes(90, d);

  // f->add_bytes(100, d);
  // f->add_bytes(5,d);
  // f->add_bytes(6,d);
  // cout << " Removed" << endl << flush;
  // d->print();
  // f->print_lfile();





  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // cout << "Mem Address Fetch: " << t->fetch() << endl << flush;
  // block_group * new_3 = new block_group(0,pair<int,int> (8,9),NULL);
  // block_group * new_2 = new block_group(1,pair<int,int> (4,7),new_3);
  // block_group * new_1 = new block_group(0,pair<int,int> (0,3),new_2);
  // t->head = new_1;
  //t->print();
  // t->remove(70);
  // t->print();
}
