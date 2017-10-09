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
	SimpleKalman sk1;
	SimpleKalman sk2;
	SimpleKalman sk3;
	SimpleKalman sk4;
	SimpleKalman sk5;

	sk1.Q = 0.00001;
	sk2.Q = 0.00001;
	sk3.Q = 0.00001;
	sk4.Q = 0.00001;
	sk5.Q = 0.00001;
	sk1.R = 0.01;
	sk2.R = 0.01;
	sk3.R = 0.01;
	sk4.R = 0.01;
	sk5.R = 0.01;

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
					sk1.initialize(baro_alt);
					sk2.initialize(baro_alt);
					sk3.initialize(baro_alt);
					sk4.initialize(baro_alt);
					sk5.initialize(baro_alt);
					initial_flag = false;
				} else {
					if (baro_alt != baro_alt_old) {
						ofs_all << newtime << "\t" << baro_alt << "\t" 
									<< sk1.update(baro_alt)
									<< "\t" << sk2.update(baro_alt)
									<< "\t" << sk3.update(baro_alt)
									<< "\t" << sk4.update(baro_alt)
									<< "\t" << sk5.update(baro_alt) << std::endl;
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

