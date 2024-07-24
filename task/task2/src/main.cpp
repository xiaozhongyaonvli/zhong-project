#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int main(int argc, char** argv) {
    // 检查参数个数
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <source_image> <mask_image>" << std::endl;
        return -1;
    }

    // 读取源图像和遮罩图像
    cv::Mat source_image = cv::imread("../images/" + std::string(argv[1]));
    cv::Mat mask_image = cv::imread("../images/" + std::string(argv[2]), cv::IMREAD_GRAYSCALE);

    if (source_image.empty() || mask_image.empty()) {
        std::cerr << "Could not open or find the image(s)!" << std::endl;
        return -1;
    }

    // 确保遮罩图像与源图像的尺寸相同
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

    // 将图像分块处理的大小
    int block_size = 100;
    cv::Mat result_image = cv::Mat::zeros(source_image.size(), source_image.type());

    // 遍历每个块
    for (int y = 0; y < source_image.rows; y += block_size) {
        for (int x = 0; x < source_image.cols; x += block_size) {
            // 定义块的范围
            cv::Rect block_rect(x, y, block_size, block_size);
            block_rect &= cv::Rect(0, 0, source_image.cols, source_image.rows);

            // 获取图像块
            cv::Mat source_block = source_image(block_rect);
            cv::Mat mask_block = inverted_mask(block_rect);

            // 应用反转后的遮罩，将遮罩图像的黑色部分保留在源图像中
            cv::Mat block_result;
            cv::bitwise_and(source_block, source_block, block_result, mask_block);

            // 灰度转换
            cv::Mat gray_image;
            cv::cvtColor(block_result, gray_image, cv::COLOR_BGR2GRAY);

            // 高斯模糊
            cv::Mat blurred_image;
            cv::GaussianBlur(gray_image, blurred_image, cv::Size(5, 5), 0);

            // 边缘检测
            cv::Mat edges;
            cv::Canny(blurred_image, edges, 50, 150);

            // 膨胀和腐蚀
            cv::Mat dilated_edges;
            cv::dilate(edges, dilated_edges, cv::Mat::ones(2, 2, CV_8U), cv::Point(-1, -1), 1);
            cv::Mat eroded_edges;
            cv::erode(dilated_edges, eroded_edges, cv::Mat::ones((int)4*(1.5-y*1.0/source_image.rows), (int)4*(1.5-y*1.0/source_image.rows), CV_8U), cv::Point(-1, -1), 1);

            // 霍夫变换检测直线
            std::vector<cv::Vec4i> lines;
            if (y*4<source_image.rows)
                cv::HoughLinesP(dilated_edges, lines, 1, CV_PI / 180, 150, 60-y*1.0/source_image.rows*10, 10);
            else
                cv::HoughLinesP(dilated_edges, lines, 1, CV_PI / 180, 8, 5, 2);
            // 在结果图像的相应块位置绘制检测到的直线
            for (size_t i = 0; i < lines.size(); i++) {
                cv::Vec4i l = lines[i];
                cv::line(result_image, cv::Point(l[0] + x, l[1] + y), cv::Point(l[2] + x, l[3] + y), cv::Scalar(255, 255, 255), 4, cv::LINE_AA);
            }
        }
    }

    // 保存和显示车道线图像
    cv::imwrite("../images/lane_lines.png", result_image);

    cv::namedWindow("Lane Lines", cv::WINDOW_AUTOSIZE);
    cv::imshow("Lane Lines", result_image);

    // 等待用户按键
    cv::waitKey(0);

    return 0;
}
