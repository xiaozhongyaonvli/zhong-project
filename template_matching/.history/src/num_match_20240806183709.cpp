#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

bool compare_nums(const std::vector<cv::Point>& a, const std::vector<cv::Point>& b)
{
    cv::Rect bounding_rect1 = cv::boundingRect(a);
    cv::Rect bounding_rect2 = cv::boundingRect(b);
    return bounding_rect1.x<bounding_rect2.x;
}

int main(int argc, char** argv){
    if (argc<3)
        std::cerr<<"Usage"<<argv[0]<<"<source_image> <target_image>"<<std::endl;
    cv::Mat img   = cv::imread("../images/"+std::string(argv[1]), cv::IMREAD_COLOR); //彩色读取
    cv::Mat templ = cv::imread("../images/"+std::string(argv[2]), cv::IMREAD_COLOR);
    
    /*转换为灰度图像*/
    cv::Mat img_gray;
    cv:cvtColor(img, img_gray,cv::COLOR_BGR2GRAY);

    /*高斯模糊去噪*/
    cv::Mat blurred;
    cv::GaussianBlur(img_gray, blurred, cv::Size(5,5), 0); //这里的最后一个0 sigmaX：高斯核在 X 方向上的标准差。参数为0表示函数会根据高斯核的大小自动计算标准差。

    /*二值化处理*/
    double threshold_value = 127.0;
    double black_value = 0.0;
    double white_value = 255.0;
    for (int y=0; y<blurred.rows; y++)
        for (int x = 0; x<blurred.cols; x++)
        {
            if (blurred.at<uchar>(y,x) < threshold_value) //at 查看或者修改点的一个方法，uchar无符号字符，常用于表示灰度值
                blurred.at<uchar>(y,x) = static_cast<uchar>(white_value);
            else blurred.at<uchar>(y,x) = static_cast<uchar>(black_value);
        }

    /*轮廓检测*/
    std::vector<std::vector<cv::Point> > contours; //一个元素是点的合集(一个轮廓)的容器
    cv::findContours(blurred, contours, cv::noArray(), cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);//第三个参数表示不需要轮廓的层次结构，第四个表示仅检测外轮廓，第五个表示不压缩
    int contours_size = contour.size();
    std::cout<<"检测到的轮廓有"<<contours_size<<"个"<<std::endl;
    sort(contours.begin(),contours.end(), compare_nums);
    for (int i=0; i<contours_size; ++i)
    {

    }
    cv::Mat outline;
    img.copyTo(outline);
    cv::drawContours(outline, contours, -1, cv::Scalar(0,0,255),3); //-1表示化所有，也可以指定某个，Scalar表示红色线，下一个参数表示2厚度
    cv::imshow("outline", outline);
    cv::waitKey(0);

    return 0;
}