#include <iostream>
#include <sstream>
#include <fstream>

#define THRESHOLD_MAX 1
#define THRESHOLD_MIN -1
#define ABRUPT_CUT_VEL 0.1

int
main() {
	std::ifstream ifs("filtered_baro.txt");
	if (!ifs) {
		std::cout << "input error" << std::endl;
		return 1;
	}
	std::ofstream ofs("1.txt");
	std::string str;

	bool	initial_flag = true;
	// time unit : micro sec
	float	newtime;
	float	oldtime;
	// alt unit : meter
	float	baro_alt;
	float	baro_alt_old;
	// vel unit : meter per sec
	float	baro_vel;
	float	baro_vel_old;
	float	baro_vel_old2;
	float	baro_vel_old3;


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
					baro_vel_old = baro_vel;
					baro_vel_old2 = baro_vel;
					baro_vel_old3 = baro_vel;
					initial_flag = false;
				} else {
					// differential calculate
					baro_vel = (baro_alt - baro_alt_old)*1000000.0f/(newtime - oldtime);

					// threshold cut
					if (baro_vel < THRESHOLD_MIN) {
						baro_vel = THRESHOLD_MIN;
					} else if (baro_vel > THRESHOLD_MAX) {
						baro_vel = THRESHOLD_MAX;
					}
					if ((baro_vel - baro_vel_old) < -ABRUPT_CUT_VEL) {
						baro_vel = baro_vel_old - ABRUPT_CUT_VEL;
					} else if ((baro_vel - baro_vel_old) > ABRUPT_CUT_VEL) {
						baro_vel = baro_vel_old + ABRUPT_CUT_VEL;
					} 
					baro_vel = (baro_vel + baro_vel_old + baro_vel_old2 + baro_vel_old3)/4.0f;
					std::cout << newtime << "\t" << baro_vel << std::endl;
					ofs << newtime << "\t" << baro_vel << std::endl;
				}
			}
		}
		oldtime = newtime;
		baro_alt_old = baro_alt;
		baro_vel_old3 = baro_vel_old2;
		baro_vel_old2 = baro_vel_old;
		baro_vel_old = baro_vel;
	}
	return 0;
}

