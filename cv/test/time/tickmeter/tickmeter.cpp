#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, const char* argv[])
{
  cv::Mat src = cv::imread("../../../../Pictures/move1.jpg", cv::IMREAD_COLOR);

  if (src.empty())
  {
    std::cerr << "Failed to open image file." << std::endl;
    return -1;
  }

  cv::Mat dst;
  cv::TickMeter meter;

  meter.start();

  cv::resize(src, dst, cv::Size(), 5.0, 5.0);

  meter.stop();

  std::cout << meter.getTimeMilli() << "[ms]" << std::endl;

  return 0;
}

