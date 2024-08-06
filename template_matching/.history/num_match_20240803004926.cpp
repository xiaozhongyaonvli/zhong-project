#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv){
    if (argc<3)
        std::cerr<<"Usage"<<argv[0]<<"<source_image> <target_image>"<<std::endl;
    cv::Mat img   = cv::imread("../images/"+std::string(argv[1]), cv::IMREAD_GRAYSCALE);
    cv::Mat templ = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);

}