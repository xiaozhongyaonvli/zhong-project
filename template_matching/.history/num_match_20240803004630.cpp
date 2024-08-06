#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv){
    if (argc<3)
        std::cerr<<"Usage"<<argv[0]<<"<source_image> <target_image>"<<std::endl;
    cv::Mat img = cv::imread
}