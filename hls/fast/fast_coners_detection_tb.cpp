// fast_coners_detection_tb.cpp
// 2017/11/21 : FAST Corners Detection

#include <iostream>
#include "hls_opencv.h"
#include "fast_coners_detection.h"

#define INPUT_IMAGE        "test.bmp"
#define OUTPUT_IMAGE    "test_result.bmp"
#define OUTPUT_IMAGE_CV    "test_result_cv.bmp"

int main (int argc, char** argv) {
    // read image
    cv::Mat src = cv::imread(INPUT_IMAGE);
    cv::Mat dst(src.rows, src.cols, CV_8UC3);
    AXI_STREAM src_axi, dst_axi;
    // convert src from cv Mat to axi stream src
    cvMat2AXIvideo(src, src_axi);
    // Fast coners detection
    image_filter(src_axi, dst_axi);
    //image_filter(src_axi, dst_axi, src.rows, src.cols);
    // convert dst_axt from axi stream to cv Mat
    AXIvideo2cvMat(dst_axi, dst);
    // write image
    cv::imwrite(OUTPUT_IMAGE, dst);
    return 0;
}
