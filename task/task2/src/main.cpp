#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main(int argc, char** argv) {
    // 检查参数个数
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <source_image> <mask_image> <output_image>" << std::endl;
        return -1;
    }

    // 读取源图像和遮罩图像
    cv::Mat source_image = cv::imread("../images/" + std::string(argv[1]));
    cv::Mat mask_image = cv::imread("../images/" + std::string(argv[2]), cv::IMREAD_GRAYSCALE);

    if (source_image.empty() || mask_image.empty()) {
        std::cerr << "Could not open or find the image(s)!" << std::endl;
        return -1;
    }

    // 确保遮罩图像与源图像的尺寸相同  不相同会出问题(不过不同管格式）
    if (source_image.size() != mask_image.size()) {
        std::cerr << "Source image and mask image have different sizes. Resizing mask image to match source image." << std::endl;
        cv::Mat resized_mask;
        cv::resize(mask_image, resized_mask, source_image.size(), 0, 0, cv::INTER_LINEAR);
        mask_image = resized_mask;
    }

    // 确保遮罩图像是二值图像（0和255）
    cv::Mat binary_mask;
    cv::threshold(mask_image, binary_mask, 128, 255, cv::THRESH_BINARY);

    // 反转遮罩图像，将白色部分变为黑色，黑色部分保持不变
    cv::Mat inverted_mask;
    cv::bitwise_not(binary_mask, inverted_mask);

    // 创建一个黑色背景图像，类型和源图像相同
    cv::Mat black_background = cv::Mat::zeros(source_image.size(), source_image.type());

    // 应用反转后的遮罩，将遮罩图像的黑色部分保留在源图像中
    cv::Mat result_image;
    cv::bitwise_and(source_image, source_image, result_image, inverted_mask);

    // 1. 灰度转换
    cv::Mat gray_image;
    cv::cvtColor(result_image, gray_image, cv::COLOR_BGR2GRAY);

    // 2. 高斯模糊
    cv::Mat blurred_image;
    cv::GaussianBlur(gray_image, blurred_image, cv::Size(5, 5), 0);

    // 3. 边缘检测
    cv::Mat edges;
    cv::Canny(blurred_image, edges, 50, 150);

    // 4. 膨胀和腐蚀
    cv::Mat dilated_edges;
    cv::dilate(edges, dilated_edges, cv::Mat::ones(4, 4, CV_8U), cv::Point(-1, -1), 3);
    cv::Mat eroded_edges;
    cv::erode(dilated_edges, eroded_edges, cv::Mat::ones(4, 4, CV_8U), cv::Point(-1, -1), 1);

    // 创建一个结果图像，显示处理后的车道线
    cv::Mat lane_lines;
    cv::cvtColor(eroded_edges, lane_lines, cv::COLOR_GRAY2BGR);
    
    // 保存和显示车道线图像
    cv::imwrite("../images/lane_lines.png", lane_lines);

    cv::namedWindow("Lane Lines", cv::WINDOW_AUTOSIZE);
    cv::imshow("Lane Lines", lane_lines);

    // 等待用户按键
    cv::waitKey(0);

    return 0;
}
