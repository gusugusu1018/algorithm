#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/gpu/gpu.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

/*
#include < opencv2/core/core.hpp>
#include < opencv2/highgui/highgui.hpp>
//#include < opencv2/video/background_segm.hpp>
#include < opencv2\stitching\detail\matchers.hpp >  
//#include < opencv2\nonfree\features2d.hpp >    
*/

int main(int argc, const char* argv[])
{
	cv::gpu::GpuMat img1(cv::imread("../../Pictures/move1.jpg", CV_LOAD_IMAGE_GRAYSCALE)); 
	cv::gpu::GpuMat img2(cv::imread("../../Pictures/move2.jpg", CV_LOAD_IMAGE_GRAYSCALE)); 

	// Value for keypoints
	cv::gpu::GpuMat keypoints1GPU, keypoints2GPU; 
	// Value for descriptor
	cv::gpu::GpuMat descriptors1GPU, descriptors2GPU; 
	// orb detector and extractor
	cv::gpu::ORB_GPU orb(2000);
	// Create Object of DescriptorMatcher
	cv::gpu::BruteForceMatcher_GPU< cv::Hamming > matcher;
	// values of how maching
	std::vector< std::vector< cv::DMatch> > matches; 

	cv::TickMeter gpumeter;
	gpumeter.start();

	orb(img1, cv::gpu::GpuMat(), keypoints1GPU, descriptors1GPU);
	orb(img2, cv::gpu::GpuMat(), keypoints2GPU, descriptors2GPU);

	gpumeter.stop();
	std::cout << "ORB (GPU): " << gpumeter.getTimeMilli() << "ms" << std::endl;

	std::cout << "FOUND " << keypoints1GPU.cols << " keypoints on first image" << std::endl; 
	std::cout << "FOUND " << keypoints2GPU.cols << " keypoints on second image" << std::endl; 

	matcher.knnMatch(descriptors1GPU, descriptors2GPU, matches, 2); 

	//matching
	std::vector< cv::DMatch > good_matches;
	for(int k = 0; k < std::min(descriptors1GPU.rows-1,(int) matches.size()); k++) 
	{
		if((matches[k][0].distance < 0.6*(matches[k][1].distance)) && ((int) matches[k].size()<=2 && (int) matches[k].size()>0))
		{
			good_matches.push_back(matches[k][0]);
		}
	}    

	std::vector< cv::KeyPoint> keypoints1, keypoints2;
	std::vector< float> descriptors1, descriptors2;
	orb.downloadKeyPoints(keypoints1GPU, keypoints1);
	orb.downloadKeyPoints(keypoints2GPU, keypoints2);
	std::cout << "keypoint1 :" << keypoints1.size() << "  keypoint2 :" << keypoints2.size() << std::endl;

	cv::Mat img_matches; 
	cv::Mat img11, img22;
	img1.download(img11);
	img2.download(img22);
	cv::Mat outImg;

	cv::drawMatches(img11, keypoints1, img22, keypoints2, good_matches, img_matches);
	//cv::drawKeypoints(img11, kp1, outImg);

	cv::namedWindow("matches", 0);
	cv::imshow("matches", img_matches);

	cv::waitKey(0);

	return 0;
}
