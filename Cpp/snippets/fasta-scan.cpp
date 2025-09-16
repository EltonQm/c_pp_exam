// compile:  g++ -std=c++17 ../fasta-scan.cpp -o fasta-scan
#include <iostream>
#include <map>
#include <filesystem>
#include <regex>
#include <fstream>

// void  then use std::cout
std::vector< std::pair<std::string,int>> scan (std::string filename) {
    std::cout << "Scanning ... " << filename << "!\n";
    std::ifstream infile; 
    std::string line ;
    std::regex rxId("^>");
    std::string id;
    int nchar = 0;

    std::vector< std::pair<std::string,int> > result;
    infile.open(filename); 
    while (std::getline(infile, line)) {
      if (std::regex_search(line,rxId)) {
         if (nchar>0) {
             //std::cout << id << "\t" << nchar << std::endl;
             result.push_back({id,nchar});
         }
         id=std::regex_replace(line,std::regex(">([^ ]+) *.*"),"$1");
         nchar=0;
      } else {
         nchar += line.length();
      }
    }

    //std::cout << id << "\t" << nchar << std::endl;
    result.push_back({id,nchar});
    infile.close();
    return(result);
}

void usage () {
    std::cout << "Usage: fasta-scan fastafile\n";
}

int main (int argc, char ** argv) {
  namespace fs = std::filesystem;
  if (argc == 2) {
      if (!fs::exists(argv[1])) {
          std::cout << "Given file does not exists!\n";
      } else {
          std::cout << "Given file" << argv[1] << " does  exists!\n";
          auto res = scan(argv[1]);
          for (auto r : res) {
              std::cout << r.first <<"\t" << r.second << std::endl;
          }
      }
  } else {
      usage()  ;
  }
}
