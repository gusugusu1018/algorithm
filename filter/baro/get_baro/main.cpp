#include <iostream>
#include <sstream>
#include <fstream>

int
main()
{
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
		for (int i = 0; i<2;i++) {
			std::getline(stream,token,'\t');
			float time;
			float baro_alt;
			if (i == 0) {
				time = std::stof(token);
				std::cout <<  time << "\t";
				ofs << time << "\t";
			} else {
				baro_alt = std::stof(token);
				std::cout << baro_alt << std::endl;
				ofs << baro_alt << std::endl;
			}
		}
	}
	return 0;
}

