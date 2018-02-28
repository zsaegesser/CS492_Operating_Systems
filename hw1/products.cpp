#include <iostream>
#include <time.h>
#include <cstdlib>

using namespace std;


class Product {
    int id;
    time_t timestamp;
    int life;

  public:
    Product (int);
    Product (int, time_t, int);
    int get_id() {return id;}
    time_t get_time_stamp() {return timestamp;}
    int get_life() {return life;}
    bool check_sub(int);
    void sub_life(int);
};
/*Normal constructor, takes in an id and creates the other values*/
Product::Product (int newid){
  id = newid;
  timestamp = time(&timestamp);
  life = rand() % 1024;
}
/*testing constructor*/
Product::Product (int newid, time_t newtime, int newlife){
  id = newid;
  timestamp = newtime;
  life = newlife;
}
/*Return TRUE if calling taken would eliminate the life of the product, false otherwise*/
bool Product::check_sub(int taken){
  if(life-taken <= 0){
    return true;
  }
  return false;
}
/*take life from the product*/
void Product::sub_life(int taken){
  life = life-taken;
}

int main(){
  srand(1);
  Product test1 (1);
  Product test2 (2);
  cout << test1.get_id() << endl;
  cout << test1.get_time_stamp() << endl;
  cout << test1.get_life() << endl;
  cout << test2.get_life() << endl;
}
