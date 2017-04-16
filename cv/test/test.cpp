#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/features2d/features2d.hpp"

int main( int argc, char** argv )
{
  cv::Mat src;
  
  src = cv::imread( argv[1], CV_LOAD_IMAGE_COLOR );
  if(! src.data ) {
    std::cout <<  "Could not open or find the image" << std::endl ;
    return -1;
  }
  cv::namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
  cv::imshow( "Display window", src );
  cv::waitKey(0);
  return 0;

}
