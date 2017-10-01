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

	// time unit : micro sec
	float newtime;
	float oldtime;
	// alt unit : meter
	float baro_alt;
	float baro_alt_old;
	float baro_vel;

	while (std::getline(ifs,str)) {
		std::string token;
		std::istringstream stream(str);
		for (int i = 0; i < 2; i++) {
			std::getline(stream,token,'\t');
			if (i == 0) {
				newtime = std::stof(token);
				std::cout <<  newtime << "\t";
				ofs << newtime << "\t";
			} else {
				baro_alt = std::stof(token);

				baro_vel = (baro_alt - baro_alt_old)*1000000.0f/(newtime - oldtime);
				std::cout << baro_vel << std::endl;
				ofs << baro_vel << std::endl;
			}
		}
		oldtime = newtime;
		baro_alt_old = baro_alt;
	}
	return 0;
}

