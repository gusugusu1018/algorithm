#include <iostream>
#include <fstream>

int
main () {
	std::ofstream ofs("txtout.csv");
	ofs<<"11,22,13,41"<<std::endl;
	ofs<<"11,22,13,41"<<std::endl;
	ofs<<"11,22,13,41"<<std::endl;
	return 0;
}
