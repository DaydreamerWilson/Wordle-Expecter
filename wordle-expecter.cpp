#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <cmath>
#include <chrono>

#define _CHAR 0
#define STATE 1

const int thread_num = 14;
const int list_size = 12972;
const double ln2 = log(2);

class match{
public:
  int scene[5][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
  // 0-25 for a-z
  // 0 for no match, 1 for match but misplaced, 2 for match and well-placed

  match(){};

  match(std::string content, std::string state){
    for(int x = 0; x < 5; x++){
      scene[x][_CHAR] = content[x]-'a';
      scene[x][STATE] = state[x]-'0';
    }
    /*
    for(int y = 0; y < 2; y++){
      for(int x = 0; x < 5; x++){
        std::cout << scene[x][y];
      }
      std::cout << " ";
    }
    */
    std::cout << std::endl;
  };

  bool all_true(){
    for(int x = 0; x < 5; x++){
      if(scene[x][STATE]!=2){
        return false;
      }
    }
    return true;
  };

  void print(){
    for(int x = 0; x < 5; x++){
      std::cout << scene[x][STATE];
    }
    std::cout << std::endl;
  };
};

class word{
public:
  std::string content;
  double entropy;

  word(std::string content, double entropy){
    this->content = content;
    this->entropy = entropy;
  }

  // assuming this->content is the correct answer
  match compare(std::string content){
    match temp;
    bool check[5] = {false, false, false, false, false};
    for(int x = 0; x < 5; x++){
      if(this->content[x]==content[x]){
        temp.scene[x][_CHAR]=this->content[x]-'a';
        temp.scene[x][STATE]=2;
        check[x] = true;
      }
    }
    for(int x = 0; x < 5; x++){
      if(temp.scene[x][STATE]!=2){
        for(int y = 0; y < 5; y++){
            if(this->content[y]==content[x] && x!=y && !check[y]){
              temp.scene[x][_CHAR]=this->content[y]-'a';
              temp.scene[x][STATE]=1;
              check[y] = true;
              break;
            }
            if(y == 4){
              temp.scene[x][STATE]=0;
            }
        }
      }
    }
    return temp;
  }

