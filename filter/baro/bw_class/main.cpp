#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

class SimpleButterWorth
{
	constexpr static float tau = 0.0025f;
	constexpr static float fc = 1.0f;
	float old1,old2;
	float dst;
   public:
	SimpleButterWorth() {
	}
	float initialize(float first_value) {
		old1 = first_value;
		old2 = first_value;
		dst = first_value;
		return dst;
	}
	float update(float src) {
		float Omg, tOmg, ss, cs, a1, a2, b1;
		float theta = M_PI / 4.0;

		//update old value
		old2 = old1;
		old1 = dst;

		Omg = 2.0 * M_PI * fc;
		tOmg = Omg * tau;

		ss = tOmg * sin(theta);
		cs = tOmg * cos(theta);

		b1 = tOmg * exp(-ss)*sin(cs)/cos(theta);
		a1 = 2.0 * exp(-ss)*cos(cs);
		a2 = -exp(-2.0*ss);

		dst = b1*src + a1*old1 + a2*old2;
		return dst;
	}
};

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
	SimpleButterWorth bw;
	float	baro_alt;
	float	baro_alt_old;
	float	bw_filtered_baro_alt;

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
					bw.initialize(baro_alt);
					initial_flag = false;
				} else {
					if (baro_alt != baro_alt_old) {
						// BW_filter
						bw_filtered_baro_alt = bw.update(baro_alt);
						// print
						ofs_all << newtime << "\t" << baro_alt <<  "\t" << bw_filtered_baro_alt << std::endl;
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

