#include <opencv2/opencv.hpp>
#include <iostream>
int main(int argc, char** argv) {
if (argc != 2) {
std::cout << "Usage: " << argv[0] << " <Image Path>" << std::endl;
return -1;
}
cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);
if (image.empty()) {
std::cout << "Could not open or find the image" << std::endl;
return -1;
}
cv::imshow("Display window", image);cv::waitKey(0);
return 0;
}
