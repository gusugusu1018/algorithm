#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, const char* argv[])
{
	cv::Mat img1_src = cv::imread("../../Pictures/move1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat img2_src = cv::imread("../../Pictures/move2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::gpu::GpuMat img1; 
	cv::gpu::GpuMat img2; 
	cv::Mat img1_dst;
	cv::Mat img2_dst;

	// Value for keypoints
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;
	cv::gpu::GpuMat keypoints1GPU; 
	cv::gpu::GpuMat keypoints2GPU; 
	// Value for descriptor
	cv::Mat descriptor1;
	cv::Mat descriptor2;
	cv::gpu::GpuMat descriptors1GPU; 
	cv::gpu::GpuMat descriptors2GPU; 
	// Creat Object of FeatureDetector
	cv::Ptr<cv::FeatureDetector> detector = new cv::ORB(200, 1.25f, 4, 7, 0, 2, 0, 7);
	// Create Object of DescriptionExtractor
	cv::Ptr<cv::DescriptorExtractor> extractor = new cv::ORB(200, 1.25f, 4, 7, 0, 2, 0, 7);
	// orb detector and extractor
	cv::gpu::ORB_GPU orb(200);
	// Create Object of DescriptorMatcher
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
	// Create Object of DescriptorMatcher
	cv::gpu::BruteForceMatcher_GPU< cv::Hamming > gpumatcher;
	// values of how maching
	std::vector<cv::DMatch> dmatch;
	std::vector<cv::DMatch> dmatchOK;
	std::vector< std::vector< cv::DMatch> > matches; 

	// CPU
	cv::TickMeter cpumeter;
	cpumeter.start();

	// detect keypoints
	detector->detect(img1_src, keypoints1);
	detector->detect(img2_src, keypoints2);
	// compute descripter
	extractor->compute(img1_src, keypoints1, descriptor1);
	extractor->compute(img2_src, keypoints2, descriptor2);
	// match descripter
	matcher->match(descriptor1, descriptor2, dmatch);

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
			dmatchOK.push_back(dmatch[i]);
		}
	}
	cpumeter.stop();

	// show result
	std::cout << "ORB (CPU): " << cpumeter.getTimeMilli() << "ms" << std::endl;
	std::cout << "keypoints1 : " << keypoints1.size() << "  keypoints2 :" << keypoints2.size() << std::endl;
	// upload src to gpu
	img1.upload(img1_src);
	img2.upload(img2_src);
	// clock
	cv::TickMeter gpumeter;
	gpumeter.start();
	// get feauters
	orb(img1, cv::gpu::GpuMat(), keypoints1GPU, descriptors1GPU);
	orb(img2, cv::gpu::GpuMat(), keypoints2GPU, descriptors2GPU);
	// matching
	gpumatcher.knnMatch(descriptors1GPU, descriptors2GPU, matches, 2);
	// threshold 
	std::vector< cv::DMatch > good_matches;
	for(int k = 0; k < std::min(descriptors1GPU.rows-1,(int) matches.size()); k++) 
	{
		if((matches[k][0].distance < 0.6*(matches[k][1].distance)) && ((int) matches[k].size()<=2 && (int) matches[k].size()>0))
		{
			good_matches.push_back(matches[k][0]);
		}
	}    

	gpumeter.stop();
	// download dst to host
	img1.download(img1_dst);
	img2.download(img2_dst);
	// show result
	std::cout << "ORB (GPU): " << gpumeter.getTimeMilli() << "ms" << std::endl;
	orb.downloadKeyPoints(keypoints1GPU, keypoints1);
	orb.downloadKeyPoints(keypoints2GPU, keypoints2);
	std::cout << "keypoint1 :" << keypoints1.size() << "  keypoint2 :" << keypoints2.size() << std::endl;
	std::cout << "GPU/CPU*100: " << cpumeter.getTimeMilli() / gpumeter.getTimeMilli() *100<< " %" << std::endl;

	return 0;
}
