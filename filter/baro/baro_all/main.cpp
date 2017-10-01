#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>

#define BARO_ALT_FILTER_TAU	0.0025f
#define BARO_ALT_FILTER_FC	1.0f

#define BARO_VEL_THRESHOLD_MAX 200.0f
#define BARO_VEL_THRESHOLD_MIN -200.0f

#define BARO_VEL_ABRUPT_CUT	 5.0f

#define BARO_VEL_FILTER_TAU	0.0025f
#define BARO_VEL_FILTER_FC	1.0f

#define BARO_ACC_THRESHOLD_MAX 100.0f
#define BARO_ACC_THRESHOLD_MIN -100.0f

#define BARO_ACC_ABRUPT_CUT	 1.0f

#define BARO_ACC_FILTER_TAU	0.0025f
#define BARO_ACC_FILTER_FC	12.0f

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
	std::ifstream ifs("out5mbaroraw.txt");
	if (!ifs) {
		std::cout << "input error" << std::endl;
		return 1;
	}
	std::ofstream ofs_RawFChanged("rawFchanged.txt");
	std::ofstream ofs_FALT("filtered_baro_alt.txt");
	std::ofstream ofs_VEL("baro_vel.txt");
	std::ofstream ofs_FVEL("filtered_baro_vel.txt");
	std::ofstream ofs_ACC("baro_acc.txt");
	std::ofstream ofs_FACC("filtered_baro_acc.txt");
	std::string str;

	bool	initial_flag = true;
	// time unit : micro sec
	float	newtime;
	float	oldtime;
	// alt unit : meter
	float	baro_alt;
	float	baro_alt_old;
	float	filtered_baro_alt;
	float	filtered_baro_alt_old;
	float	filtered_baro_alt_old2;
	// vel unit : meter per sec
	float	baro_vel;
	float	baro_vel_old;
	float	filtered_baro_vel;
	float	filtered_baro_vel_old;
	float	filtered_baro_vel_old2;
	// acc unit : meter per squeared sec 
	float	baro_acc;
	float	baro_acc_old;
	float	filtered_baro_acc;
	float	filtered_baro_acc_old;
	float	filtered_baro_acc_old2;

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
					filtered_baro_alt = baro_alt;
					filtered_baro_alt_old = baro_alt;
					filtered_baro_alt_old2 = baro_alt;
					baro_vel_old = 0.0f;
					filtered_baro_vel = 0.0f;
					filtered_baro_vel_old = 0.0f;
					filtered_baro_vel_old2 = 0.0f;
					baro_acc_old = 0.0f;
					filtered_baro_acc = 0.0f;
					filtered_baro_acc_old = 0.0f;
					filtered_baro_acc_old2 = 0.0f;
					initial_flag = false;
				} else {
					if (baro_alt != baro_alt_old) {
						// BW_filter
						filtered_baro_alt = BW_filter(filtered_baro_alt_old,filtered_baro_alt_old2,baro_alt,BARO_ALT_FILTER_FC,BARO_ALT_FILTER_TAU);
						// differential calculate
						baro_vel = (filtered_baro_alt - filtered_baro_alt_old)*1000000.0f/(newtime - oldtime);
						// threshold cut
						if (baro_vel < BARO_VEL_THRESHOLD_MIN) {
							baro_vel = BARO_VEL_THRESHOLD_MIN;
						} else if (baro_vel > BARO_VEL_THRESHOLD_MAX) {
							baro_vel = BARO_VEL_THRESHOLD_MAX;
						}
						// abrupt cut
						if ((baro_vel - baro_vel_old) < -BARO_VEL_ABRUPT_CUT) {
							baro_vel = baro_vel_old - BARO_VEL_ABRUPT_CUT;
						} else if ((baro_vel - baro_vel_old) > BARO_VEL_ABRUPT_CUT) {
							baro_vel = baro_vel_old + BARO_VEL_ABRUPT_CUT;
						} 
						// BW_filter
						filtered_baro_vel = BW_filter(filtered_baro_vel_old,filtered_baro_vel_old2,baro_vel,BARO_VEL_FILTER_FC,BARO_VEL_FILTER_TAU);
						// differential calculate
						baro_acc = (filtered_baro_vel - filtered_baro_vel_old)*1000000.0f/(newtime - oldtime);
						// threshold cut
						if (baro_acc < BARO_ACC_THRESHOLD_MIN) {
							baro_acc = BARO_ACC_THRESHOLD_MIN;
						} else if (baro_acc > BARO_ACC_THRESHOLD_MAX) {
							baro_acc = BARO_ACC_THRESHOLD_MAX;
						}
						// abrupt cut
						if ((baro_acc - baro_acc_old) < -BARO_ACC_ABRUPT_CUT) {
							baro_acc = baro_acc_old - BARO_ACC_ABRUPT_CUT;
						} else if ((baro_acc - baro_acc_old) > BARO_ACC_ABRUPT_CUT) {
							baro_acc = baro_acc_old + BARO_ACC_ABRUPT_CUT;
						} 
						// BW_filter
						filtered_baro_acc = BW_filter(filtered_baro_acc_old,filtered_baro_acc_old2,baro_acc,BARO_ACC_FILTER_FC,BARO_ACC_FILTER_TAU);
						// print
						ofs_RawFChanged << newtime << "\t" << baro_alt << std::endl;
						ofs_FALT << newtime << "\t" << filtered_baro_alt << std::endl;
						ofs_VEL << newtime << "\t" << baro_vel << std::endl;
						ofs_FVEL << newtime << "\t" << filtered_baro_vel << std::endl;
						ofs_ACC << newtime << "\t" << baro_acc << std::endl;
						ofs_FACC << newtime << "\t" << filtered_baro_acc << std::endl;
						// update old value
						oldtime = newtime;
						baro_alt_old = baro_alt;
						baro_vel_old = baro_vel;
						baro_acc_old = baro_acc;
						filtered_baro_alt_old2 = filtered_baro_alt_old;
						filtered_baro_alt_old = filtered_baro_alt;
						filtered_baro_vel_old2 = filtered_baro_vel_old;
						filtered_baro_vel_old = filtered_baro_vel;
						filtered_baro_acc_old2 = filtered_baro_acc_old;
						filtered_baro_acc_old = filtered_baro_acc;

					}
				}
			}
		}
	}
	return 0;
}

