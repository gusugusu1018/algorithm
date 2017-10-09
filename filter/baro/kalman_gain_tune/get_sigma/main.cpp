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
	float	baro_alt;
	float	baro_alt_old;

	int counter = 1;
	float sum= 0.0f;
	float myu;
	float sigma;
	float sum_sigma=0.0f;
	float sigma_av;

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
					initial_flag = false;
				} else {
					if (baro_alt != baro_alt_old) {
						sum = sum + baro_alt;
						myu = sum / (float)counter;
						sigma = pow((baro_alt - myu),2);
						sum_sigma = sum_sigma + sigma;
						sigma_av = sum_sigma / (float)counter;
						ofs_all << newtime << "\t" << baro_alt<< "\t" << myu << "\t" << sigma_av << std::endl;
						// update old value
						oldtime = newtime;
						baro_alt_old = baro_alt;
						counter++;
					}
				}
			}
		}
	}
	return 0;
}

