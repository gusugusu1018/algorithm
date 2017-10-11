#include <iostream>
#include <sstream>
#include <fstream>
#include "../../libraries/Filter.h"

template <class T>
class Buffer {
public:
	T now,old;
	void update(){
		old = now;
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
	std::ofstream ofs_alt("alt.txt");
	std::ofstream ofs_vel("vel.txt");
	std::ofstream ofs_acc("acc.txt");
	std::string str;

	bool	initial_flag = true;
	// time unit : micro sec
	Buffer<float>	baro_time;
	// alt unit : meter
	Buffer<float>	baro_alt;	
	Buffer<float>	filtered_baro_alt;	
	ButterWorthFilter<float,3> bw_alt;
	// vel unit : meter per sec
	Buffer<float>	baro_vel;
	Buffer<float>	crt_filtered_baro_vel;
	Buffer<float>	bw_filtered_baro_vel;
	ChangeRateThresholdFilter<float,2> crt_vel;
	ButterWorthFilter<float,3> bw_vel;
	// acc unit : meter per sec^2
	Buffer<float>	baro_acc;
	Buffer<float>	crt_filtered_baro_acc;
	Buffer<float>	bw_filtered_baro_acc;
	ChangeRateThresholdFilter<float,2> crt_acc;
	ButterWorthFilter<float,3> bw_acc;

	// set param
	bw_alt.set_fc(2.8f);
	crt_vel.set_threshold(10.0f);
	bw_vel.set_fc(10.0f);
	crt_acc.set_threshold(100.0f);
	bw_acc.set_fc(10.0f);

	while (std::getline(ifs,str)) {
		std::string token;
		std::istringstream stream(str);
		for (int i = 0; i < 2; i++) {
			std::getline(stream,token,'\t');
			if (i == 0) {
				baro_time.now = std::stof(token);
			} else {
				baro_alt.now = std::stof(token);
				// initialize value
				if (initial_flag) {
					initial_flag = false;
					bw_alt.initialize(baro_alt.now);
					baro_time.update();
					baro_alt.update();
					filtered_baro_alt.update();
					baro_vel.now = 0.0f;
					crt_filtered_baro_vel.now = 0.0f;
					bw_filtered_baro_vel.now = 0.0f;
					crt_vel.initialize(0.0f);
					bw_vel.initialize(0.0f);
					baro_acc.now = 0.0f;
					crt_filtered_baro_acc.now = 0.0f;
					bw_filtered_baro_acc.now = 0.0f;
					crt_acc.initialize(0.0f);
					bw_acc.initialize(0.0f);
				} else {
					if (baro_alt.now != baro_alt.old) {
						// Butter Worth Filter
						filtered_baro_alt.now = bw_alt.update(baro_alt.now);
						if (baro_time.now != baro_time.old) {
							// differential calculate
							baro_vel.now = (filtered_baro_alt.now - filtered_baro_alt.old)*1000000.0f/(baro_time.now - baro_time.old);
							// Change Rate Threshold cut
							crt_filtered_baro_vel.now = crt_vel.update(baro_vel.now);
							// Butter Worth Filter
							bw_filtered_baro_vel.now = bw_vel.update(crt_filtered_baro_vel.now);
							// differential calculate
							baro_acc.now = (bw_filtered_baro_vel.now - bw_filtered_baro_vel.old)*1000000.0f/(baro_time.now - baro_time.old);
							// Change Rate Threshold cut
							crt_filtered_baro_acc.now = crt_acc.update(baro_acc.now);
							// Butter Worth Filter
							bw_filtered_baro_acc.now = bw_acc.update(crt_filtered_baro_acc.now);
						}
						ofs_all << baro_time.now
							<< "\t" << filtered_baro_alt.now
							<< "\t" << bw_filtered_baro_vel.now
							<< "\t" << bw_filtered_baro_acc.now
							<< std::endl;
						ofs_alt << baro_time.now
							<< "\t" << baro_alt.now
							<< "\t" << filtered_baro_alt.now
							<< std::endl;
						ofs_vel << baro_time.now 
							<< "\t" << baro_vel.now
							<< "\t" << crt_filtered_baro_vel.now
							<< "\t" << bw_filtered_baro_vel.now
							<< std::endl;
						ofs_acc << baro_time.now 
							<< "\t" << baro_acc.now
							<< "\t" << crt_filtered_baro_acc.now
							<< "\t" << bw_filtered_baro_acc.now
							<< std::endl;
						// update old value
						baro_time.update();
						baro_alt.update();
						filtered_baro_alt.update();
						baro_vel.update();
						crt_filtered_baro_vel.update();
						bw_filtered_baro_vel.update();
						baro_acc.update();
						crt_filtered_baro_acc.update();
						bw_filtered_baro_acc.update();
					}
				}
			}
		}
	}
	return 0;
}

