#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> // 添加图像处理头文件
#include <iostream>
#include <string>

int main(int argc, char** argv) /*argc是命令行参数的个数包含程序本身，argv数组包含了所有命令行参数*/
 {                              //argv[0]通常是程序的名称，其他才是参数记住
    // 检查参数个数                          
    if (argc<8)
    {    std::cerr<<"Usage"<<argv[0]<<"Wrong number of parameters "<<"Please enter like this <image_path> <scale> <interpolation> <tx> <ty> <Rotation_center> <angle>"<<std::endl;
        return -1;
    }

    std::string image_path = "../images/"+std::string(argv[1]);//拼接得到地址
    double image_scale = std::stod(argv[2]);      //缩放比例
    std::string interpolation_str = argv[3];      //插值方法
    int tx = std::stoi(argv[4]);                  //水平位移
    int ty = std::stoi(argv[5]);                  //垂直位移 
    std::string Rotation_center = argv[6];        //选择旋转中心模式
    double Rotation_angle = std::stod(argv[7]);                 //旋转角度 //一般都以度为单位不需要转换

    // 读取图像
    cv::Mat image = cv::imread(image_path);
    if (image.empty()) 
    {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    // 处理旋转中心，插值方法；
    cv::Point2f center;
    if (Rotation_center == "origin")
        cv::Point2f center(0,0);
    else if (Rotation_center == "center")
        cv::Point2f center(image.cols/2.0,image.rows/2.0);
    
    int interpolation_method = 0;
    if (interpolation_str == "NEAREST") interpolation_method = cv::INTER_NEAREST;
    else if (interpolation_str == "LINEAR") interpolation_method = cv::INTER_LINEAR;

    // 定义平移变换矩阵
    cv::Mat translation_matrix = (cv::Mat_<double>(2, 3) << 1, 0, tx, 0, 1, ty);

    // 定义旋转变换矩阵
    cv::Mat rotation_matrix = cv::getRotationMatrix2D(center, Rotation_angle, image_scale);

    // 合并平移和旋转变换矩阵
    cv::Mat combined_matrix = rotation_matrix.clone(); //深拷贝厉害了哈哈
    combined_matrix.at<double>(0, 2) += translation_matrix.at<double>(0, 2); //这里的第一个是行索引第二个是列索引
    combined_matrix.at<double>(1, 2) += translation_matrix.at<double>(1, 2);

    // 应用变换
    cv::Mat transformed_image;
    cv::warpAffine(image, transformed_image, combined_matrix, image.size(), interpolation_method,cv::BORDER_CONSTANT,cv::Scalar(255,255,255));

    // 创建窗口并显示变换后的图像
    cv::namedWindow("Transformed Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Transformed Image", transformed_image);

    // 等待用户按键
    cv::waitKey(0);

    return 0;
}
