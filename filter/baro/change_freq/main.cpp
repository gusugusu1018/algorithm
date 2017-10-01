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

	float	newtime;
	float	oldtime;
	float	baro_alt;
	float	baro_alt_old;
	bool	initial_flag = true;

	while (std::getline(ifs,str)) {
		std::string token;
		std::istringstream stream(str);
		for (int i = 0; i < 2; i++) {
			std::getline(stream,token,'\t');
			if (i == 0) {
				newtime = std::stof(token);
				if (initial_flag) oldtime = newtime;
			} else {
				baro_alt = std::stof(token);
				if (initial_flag) {
					baro_alt_old = baro_alt;
					initial_flag = false;
				}
				if (baro_alt != baro_alt_old) {
					std::cout << newtime << "\t" << baro_alt << std::endl;
					ofs << newtime << "\t" << baro_alt << std::endl;
					baro_alt_old = baro_alt;
					oldtime = newtime;
				}
			}
		}
	}
	return 0;
}

