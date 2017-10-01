#include <iostream>
#include <sstream>
#include <fstream>

int
main() {
	std::ifstream ifs("filtered_baro.txt");
	if (!ifs) {
		std::cout << "input error" << std::endl;
		return 1;
	}
	std::ofstream ofs("1.txt");
	std::string str;
	while (std::getline(ifs,str)) {
		std::string token;
		std::istringstream stream(str);
		while (std::getline(stream,token,'\t')) {
			float temp=std::stof(token);
			std::cout << temp << "\t";
			ofs << temp << "\t";
		}
		std::cout << std::endl;
		ofs << std::endl;
	}
	return 0;
}

