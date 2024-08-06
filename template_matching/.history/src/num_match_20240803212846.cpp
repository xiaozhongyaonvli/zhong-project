#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv){
    if (argc<3)
        std::cerr<<"Usage"<<argv[0]<<"<source_image> <target_image>"<<std::endl;
    cv::Mat img   = cv::imread("../images/"+std::string(argv[1]), cv::IMREAD_GRAYSCALE); //灰度图读取
    cv::Mat templ = cv::imread("../images/"+std::string(argv[2]), cv::IMREAD_GRAYSCALE);
    /*高斯模糊去噪*/
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5,5), 0);

    /*二值化处理*/
    double threshold_value = 127.0;
    double black_value = 0.0;
    double white_value = 255.0;
    for (int y=0; y<blurred.rows; y++)
        for (int x = 0; x<blurred.cols; x++){
            if (blurred.at<uchar>(y,x) < threshold_value)
                blurred.at<uchar>(y,x) = static_cast<uchar>(white_value);
            else blurred.at<uchar>(y,x) = static_cast<uchar>(black_value);
        }
    cv::imshow("img", img);
    cv::waitKey(0);
    // at 方法用于访问和修改 cv::Mat 对象中的单个像素值
    // at<T>(row, col)
    //T 是图像数据的类型（如 uchar、float、int 等）。
    // row 和 col 是像素的行和列索引。
    // uchar 表示无符号字符类型，即 unsigned char，通常用于表示灰度图像中的像素值

    
    return 0;
}