  // return true if the answer is valid under circumstance
  bool in_list(match temp){
    bool check[5] = {false, false, false, false, false};
    for(int x = 0; x < 5; x++){
      if(temp.scene[x][STATE]==2){
        if(temp.scene[x][_CHAR]!=this->content[x]-'a'){return false;}
        else{check[x]=true;}
      }
    }
    for(int x = 0; x < 5; x++){
      if(temp.scene[x][STATE]==0){
        for(int y = 0; y < 5; y++){
          if(temp.scene[x][_CHAR]==this->content[y]-'a' && !check[y]){return false;}
        }
      }
      if(temp.scene[x][STATE]==1){
        for(int y = 0; y < 5; y++){
          if(temp.scene[x][_CHAR]==this->content[y]-'a' && !check[y]){
              if(x==y){return false;}
              check[y]=true; break;
          }
          if(y == 4){return false;}
        }
      }
    }
    //std::cout << content << " valid ";
    return true;
  }
};

std::vector<word> words;
std::vector<word> full_words;
std::vector<word> backup_words;

// Assign entropy to vector words from start(inclusive) to ending(exclusive)
void assign_entropy(int start, int ending, int threads){
  match temp;
  for(int x = start; x < ending; x++){
    /*
    if(x%1 == 0){
      std::cout << threads << " : " << x << std::endl;
    }
    */
    double expected_entropy = 0;
    for(int y = 0; y < words.size(); y++){
      temp = words[y].compare(full_words[x].content);
      int count = 0;
      for(int z = 0; z < words.size(); z++){
        if(words[z].in_list(temp)){count++;}
      }
      if(count != 0){
        expected_entropy += -(log((double)count/words.size())/ln2);
      }
    }
    full_words[x].entropy = expected_entropy/words.size();
    //printf("%2d:  %5d  /  %lf  %5d > %5d > %5d  /  %f\n", threads, x, full_words[x].entropy, start, x, ending, ((float)x-start+1)/(ending-start));
    //std::cout << threads << " : " << start << " " << ending << " / " << words[x].entropy << std::endl;
  }
  //printf("%2d: finished processing data");
}

void eliminate_choices(match temp){
  std::vector<word> temp_list;
  for(int x = 0; x < words.size(); x++){
    if(words[x].in_list(temp)){
      temp_list.push_back(words[x]);
    }
  }
  words = temp_list;
  std::cout << words.size() << " words remaining." << std::endl;
}

void load(){
  std::ifstream file;
  file.open("all_combinations_initial_entropy.txt");
  std::string temp_content;
  double temp_entropy;
  while(!file.eof()){
    file >> temp_content;
    file >> temp_entropy;
    words.push_back(*(new word (temp_content, temp_entropy)));
  }

  full_words = words;
  backup_words = words;
  /*
  for(int x=0; x<words.size(); x++){
    std::cout << x << " : " << words[x].content << " " << words[x].entropy << std::endl;
  }
  */

  file.close();
  std::cout << "Loading completed" << std::endl;
}

void save(){
  std::cout << "Saving data of " << words.size() << " words" << std::endl;
  std::ofstream file;
  file.open("all_combinations_initial_entropy.txt");
  for(int x = 0; x < words.size(); x++){
    file << full_words[x].content << " " << full_words[x].entropy << std::endl;
  }
  file.close();
  std::cout << "Saving completed" << std::endl;
}

int print_possible(){
  word temp("", 0);
  for(int y = 0; y < 5; y++){
    for(int x = y+1; x < words.size(); x++){
      if(words[x].entropy > words[y].entropy){
        temp = words[y];
        words[y] = words[x];
        words[x] = temp;
      }
    }
  }

  int count = 5;
  if(words.size()<count){count = words.size();}
  for(int x = 0; x < count; x++){
    std::cout << words[x].content << std::endl;
  }
}

int print_max(){
  word temp("", 0);
  for(int y = 0; y < 5; y++){
    for(int x = y+1; x < full_words.size(); x++){
      if(full_words[x].entropy > full_words[y].entropy){
        temp = full_words[y];
        full_words[y] = full_words[x];
        full_words[x] = temp;
      }
    }
  }
  std::cout << std::endl;
  std::cout << "Highest entropies:" << std::endl;
  for(int x = 0; x < 5; x++){
    std::cout << full_words[x].content << " : " << full_words[x].entropy << std::endl;
  }
  std::cout << std::endl;
}

int main(){
  load();

  std::string input_c = "     ";
  std::string input_m = "     ";

  std::thread threads[thread_num];

  /*
  for(int x = 0; x < thread_num; x++){
    if(x!=thread_num-1){
      threads[x] = std::thread(assign_entropy, (list_size/thread_num)*x, (list_size/thread_num)*(x+1), x);
      std::cout << (list_size/thread_num)*x << " " << (list_size/thread_num)*(x+1) << std::endl;
    }
    else{
      threads[x] = std::thread(assign_entropy, (list_size/thread_num)*x, list_size, x);
      std::cout << (list_size/thread_num)*x << " " << list_size << std::endl;
    }
  }

  for(int x = 0; x < thread_num; x++){
    threads[x].join();
  }

  for(int x = 0; x < words.size(); x++){
    std::cout << full_words[x].content << " " << full_words[x].entropy << std::endl;
  }

  save();
  */

  print_possible();
  print_max();

  while(true){
    std::cin >> input_c;
    if(input_c != "-r"){
      std::cin >> input_m;
      match temp_match (input_c, input_m);
      eliminate_choices(temp_match);

      for(int x = 0; x < thread_num; x++){
        if(x!=thread_num-1){
          threads[x] = std::thread(assign_entropy, (full_words.size()/thread_num)*x, (full_words.size()/thread_num)*(x+1), x);
          //std::cout << (words.size()/thread_num)*x << " " << (words.size()/thread_num)*(x+1) << std::endl;
        }
        else{
          threads[x] = std::thread(assign_entropy, (full_words.size()/thread_num)*x, full_words.size(), x);
          //std::cout << (words.size()/thread_num)*x << " " << words.size() << std::endl;
        }
      }

      for(int x = 0; x < thread_num; x++){
        threads[x].join();
      }
    }
    else{
      full_words=backup_words;
      words=full_words;
    }

    print_possible();
    print_max();
  }

}
