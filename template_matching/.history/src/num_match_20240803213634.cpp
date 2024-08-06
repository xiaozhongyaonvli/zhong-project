#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv){
    if (argc<3)
        std::cerr<<"Usage"<<argv[0]<<"<source_image> <target_image>"<<std::endl;
    cv::Mat img   = cv::imread("../images/"+std::string(argv[1]), cv::IMREAD_GRAYSCALE); //灰度图读取
    cv::Mat templ = cv::imread("../images/"+std::string(argv[2]), cv::IMREAD_GRAYSCALE);
    /*高斯模糊去噪*/
    cv::Mat blurred;
    cv::GaussianBlur(gray, blurred, cv::Size(5,5), 0); //这里的最后一个0 sigmaX：高斯核在 X 方向上的标准差。参数为0表示函数会根据高斯核的大小自动计算标准差。

    /*二值化处理*/
    double threshold_value = 127.0;
    double black_value = 0.0;
    double white_value = 255.0;
    for (int y=0; y<blurred.rows; y++)
        for (int x = 0; x<blurred.cols; x++){
            if (blurred.at<uchar>(y,x) < threshold_value) //at 查看或者修改点的一个方法，
                blurred.at<uchar>(y,x) = static_cast<uchar>(white_value);
            else blurred.at<uchar>(y,x) = static_cast<uchar>(black_value);
        }
    cv::imshow("img", blurred);
    cv::waitKey(0);

    /*轮廓检测*/
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(blurred, contours, cv::noArray(), cv::RETR_EXTERNAL， cv::CHAIN_APPROX_NONE);
    cv::Mat outline;
    blurred.copyTo(outline);
    cv::drawContours(outline, contours, -1, cv::Scalar(0,255,0),2);

    return 0;
}
