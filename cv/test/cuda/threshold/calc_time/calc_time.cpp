#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, const char* argv[])
{
	cv::Mat src = cv::imread("kinen.jpg", cv::IMREAD_GRAYSCALE);
	cv::Mat dst;
	cv::gpu::GpuMat gpusrc, gpudst;

	const int Count = 100;

	// CPU
	cv::TickMeter cpumeter,gpumeter;
	cpumeter.start();
	for (int i = 0; i < Count; i++) {
		cv::threshold(src, dst, 128, 255, cv::THRESH_BINARY);
	}
	cpumeter.stop();
	std::cout << "Threshold (CPU): " << cpumeter.getTimeMilli() << "ms" << std::endl;

	// GPU
	gpusrc.upload(src);
	gpumeter.start();
	for (int i = 0; i < Count; i++) {
		cv::gpu::threshold(gpusrc, gpudst, 128, 255, cv::THRESH_BINARY);
	}
	gpumeter.stop();
	gpudst.download(dst);
	std::cout << "Threshold (GPU): " << gpumeter.getTimeMilli() << "ms" << std::endl;
	std::cout << "GPU/CPU*100: " << cpumeter.getTimeMilli() / gpumeter.getTimeMilli() *100<< " %" << std::endl;
	return 0;
}
