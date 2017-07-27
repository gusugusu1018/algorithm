#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(void)
{
/*
    cv::Mat target = cv::imread("../../images/redbull_target.jpg");
    cv::Mat scene = cv::imread("../../images/redbull_scene.jpg");
    cv::Mat t_gray = cv::imread("../../images/redbull_target.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat s_gray = cv::imread("../../images/redbull_scene.jpg", cv::IMREAD_GRAYSCALE);
*/
    cv::Mat target = cv::imread("../../images/genmai_target.jpg");
    cv::Mat scene = cv::imread("../../images/genmai_scene.jpg");
    cv::Mat t_gray = cv::imread("../../images/genmai_target.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat s_gray = cv::imread("../../images/genmai_scene.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat dst;

    // 時間計算のための周波数
    double f = 1000.0 / cv::getTickFrequency();

    int64 time_s; //スタート時間
    double time_detect; // 検出エンド時間
    double time_match; // マッチングエンド時間


    // 特徴点検出と特徴量計算

    cv::Ptr<cv::Feature2D> feature;
    std::stringstream ss;

    feature = cv::xfeatures2d::SIFT::create();
    ss << "SIFT";
    std::cout << "--- 計測（SIFT ） ---" << std::endl;


    //******************************
    // キーポイント検出と特徴量記述
    //******************************
    std::vector<cv::KeyPoint> kpts1, kpts2;
    cv::Mat desc1, desc2;

    feature->detectAndCompute(t_gray, cv::noArray(), kpts1, desc1);

    time_s = cv::getTickCount(); // 時間計測 Start
    feature->detectAndCompute(s_gray, cv::noArray(), kpts2, desc2);
    time_detect = (cv::getTickCount() - time_s)*f; // 時間計測 Stop

    if (desc2.rows == 0){
        std::cout << "WARNING: 特徴点検出できず" << std::endl;
        return 1;
    }

    //*******************
    // 特徴量マッチング
    //*******************
    auto matchtype = feature->defaultNorm(); // SIFT, SURF: NORM_L2
                                             // BRISK, ORB, KAZE, A-KAZE: NORM_HAMMING
    cv::BFMatcher matcher(matchtype);
    std::vector<std::vector<cv::DMatch >> knn_matches;


    time_s = cv::getTickCount(); // 時間計測 Start
    // 上位2点
    matcher.knnMatch(desc1, desc2, knn_matches, 2);
    time_match = (cv::getTickCount() - time_s)*f; // 時間計測 Stop


    //***************
    // 対応点を絞る
    //***************
    const auto match_par = .6f; //対応点のしきい値
    std::vector<cv::DMatch> good_matches;

    std::vector<cv::Point2f> match_point1;
    std::vector<cv::Point2f> match_point2;

    for (size_t i = 0; i < knn_matches.size(); ++i) {
        auto dist1 = knn_matches[i][0].distance;
        auto dist2 = knn_matches[i][1].distance;

        //良い点を残す（最も類似する点と次に類似する点の類似度から）
        if (dist1 <= dist2 * match_par) {
            good_matches.push_back(knn_matches[i][0]);
            match_point1.push_back(kpts1[knn_matches[i][0].queryIdx].pt);
            match_point2.push_back(kpts2[knn_matches[i][0].trainIdx].pt);
        }
    }

    //ホモグラフィ行列推定
    cv::Mat masks;
    cv::Mat H;
    if (match_point1.size() != 0 && match_point2.size() != 0) {
        H = cv::findHomography(match_point1, match_point2, masks, cv::RANSAC, 3.f);
    }

    //RANSACで使われた対応点のみ抽出
    std::vector<cv::DMatch> inlierMatches;
    for (auto i = 0; i < masks.rows; ++i) {
        uchar *inlier = masks.ptr<uchar>(i);
        if (inlier[0] == 1) {
            inlierMatches.push_back(good_matches[i]);
        }
    }
    //特徴点の表示
    cv::drawMatches(target, kpts1, scene, kpts2, good_matches, dst);

    //インライアの対応点のみ表示
    cv::drawMatches(target, kpts1, scene, kpts2, inlierMatches, dst);

    if (!H.empty()) {

        //
        // 対象物体画像からコーナーを取得 ( 対象物体が"検出"される )
        std::vector<cv::Point2f> obj_corners(4);
        obj_corners[0] = cv::Point2f(.0f, .0f);
        obj_corners[1] = cv::Point2f(static_cast<float>(target.cols), .0f);
        obj_corners[2] = cv::Point2f(static_cast<float>(target.cols), static_cast<float>(target.rows));
        obj_corners[3] = cv::Point2f(.0f, static_cast<float>(target.rows));

        // シーンへの射影を推定
        std::vector<cv::Point2f> scene_corners(4);
        cv::perspectiveTransform(obj_corners, scene_corners, H);

        // コーナー間を線で結ぶ ( シーン中のマップされた対象物体 - シーン画像 )
        float w = static_cast<float>(target.cols);
        cv::line(dst, scene_corners[0] + cv::Point2f(w, .0f), scene_corners[1] + cv::Point2f(w, .0f), cv::Scalar(0, 255, 0), 4);
        cv::line(dst, scene_corners[1] + cv::Point2f(w, .0f), scene_corners[2] + cv::Point2f(w, .0f), cv::Scalar(0, 255, 0), 4);
        cv::line(dst, scene_corners[2] + cv::Point2f(w, .0f), scene_corners[3] + cv::Point2f(w, .0f), cv::Scalar(0, 255, 0), 4);
        cv::line(dst, scene_corners[3] + cv::Point2f(w, .0f), scene_corners[0] + cv::Point2f(w, .0f), cv::Scalar(0, 255, 0), 4);
    }


    double beta = 1.2;
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 40), cv::FONT_HERSHEY_SIMPLEX, beta-.1, cv::Scalar(255, 255, 255), 1, CV_AA);
    ss.str("");
    ss << "Detection & Description";
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 70), cv::FONT_HERSHEY_SIMPLEX, beta - .1, cv::Scalar(0, 255, 255), 1, CV_AA);
    ss.str("");
    ss << "Time: " << time_detect << " [ms]";
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 95), cv::FONT_HERSHEY_SIMPLEX, beta - .1, cv::Scalar(0, 255, 255), 1, CV_AA);
    ss.str("");
    ss << "Matching";
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 120), cv::FONT_HERSHEY_SIMPLEX, beta - .1, cv::Scalar(0, 255, 255), 1, CV_AA);
    ss.str("");
    ss << "Time: " << time_match << " [ms]";
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 145), cv::FONT_HERSHEY_SIMPLEX, beta - .1, cv::Scalar(0, 255, 255), 1, CV_AA);

    ss.str("");
    ss << "--Matches--";
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 170), cv::FONT_HERSHEY_SIMPLEX, beta - .1, cv::Scalar(255, 255, 0), 1, CV_AA);
    ss.str("");
    ss << "Good Matches: " << good_matches.size();
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 190), cv::FONT_HERSHEY_SIMPLEX, beta - .1, cv::Scalar(255, 255, 0), 1, CV_AA);

    ss.str("");
    ss << "Inlier: " << inlierMatches.size();
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 220), cv::FONT_HERSHEY_SIMPLEX, beta - .1, cv::Scalar(255, 255, 0), 1, CV_AA);

    ss.str("");
    auto ratio = .0;
    if (good_matches.size() != .0)
        ratio = inlierMatches.size()*1.0 / good_matches.size();
    ss << "Inlier ratio: " << ratio;
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 240), cv::FONT_HERSHEY_SIMPLEX, beta - .1, cv::Scalar(255, 255, 0), 1, CV_AA);


    ss.str("");
    ss << "Target KeyPoints: " << kpts1.size();
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 270), cv::FONT_HERSHEY_SIMPLEX, beta - .1, cv::Scalar(255, 0, 255), 1, CV_AA);
    ss.str("");
    ss << "Scene KeyPoints: " << kpts2.size();
    cv::putText(dst, ss.str(), cv::Point(10, target.rows + 290), cv::FONT_HERSHEY_SIMPLEX, beta - .1, cv::Scalar(255, 0, 255), 1, CV_AA);

    std::cout << "検出時間: " << time_detect << " [ms]" << std::endl;
    std::cout << "照合時間: " << time_match << " [ms]" << std::endl;
    std::cout << "Good Matches: " << good_matches.size() << std::endl;
    std::cout << "Inlier: " << inlierMatches.size() << std::endl;
    std::cout << "Inlier ratio: " << ratio << std::endl;
    std::cout << "target Keypoints: " << kpts1.size() << std::endl;
    std::cout << "scene Keypoints: " << kpts2.size() << std::endl;
    std::cout << "target match_points: " << match_point1.size() << std::endl;
    std::cout << "scene match_points: " << match_point2.size() << std::endl;

    cv::imshow("dst",dst);
    cv::waitKey(0);
    return 0;
}
