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
	std::vector< cv::KeyPoint> keypoints1, keypoints2;
	cv::gpu::GpuMat keypoints1GPU, keypoints2GPU; 
	// Value for descriptor
	std::vector< float> descriptors1, descriptors2;
	cv::gpu::GpuMat descriptors1GPU, descriptors2GPU; 
	// orb detector and extractor
	cv::gpu::ORB_GPU orb(200);
	// Create Object of DescriptorMatcher
	cv::gpu::BruteForceMatcher_GPU< cv::Hamming > matcher;
	// values of how maching
	std::vector< std::vector< cv::DMatch> > matches; 
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
	matcher.knnMatch(descriptors1GPU, descriptors2GPU, matches, 2);
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
	std::cout << "FOUND " << keypoints1GPU.cols << " keypoints on first image" << std::endl; 
	std::cout << "FOUND " << keypoints2GPU.cols << " keypoints on second image" << std::endl; 
	orb.downloadKeyPoints(keypoints1GPU, keypoints1);
	orb.downloadKeyPoints(keypoints2GPU, keypoints2);
	std::cout << "keypoint1 :" << keypoints1.size() << "  keypoint2 :" << keypoints2.size() << std::endl;

	cv::Mat img_matches; 
	cv::drawMatches(img1_dst, keypoints1, img2_dst, keypoints2, good_matches, img_matches);
	cv::namedWindow("matches", 0);
	cv::imshow("matches", img_matches);
	cv::waitKey(0);

	return 0;
}
