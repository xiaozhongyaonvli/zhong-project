#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv){
    if (argc<3)
        std::cerr<<"Usage"<<argv[0]<<"<source_image> <target_image>"<<std::endl;
    cv::Mat img   = cv::imread("../images/"+std::string(argv[1]), cv::IMREAD_GRAYSCALE); //灰度图读取
    cv::Mat templ = cv::imread("../images/"+std::string(argv[2]), cv::IMREAD_GRAYSCALE);
    
    /*二值化处理*/
    double threshold_value = 127.0;
    double black_value = 0.0;
    double white_value = 255.0;
    for (int y=0; y<img.rows; y++)
        for (int x = 0; x<img.cols; x++){
            if (img.at<uchar>(y,x) < threshold_value)
                img.at<uchar>(y,x) = static_cast<uchar>()
        }
    return 0;
}