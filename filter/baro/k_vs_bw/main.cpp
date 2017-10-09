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
	SimpleKalman k_baro_alt;
	SimpleButterWorth bw_baro_alt;

	k_baro_alt.Q = 0.00001;
	k_baro_alt.R = 0.01;
	bw_baro_alt.fc = 2.5f;
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
					k_baro_alt.initialize(baro_alt);
					bw_baro_alt.initialize(baro_alt);
					initial_flag = false;
				} else {
					if (baro_alt != baro_alt_old) {
						ofs_all << newtime << "\t" << baro_alt
								<< "\t" << k_baro_alt.update(baro_alt)
								<< "\t" << bw_baro_alt.update(baro_alt)
								<< std::endl;
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

