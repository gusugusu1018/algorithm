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

	bw1.fc = 1.75f;
	bw2.fc = 2.0f;
	bw3.fc = 2.25f;
	bw4.fc = 2.3f;

	float	baro_alt;
	float	baro_alt_old;
	float baro_vel1;
	float baro_vel2;
	float baro_vel3;
	float baro_vel4;

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
					initial_flag = false;
				} else {
					if (baro_alt != baro_alt_old) {
						bw1.update(baro_alt);
						bw2.update(baro_alt);
						bw3.update(baro_alt);
						bw4.update(baro_alt);
						baro_vel1 = (bw1.now - bw1.old)*1000000.0f/(newtime - oldtime);
						baro_vel2 = (bw2.now - bw2.old)*1000000.0f/(newtime - oldtime);
						baro_vel3 = (bw3.now - bw3.old)*1000000.0f/(newtime - oldtime);
						baro_vel4 = (bw4.now - bw4.old)*1000000.0f/(newtime - oldtime);
						ofs_all << newtime << "\t" << baro_alt
									<< "\t" << bw1.now
									<< "\t" << bw2.now
									<< "\t" << bw3.now
									<< "\t" << bw4.now
									<< "\t" << baro_vel1
									<< "\t" << baro_vel2
									<< "\t" << baro_vel3
									<< "\t" << baro_vel4
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

