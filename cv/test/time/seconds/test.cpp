#include <iostream>
#include <chrono>

int main(){
	std::chrono::system_clock::time_point  start, end;
	start = std::chrono::system_clock::now();
	// caluculate
		for (int i=0; i<10000000; i++) 
			std::cout << i << std::endl;
	end = std::chrono::system_clock::now(); 
	double elapsed = std::chrono::duration_cast<std::chrono::seconds>(end-start).count();
	 //convert time to msec
	std::cout << "time " << elapsed << " sec " << std::endl;
	return 0;
}
