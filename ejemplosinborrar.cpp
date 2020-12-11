#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <map>
using namespace std;

string  get_var_name(string line);

int get_var_value(string line);

bool skip_line(string line);

int main(int argc, char** argv){
  fstream newfile;
  string filename;
  if (argc < 2 ){
    filename="muestras-mysqladmin";
  }
  else {
    filename=argv[1];
  }
  cout << "File:"  << filename << std::endl;
  cout << "executable:"  << argv[0] << std::endl;
  newfile.open(filename,ios::in); 
  if (newfile.is_open()){   
    string line;
    string vname;
    int vvalue;
    int col=0;
    std::map <string, std::map <int,int> > result;
    std::map <string, int > previous;
    while(getline(newfile, line)){ 
      if(skip_line(line)){
        continue;
      }
      vname = get_var_name(line);
      vname = vname.substr(vname.find_first_not_of(" \t")).erase(vname.find_last_not_of(" \n\r\t")); 
      if (vname.compare("Variable_name") == 0) {
      
        col++;
        continue;
      }
      vvalue = get_var_value(line);
      result[vname][col]=vvalue-previous[vname];
      previous[vname]=vvalue;
    }
    newfile.close();
    map<string,map<int,int>>::iterator nextLine = result.begin();
    while (nextLine != result.end()) {
      printf("\n%s ", nextLine->first.c_str());
      map<int,int>::iterator nextColumn = nextLine->second.begin();
      while (nextColumn != nextLine->second.end()) {
        printf("%d\t", nextColumn->second);
        nextColumn++;
      }
      nextLine++;
    }
  }
}

string  get_var_name(string line){
    //line.find("|", 1);
    return line.substr(1,line.find("|", 1)-1);
}

int get_var_value(string line){
    int cut_a = line.find("|", 1) + 1; 
    int cut_b = line.find("|", cut_a) - 1 - cut_a;
    char *ptr;
    return strtol(line.substr(cut_a, cut_b).c_str(), &ptr,10);
}

bool skip_line (string line){
    return !(line.substr(0,1).compare("|") == 0);
}
