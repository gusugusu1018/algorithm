#include <iostream>
#include <sstream>
#include <fstream>

int
main() {
   std::ifstream ifs("../timestamps.txt");
   if (!ifs) {
      std::cout << "input error" << std::endl;
      return 1;
   }
   std::ofstream ofs("plot.txt");
   std::string str;
   int counter = 1;
   int index=1;

   while (std::getline(ifs,str)) {
      std::string token;
      std::istringstream stream(str);
      std::getline(stream,token,'\t');
      if (counter%2 == 1) {
         double time = std::stof(token);
         ofs << index << "\t" << time<< std::endl;
         index++;
      } else {
         int time = std::stof(token);
      }
      counter++;
   }
   return 0;
}

