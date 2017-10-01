#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

#define BARO_FILTER_TAU	0.0025f
#define BARO_FILTER_FC	1.0f

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
	std::ifstream ifs("out5mbaroraw_Fchanged.txt");
	if (!ifs) {
		std::cout << "input error" << std::endl;
		return 1;
	}
	std::ofstream ofs("baro_alt_BW_fc1.txt");
	std::string str;

	bool	initial_flag = true;
	// time unit : micro sec
	float	newtime;
	float	oldtime;
	// alt unit : meter
	float	baro_alt;
	float	filtered_baro_alt;
	float	filtered_baro_alt_old;
	float	filtered_baro_alt_old2;

	while (std::getline(ifs,str)) {
		std::string token;
		std::istringstream stream(str);
		for (int i = 0; i < 2; i++) {
			std::getline(stream,token,'\t');
			if (i == 0) {
				newtime = std::stof(token);
			} else {
				baro_alt = std::stof(token);
				if (initial_flag) {
					oldtime = newtime;
					filtered_baro_alt = baro_alt;
					filtered_baro_alt_old = baro_alt;
					filtered_baro_alt_old2 = baro_alt;
					initial_flag = false;
				} else {
					// BW_filter
					filtered_baro_alt = BW_filter(filtered_baro_alt_old,filtered_baro_alt_old2,baro_alt,BARO_FILTER_FC,BARO_FILTER_TAU);
				}
			}
		}
		// print
		std::cout <<  newtime << "\t" << filtered_baro_alt << std::endl;
		ofs << newtime << "\t" << filtered_baro_alt << std::endl;
		// update old value
		oldtime = newtime;
		filtered_baro_alt_old2 = filtered_baro_alt_old;
		filtered_baro_alt_old = filtered_baro_alt;
	}
	return 0;
}

