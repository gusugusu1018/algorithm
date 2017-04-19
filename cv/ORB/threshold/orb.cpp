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
  Mat img1 = imread("../../Pictures/move1.jpg");
  Mat img2 = imread("../../Pictures/move2.jpg");

  // Creat Object of FeatureDetector
  //Ptr<FeatureDetector> detector = FeatureDetector::create("ORB");
  Ptr<FeatureDetector> detector = new ORB(80, 1.25f, 4, 7, 0, 2, 0, 7);

  // Create Object of DescriptionExtractor
  //Ptr<DescriptorExtractor> extractor = DescriptorExtractor::create("ORB");
  Ptr<DescriptorExtractor> extractor = new ORB(80, 1.25f, 4, 7, 0, 2, 0, 7);

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

  vector<DMatch> dmatchOK;

  int count = dmatch.size();
  int matchCount = 0;

  // 最小距離の取得
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

  // しきい値を設定する
  const double threshold = 2.0 * min_dist;//これが実験

  for (int i = 0; i < (int)dmatch.size(); i++){
    if (dmatch[i].distance < threshold){
      matchCount++;

      dmatchOK.push_back(dmatch[i]);
    }
  }

  // しきい値内の結果で、マッチング結果画像の作成
  Mat result;
  drawMatches(img1, keypoints1, img2, keypoints2, dmatchOK, result);
  imshow("matching prm thre", result);

  printf("match = %d\n", count);
  printf("matchOK = %d\n", matchCount);

  cvWaitKey(0);
  return 0;
}
