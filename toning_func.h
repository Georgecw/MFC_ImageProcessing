#pragma once

#include<opencv2\opencv.hpp>
//using namespace cv;
//using namespace std;

#define max2(a,b) (a>b?a:b)
#define max3(a,b,c) (a>b?max2(a,c):max2(b,c))
#define min2(a,b) (a<b?a:b)
#define min3(a,b,c) (a<b?min2(a,c):min2(b,c))

//函数申明
cv::Mat Brightness(cv::Mat src, float brightness, int contrast);				//亮度+对比度。
cv::Mat Saturation(cv::Mat src, int saturation);								//饱和度
cv::Mat HighLight(cv::Mat src, int highlight);									//高光
cv::Mat ColorTemperature(cv::Mat src, int warm);								//暖色调
cv::Mat Shadow(cv::Mat src, int shadow);										//阴影

cv::Mat Sharpen(cv::Mat input, int percent, int type);							//图像锐化
cv::Mat Grainy(cv::Mat src, int level);											//颗粒感
cv::Mat Cartoon(cv::Mat src, double clevel, int d, double sigma, int size);		//漫画效果
cv::Mat WhiteBalcane_PRA(cv::Mat src);											//白平衡-完美反射算法（效果偏白）
cv::Mat WhiteBalcane_Gray(cv::Mat src);											//白平衡-灰度世界算法（效果偏蓝）
cv::Mat Relief(cv::Mat src);													//浮雕
cv::Mat Eclosion(cv::Mat src, cv::Point center, float level);					//羽化
