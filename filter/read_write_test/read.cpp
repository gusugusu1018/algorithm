#include <iostream>
#include <sstream>
#include <fstream>

int
main() {
	std::ifstream ifs("test.csv");
	if (!ifs) {
		std::cout << "input error" << std::endl;
		return 1;
	}
	std::string str;
	while (std::getline(ifs,str)) {
		std::string token;
		std::istringstream stream(str);
		while (std::getline(stream,token,',')) {
			float temp=std::stof(token);
			std::cout << temp << ",";
		}
		std::cout << std::endl;
	}
	return 0;
}

