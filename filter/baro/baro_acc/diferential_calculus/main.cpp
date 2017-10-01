#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

#define THRESHOLD_MAX 1
#define THRESHOLD_MIN -1

#define BARO_FILTER_TAU	0.0025f
#define BARO_FILTER_FC	12.0f

/**
 * Butter Worth filter for barometer 
 * ARISUMI SENSEI @AIST
 * y1 = filtered_old1, y2 = filtered_old2, x1 = raw_now 
 */
float BW_filter(float y1, float y2, float x1, float fc, float tau)
{
        float Omg, tOmg, ss, cs, a1, a2, b1;
        float theta = M_PI / 4.0;

        Omg = 2.0 * M_PI * fc;
        tOmg = Omg * tau;

        ss = tOmg * sin(theta);
        cs = tOmg * cos(theta);

        b1 = tOmg * exp(-ss)*sin(cs)/cos(theta);
        a1 = 2.0 * exp(-ss)*cos(cs);
        a2 = -exp(-2.0*ss);

        //y[ik] = b1*x[ik - 1] + a1*y[ik - 1] + a2*y[ik - 2];
        return b1*x1 + a1*y1 + a2*y2;
}


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
	float	filtered_baro_vel;
	float	filtered_baro_vel_old;
	float	filtered_baro_vel_old2;
	// acc 
	float	baro_acc;

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
					filtered_baro_vel = 0.0f;
					filtered_baro_vel_old = 0.0f;
					filtered_baro_vel_old2 = 0.0f;
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
					// BW_filter
					filtered_baro_vel = BW_filter(filtered_baro_vel_old,filtered_baro_vel_old2,baro_vel,BARO_FILTER_FC,BARO_FILTER_TAU);

					// differential calculate
					baro_acc = (filtered_baro_vel - filtered_baro_vel_old)*1000000.0f/(newtime - oldtime);

					std::cout << newtime << "\t" << baro_acc << std::endl;
					ofs << newtime << "\t" << baro_acc << std::endl;
				}
			}
		}
		oldtime = newtime;
		baro_alt_old = baro_alt;
		filtered_baro_vel_old2 = filtered_baro_vel_old;
		filtered_baro_vel_old = filtered_baro_vel;
	}
	return 0;
}

