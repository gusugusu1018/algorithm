#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

string win_src = "src";

Mat src; Mat src_gray; Mat drawing;
int thresh = 10;
int max_thresh = 255;
RNG rng(12345);

/// Function header
void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
//   src = imread( argv[1], 1 );


	VideoCapture capture(0);
	if(!capture.isOpened()) {
		cout << "error" << endl;
		return -1;
	}

    while (1){
    	capture >> src;
        /// Convert image to gray and blur it
        cvtColor( src, src_gray, CV_BGR2GRAY );
        blur( src_gray, src_gray, Size(3,3) );
        /// Create Window
        namedWindow (win_src, CV_WINDOW_AUTOSIZE);
        imshow(win_src, src);
        
        createTrackbar( " Canny thresh:", "src", &thresh, max_thresh, thresh_callback );
        thresh_callback( 0, 0 );

		if (waitKey(1) == 'q') break;
	}
	return 0;
}
/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using canny
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Draw contours
  drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//       Scalar color = Scalar( 255,255,255);
       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  imshow( "Contours", drawing );
}
