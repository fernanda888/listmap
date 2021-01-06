// $Id: main.cpp,v 1.12 2020-10-22 16:50:08-07 - - $

#include <cstdlib>
#include <exception>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <regex>

using namespace std;

#include <libgen.h>
#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;
const string cin_name = "-";

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << static_cast<char> (option)
                       << ": invalid option" << endl;
            break;
      }
   }
}

void map_commands(istream& infile, const string& filename) {
   regex comment_regex {R"(^\s*(#.*)?$)"};
    regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
    regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
    int line_num=0;
    str_str_map map;
    for(;;) {
      string line;
      getline (infile, line);
      if (infile.eof()) break;
      ++line_num;
      cout <<filename<< ": "<<line_num<< ": "<< line << endl;
      smatch result;
      if (regex_search (line, result, comment_regex)) {
         continue;
      }
      if (regex_search (line, result, key_value_regex)) {
         if(result[1]!="" && result[2]!=""){//insert
           str_str_pair pair(result[1],result[2]);
           cout<<*(map.insert(pair))<<endl;
         }
         else if(result[1]=="" && result[2]==""){//print all keys
           for (str_str_map::iterator itor = map.begin();
            itor != map.end(); ++itor) {
             cout << *itor << endl;
           }
         }
         else if(result[1]!="" && result[2]==""){//erase
           for (str_str_map::iterator itor = map.begin();
            itor != map.end(); ++itor) {
             if(itor==map.find(result[1])){
               map.erase(itor);
               break;
             }
           }

         }
         else if(result[1]=="" && result[2]!=""){
           map.find_val(result[2]);
         }
      }else if (regex_search (line, result, trimmed_regex)) {
         bool exists=false;
           for (str_str_map::iterator itor = map.begin();
            itor != map.end(); ++itor) {
             if(itor==map.find(result[1])){
               cout << *itor << endl;
               exists=true;
             }
           }
           if(exists==false) cout<<result[1]<<": key not found"<<endl;
      }else {
         assert (false and "This can not happen.");
      }
    }
}
int main (int argc, char** argv) {
  sys_info::execname (argv[0]);
  scan_options (argc, argv);
  string progname (basename (argv[0]));
  int status = 0;
  if(argc>1){
  for(int file_num=1; file_num!=argc;file_num++){
  string filename (argv[file_num]);
  ifstream infile (filename);
  if (infile.fail()) {
    status = 1;
    cerr << progname << ": " << filename << ": "
    << strerror (errno) << endl;
  }
  else {
    map_commands(infile,filename);
    infile.close();
  }
  }
  }
  else{
    map_commands(cin,cin_name);
  }
  return status;
}
