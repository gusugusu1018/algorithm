// fast_coners_detection.cpp
// 2017/11/21 : FAST Corners Detection

#include "fast_coners_detection.h"

void image_filter(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM) {
    RGB_IMAGE img_0(MAX_HEIGHT, MAX_WIDTH);
    RGB_IMAGE img_1(MAX_HEIGHT, MAX_WIDTH);
    RGB_IMAGE img_1_(MAX_HEIGHT, MAX_WIDTH);
    GRAY_IMAGE img_1g(MAX_HEIGHT, MAX_WIDTH);
    GRAY_IMAGE mask(MAX_HEIGHT, MAX_WIDTH);
    GRAY_IMAGE dmask(MAX_HEIGHT, MAX_WIDTH);
    GRAY_IMAGE img_2g(MAX_HEIGHT, MAX_WIDTH);
    RGB_IMAGE img_3(MAX_HEIGHT, MAX_WIDTH);
    RGB_PIXEL color(255, 0, 0);

    hls::AXIvideo2Mat(INPUT_STREAM, img_0);
    hls::Duplicate(img_0, img_1, img_1_);
    hls::CvtColor<HLS_BGR2GRAY>(img_1, img_1g);
    hls::FASTX(img_1g, mask, 10, true);
    hls::Dilate(mask, dmask);
    hls::PaintMask(img_1_, dmask, img_3, color);
    hls::Mat2AXIvideo(img_3, OUTPUT_STREAM);
}
