#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
using namespace std;

int main ( int argc, char *argv[] ){


  vector<string> directories;
  string line ="";
  ifstream dir_list(argv[1]);
  if (dir_list.is_open())
  {
      while (dir_list>>line) //find size of current directory name
      {
        int i = line.length();
        while (line[i] != '/') {
          i--;
        }
        std::cout << "NEW NODE: \'" << line << "\' WITH PARENT: \'" << line.substr(0,i) << "\'" << flush << '\n';
      }
      dir_list.close();
  }


  ifstream file_list(argv[2]);
  string one,two,three,four,five,six,seven,eight,nine,ten,eleven;
  if (file_list.is_open())
  {
      while (file_list>>one>>two>>three>>four>>five>>six>>seven>>eight>>nine>>ten>>eleven)
      {
        //std::cout << eleven << '\n';
        int i = eleven.length();
        while (line[i] != '/') {
          i--;
        }
        std::cout << "NEW NODE: \'" << eleven << "\' WITH PARENT: \'" << eleven.substr(0,i) << "\'" << flush << '\n';
      }
      file_list.close();
  }
  return 0;
}
