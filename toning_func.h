#pragma once

#include<opencv2\opencv.hpp>
//using namespace cv;
//using namespace std;

#define max2(a,b) (a>b?a:b)
#define max3(a,b,c) (a>b?max2(a,c):max2(b,c))
#define min2(a,b) (a<b?a:b)
#define min3(a,b,c) (a<b?min2(a,c):min2(b,c))

//��������
cv::Mat Brightness(cv::Mat src, float brightness, int contrast);				//����+�Աȶȡ�
cv::Mat Saturation(cv::Mat src, int saturation);								//���Ͷ�
cv::Mat HighLight(cv::Mat src, int highlight);									//�߹�
cv::Mat ColorTemperature(cv::Mat src, int warm);								//ůɫ��
cv::Mat Shadow(cv::Mat src, int shadow);										//��Ӱ

cv::Mat Sharpen(cv::Mat input, int percent, int type);							//ͼ����
cv::Mat Grainy(cv::Mat src, int level);											//������
cv::Mat Cartoon(cv::Mat src, double clevel, int d, double sigma, int size);		//����Ч��
cv::Mat WhiteBalcane_PRA(cv::Mat src);											//��ƽ��-���������㷨��Ч��ƫ�ף�
cv::Mat WhiteBalcane_Gray(cv::Mat src);											//��ƽ��-�Ҷ������㷨��Ч��ƫ����
cv::Mat Relief(cv::Mat src);													//����
cv::Mat Eclosion(cv::Mat src, cv::Point center, float level);					//��
