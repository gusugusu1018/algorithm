#include <iostream>
#include <chrono>

int main(){
	std::chrono::system_clock::time_point  start, end;
	start = std::chrono::system_clock::now();
	// caluculate
		for (int i=0; i<100; i++) 
			std::cout << i << std::endl;
	end = std::chrono::system_clock::now(); 
	double elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
	 //convert time to msec
	std::cout << "time " << elapsed << " nanosec " << std::endl;
	return 0;
}
