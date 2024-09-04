#pragma once

#include<opencv2\opencv.hpp>
//using namespace cv;
//using namespace std;

#define max2(a,b) (a>b?a:b)
#define max3(a,b,c) (a>b?max2(a,c):max2(b,c))
#define min2(a,b) (a<b?a:b)
#define min3(a,b,c) (a<b?min2(a,c):min2(b,c))

//��������
cv::Mat Brightness(cv::Mat src, float brightness, int contrast, CProgressCtrl& progress);				//����+�Աȶȡ�
cv::Mat Saturation(cv::Mat src, int saturation,CProgressCtrl& progress);								//���Ͷ�

cv::Mat ColorTemperature(cv::Mat src, int warm, CProgressCtrl& progress);								//ůɫ��

cv::Mat Sharpen(cv::Mat input, int percent, int type);							//ͼ����
