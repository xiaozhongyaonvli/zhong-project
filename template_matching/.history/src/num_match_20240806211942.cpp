#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// 比较函数，根据轮廓矩形的列来排序
bool compare_nums(const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) 
{
    cv::Rect bounding_rect1 = cv::boundingRect(a);
    cv::Rect bounding_rect2 = cv::boundingRect(b);
    return bounding_rect1.x < bounding_rect2.x;
}

int main(int argc, char** argv){
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <source_image> <target_image>" << std::endl;
        return -1;
    }
    
    // 读取图像
    cv::Mat img = cv::imread("../images/" + std::string(argv[1]), cv::IMREAD_COLOR); // 彩色读取
    cv::Mat templ = cv::imread("../images/" + std::string(argv[2]), cv::IMREAD_COLOR);
    
    // 转换为灰度图像
    cv::Mat img_gray;
    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
    
    // 高斯模糊去噪
    cv::Mat blurred;
    cv::GaussianBlur(img_gray, blurred, cv::Size(5, 5), 0);
    
    // 二值化处理
    double threshold_value = 127.0;
    double black_value = 0.0;
    double white_value = 255.0;
    for (int y = 0; y < blurred.rows; y++) {
        for (int x = 0; x < blurred.cols; x++) {
            if (blurred.at<uchar>(y, x) < threshold_value) {
                blurred.at<uchar>(y, x) = static_cast<uchar>(white_value);
            } else {
                blurred.at<uchar>(y, x) = static_cast<uchar>(black_value);
            }
        }
    }
    
    // 轮廓检测
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(blurred, contours, cv::noArray(), cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    int contours_size = contours.size();
    std::cout << "检测到的轮廓有" << contours_size << "个" << std::endl;
    
    // 对轮廓进行排序
    sort(contours.begin(), contours.end(), compare_nums);
    
    cv::Mat outline;
    img.copyTo(outline);
    std::vector<cv::Mat> num_imgs;
    for (int i = 0; i < contours_size; ++i) {
        // 画轮廓矩形
        cv::Rect bounding_rect = cv::boundingRect(contours[i]);
        cv::rectangle(outline, bounding_rect, cv::Scalar(0, 0, 255), 3);
        
        // 检查并调整矩形边界，确保在图像范围内
        bounding_rect &= cv::Rect(0, 0, img.cols, img.rows);
        
        // 从原始图像中提取感兴趣区域
        cv::Mat roi = img(bounding_rect).clone();
        num_imgs.push_back(roi);
    }
    
    // 显示轮廓
    cv::drawContours(outline, contours, -1, cv::Scalar(0, 0, 255), 3);
    cv::imshow("outline", outline);
    std::cout << "数字图像有" << num_imgs.size() << "个" << std::endl;
    
    // 显示提取的一个数字图像
    if (!num_imgs.empty()) {
        cv::imshow("one_img", num_imgs[1]);
    }
    
    cv::waitKey(0);
    
    return 0;
}