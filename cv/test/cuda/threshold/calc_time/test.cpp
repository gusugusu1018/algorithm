#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/gpu/gpu.hpp"

using namespace std;
using namespace cv;
using namespace cv::gpu;

Mat src_host = imread("C:\\Lenna.png", cv::IMREAD_GRAYSCALE);
GpuMat srcg(src);

const int Count = 5000;

// CPU
TickMeter meter;
meter.start();
for (int i = 0; i < Count; i++) {
    Mat binary;
    threshold(src, binary, 128, 255, cv::THRESH_BINARY);
}
meter.stop();
cout << "Threshold (CPU): " << meter.getTimeMilli() << "ms" << endl;

// GPU
meter.reset();
meter.start();
for (int i = 0; i < Count; i++) {
    GpuMat binaryg;
    threshold(srcg, binaryg, 128, 255, cv::THRESH_BINARY);
}
meter.stop();
cout << "Threshold (GPU): " << meter.getTimeMilli() << "ms" << endl;

