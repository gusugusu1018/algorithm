#include "opencv2/highgui/highgui.hpp"                                                                                                                                                                               
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;

int main(int argc, char** argv )
{
  Mat img1 = imread("../../../Pictures/move1.jpg");
  Mat img2 = imread("../../../Pictures/move2.jpg");

  // Creat Object of FeatureDetector
  Ptr<FeatureDetector> detector = FeatureDetector::create("ORB");

  // Create Object of DescriptionExtractor
  Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("ORB");

  // Create Object of DescriptorMatcher
  Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

  // Value for data of features
  vector<KeyPoint> keypoints1;
  vector<KeyPoint> keypoints2;

  // Extract features
  detector->detect(img1, keypoints1);
  detector->detect(img2, keypoints2);

  // 画像の特徴情報を格納するための変数
  Mat descriptor1;
  Mat descriptor2;

  // 特徴記述の計算を実行
  extractor->compute(img1, keypoints1, descriptor1);
  extractor->compute(img2, keypoints2, descriptor2);

  // 特徴点のマッチング情報を格納する変数
  vector<DMatch> dmatch;

  // 特徴点マッチングの実行
  matcher->match(descriptor1, descriptor2, dmatch);

  // マッチング結果画像の作成
  Mat result;
  drawMatches(img1, keypoints1, img2, keypoints2, dmatch, result);
  imshow("matching", result);

  cvWaitKey(0);
  return 0;
}
