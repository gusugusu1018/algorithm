#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include "main.hpp"

int
main() {
	std::ifstream ifs("out5mbaroraw.txt");
	if (!ifs) {
		std::cout << "input error" << std::endl;
		return 1;
	}
	std::ofstream ofs_all("all.txt");
	std::string str;

	bool	initial_flag = true;
	// time unit : micro sec
	float	newtime;
	float	oldtime;
	// alt unit : meter
	SimpleButterWorth bw1;
	SimpleButterWorth bw2;
	SimpleButterWorth bw3;
	SimpleButterWorth bw4;
	SimpleButterWorth bw5;

	bw1.fc = 0.1f;
	bw2.fc = 0.5f;
	bw3.fc = 1.0f;
	bw4.fc = 5.0f;
	bw5.fc = 10.0;

	float	baro_alt;
	float	baro_alt_old;

	while (std::getline(ifs,str)) {
		std::string token;
		std::istringstream stream(str);
		for (int i = 0; i < 2; i++) {
			std::getline(stream,token,'\t');
			if (i == 0) {
				newtime = std::stof(token);
			} else {
				baro_alt = std::stof(token);
				// initialize value
				if (initial_flag) {
					oldtime = newtime; 
					baro_alt_old = baro_alt;
					bw1.initialize(baro_alt);
					bw2.initialize(baro_alt);
					bw3.initialize(baro_alt);
					bw4.initialize(baro_alt);
					bw5.initialize(baro_alt);
					initial_flag = false;
				} else {
					if (baro_alt != baro_alt_old) {
						ofs_all << newtime << "\t" << baro_alt
									<< "\t" << bw1.update(baro_alt)
									<< "\t" << bw2.update(baro_alt)
									<< "\t" << bw3.update(baro_alt)
									<< "\t" << bw4.update(baro_alt)
									<< "\t" << bw5.update(baro_alt) << std::endl;
						// update old value
						oldtime = newtime;
						baro_alt_old = baro_alt;
					}
				}
			}
		}
	}
	return 0;
}

