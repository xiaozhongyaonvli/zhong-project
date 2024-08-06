#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv){
    if (argc<3)
        std::cerr<<"Usage"<<argv[0]<<"<source_image> <target_image>"<<std::endl;
    cv::Mat img   = cv::imread("../images/"+std::string(argv[1]), cv::IMREAD_GRAYSCALE); //灰度图读取
    cv::Mat templ = cv::imread("../images/"+std::string(argv[2]), cv::IMREAD_GRAYSCALE);
    
    /*二值化处理*/
    cv::Mat binary_img;
    double threshold_value = 127.0;
    double max_binary_value = 255.0;
    cv

    return 0;
}