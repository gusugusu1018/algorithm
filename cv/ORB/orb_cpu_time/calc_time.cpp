#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, const char* argv[])
{
	cv::Mat img1 = cv::imread("../../Pictures/move1.jpg");
	cv::Mat img2 = cv::imread("../../Pictures/move2.jpg");
	// Value for keypoints
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;
	// Value for descriptor
	cv::Mat descriptor1;
	cv::Mat descriptor2;
	// Creat Object of FeatureDetector
	cv::Ptr<cv::FeatureDetector> detector = new cv::ORB(80, 1.25f, 4, 7, 0, 2, 0, 7);
	// Create Object of DescriptionExtractor
	cv::Ptr<cv::DescriptorExtractor> extractor = new cv::ORB(80, 1.25f, 4, 7, 0, 2, 0, 7);
	// Create Object of DescriptorMatcher
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
	// values of how maching
	std::vector<cv::DMatch> dmatch;
	std::vector<cv::DMatch> dmatchOK;

	// CPU
	cv::TickMeter cpumeter;
	cpumeter.start();

	// detect keypoints
	detector->detect(img1, keypoints1);
	detector->detect(img2, keypoints2);
	// compute descripter
	extractor->compute(img1, keypoints1, descriptor1);
	extractor->compute(img2, keypoints2, descriptor2);
	// match descripter
	matcher->match(descriptor1, descriptor2, dmatch);

	int count = dmatch.size();
	int matchCount = 0;

	// get minimal distance of match
	double min_dist = DBL_MAX;
	for (int i = 0; i < (int)dmatch.size(); i++){
		double dist = dmatch[i].distance;
		if (dist < min_dist){
			min_dist = dist;
		}
	}
	if (min_dist < 1.0)
	{
		min_dist = 1.0;
	}

	// set threshold
	const double threshold = 2.0 * min_dist;
	for (int i = 0; i < (int)dmatch.size(); i++){
		if (dmatch[i].distance < threshold){
			matchCount++;

			dmatchOK.push_back(dmatch[i]);
		}
	}
	cpumeter.stop();

	// show result
	std::cout << "ORB (CPU): " << cpumeter.getTimeMilli() << "ms" << std::endl;
	cv::Mat result;
	cv::drawMatches(img1, keypoints1, img2, keypoints2, dmatchOK, result);
	cv::imshow("matching", result);
	cv::waitKey(0);

	return 0;
}
