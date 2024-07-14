#include <opencv2/opencv.hpp>
#include <iostream>
#include "my_header.h"
using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	if (argc!=2){
		std::cerr<<"Usage:"<<argv[0]<<"<video_path>"<<std::endl;
		return -1;
	}

	// 打开源文件
	cv::VideoCapture cap(argv[1]);
	if (!cap.isOpened()){
		std::cerr<<"Error: Could not open video."<<std::endl;
		return -1;
	}

	cv::Mat frame;
	while (true){
		bool ret = cap.read(frame);
		if (!ret){
			std::cout<<"End of video"<<std::endl;
			break;
		}

		cv::imshow("Video Playback", frame);

		// 按'q'退出播放
		if (cv::waitKey(25)=='q'){
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}
