#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <iostream>

std::string win_src = "src";

cv::Mat src;
cv::Mat src_gray;
cv::Mat drawing;
int thresh = 10;
int max_thresh = 255;
// random number generator
cv::RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
//   src = imread( argv[1], 1 );


	cv::VideoCapture capture(0);
	if(!capture.isOpened()) {
		std::cout << "error" << std::endl;
		return -1;
	}

    while (1){
    	capture >> src;
        /// Convert image to gray and blur it
        cv::cvtColor( src, src_gray, CV_BGR2GRAY );
        cv::blur( src_gray, src_gray, cv::Size(3,3) );
        /// Create Window
        cv::namedWindow (win_src, CV_WINDOW_AUTOSIZE);
        cv::imshow(win_src, src);
        
        cv::createTrackbar( " Canny thresh:", "src", &thresh, max_thresh, thresh_callback );
        thresh_callback( 0, 0 );

		if (cv::waitKey(1) == 'q') break;
	}
	return 0;
}
/** @function thresh_callback */
void thresh_callback(int, void* )
{
  cv::Mat canny_output;
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchy;

  /// Detect edges using canny
  cv::Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  cv::findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

  /// Draw contours
  drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//       Scalar color = Scalar( 255,255,255);
       cv::drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
     }

  /// Show in a window
  cv::namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  cv::imshow( "Contours", drawing );
}
