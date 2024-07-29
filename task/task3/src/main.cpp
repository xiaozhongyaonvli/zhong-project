#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv) {

    // 检查参数个数
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <source_image> <target_image>" << std::endl;
        return -1;
    }

    // 读取主图像和模板图像
    cv::Mat img = cv::imread(argv[1], cv::IMREAD_COLOR);
    cv::Mat templ = cv::imread(argv[2], cv::IMREAD_COLOR);

    if (img.empty() || templ.empty()) {
        std::cerr << "Could not open or find the images!" << std::endl;
        return -1;
    }

    // 创建ORB特征检测器
    cv::Ptr<cv::ORB> detector = cv::ORB::create();

    // 检测特征点
    std::vector<cv::KeyPoint> keypoints_img, keypoints_templ;
    cv::Mat descriptors_img, descriptors_templ;
    detector->detectAndCompute(img, cv::Mat(), keypoints_img, descriptors_img);
    detector->detectAndCompute(templ, cv::Mat(), keypoints_templ, descriptors_templ);

    // 使用BFMatcher进行特征点匹配
    cv::BFMatcher matcher(cv::NORM_HAMMING, true);
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors_img, descriptors_templ, matches);

    // 筛选匹配点
    double max_dist = 0; double min_dist = 100;
    for (int i = 0; i < descriptors_img.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }
    std::vector<cv::DMatch> good_matches;
    for (int i = 0; i < descriptors_img.rows; i++) {
        if (matches[i].distance <= std::max(2 * min_dist, 30.0)) {
            good_matches.push_back(matches[i]);
        }
    }

    // 绘制匹配结果
    cv::Mat img_matches;
    cv::drawMatches(img, keypoints_img, templ, keypoints_templ, good_matches, img_matches);

    // 显示结果
    cv::imshow("Matches", img_matches);
    cv::waitKey(0);

    return 0;
}
