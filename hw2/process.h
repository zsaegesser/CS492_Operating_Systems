#include <vector>

struct page {
  std::pair<int,int> page_id;
  bool valid_bit;
  long timestamp;
};

class Process{
public:
  int pid;
  std::vector<page> page_table;

  //provides a printout of the Process and Corresponding Page Table
  void printout(){
    std::cout <<'\n'<< pid << "---------------------------------------" << '\n';
    int i = 0;
    for (size_t i = 0; i < page_table.size(); i++) {
      int second = page_table[i].page_id.second;
      int t = page_table[i].timestamp;
      if (second <10) {
        std::cout << second << ":" << t << "  " << "|";
      }else if (second < 100) {
        std::cout << second << ":" << t << " " <<  "|";
      }else{
        std::cout << second << ":" << t << "|";
      }
      if (!(i%10)) {
        std::cout << '\n';
       }
    }
  }

  //Method for creating page table
  void createTable(int pID, int totalMemory, int page_size){
    //calculate page table size based of page size
    int table_size = totalMemory/page_size;
    std::vector<page> result;

    //crate pages with unique id
    for (size_t i = 0; i < table_size; i++) {
      page t;
      t.page_id.first = pID;
      t.page_id.second = i; // pairs a good naming scheme?
      t.valid_bit = 0; //not sure yet
      t.timestamp = 0;
      result.push_back(t);
    }
    page_table = result;
  }
};
