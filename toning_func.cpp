#include "pch.h"
#include "toning_func.h"

//�����Աȶ�������
cv::Mat Brightness(cv::Mat src, float brightness, int contrast, CProgressCtrl& progress)
{
	cv::Mat dst;
	dst = cv::Mat::zeros(src.size(), src.type());		//�½��հ�ģ�壺��С/������ԭͼ��һ�£�����ֵȫ0��
	int height = src.rows;								//��ȡͼ��߶�
	int width = src.cols;								//��ȡͼ����
	float alpha = brightness;							//���ȣ�0~1Ϊ����1~������Ϊ����
	float beta = contrast;								//�Աȶ�

	int num_pixel = height * width;
	int num_step = num_pixel / 100;
	int counter = 0;

	cv::Mat template1;
	src.convertTo(template1, CV_32F);					//��CV_8UC1ת��ΪCV32F1���ݸ�ʽ��
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			if (src.channels() == 3)
			{
				float b = template1.at<cv::Vec3f>(row, col)[0];		//��ȡͨ��������ֵ��blue��
				float g = template1.at<cv::Vec3f>(row, col)[1];		//��ȡͨ��������ֵ��green��
				float r = template1.at<cv::Vec3f>(row, col)[2];		//��ȡͨ��������ֵ��red��

				//cv::saturate_cast<uchar>(vaule)����ע�⣬valueֵ��Χ������0~255֮�䡣
				dst.at<cv::Vec3b>(row, col)[0] = cv::saturate_cast<uchar>(b * alpha + beta);		//�޸�ͨ��������ֵ��blue��
				dst.at<cv::Vec3b>(row, col)[1] = cv::saturate_cast<uchar>(g * alpha + beta);		//�޸�ͨ��������ֵ��green��
				dst.at<cv::Vec3b>(row, col)[2] = cv::saturate_cast<uchar>(r * alpha + beta);		//�޸�ͨ��������ֵ��red��
			}
			else if (src.channels() == 1)
			{
				float v = src.at<uchar>(row, col);											//��ȡͨ��������ֵ������
				dst.at<uchar>(row, col) = cv::saturate_cast<uchar>(v * alpha + beta);		//�޸�ͨ��������ֵ������
				//saturate_cast<uchar>����Ҫ��Ϊ�˷�ֹ��ɫ������������color<0����color����0�����color>255����color����255��
			}
			counter++;
			if (counter % num_step == 0)
				progress.StepIt();
		}
	}
	progress.SetPos(100);
	return dst;
}

//--------------------------------------------------------------------------------
// ���Ͷ�
cv::Mat Saturation(cv::Mat src, int saturation,CProgressCtrl&  progress)
{
	float Increment = saturation * 1.0f / 100;
	cv::Mat temp = src.clone();
	int row = src.rows;
	int col = src.cols;

	int num_pixel = row * col;
	int num_step = num_pixel / 100;
	int counter = 0;

	for (int i = 0; i < row; ++i)
	{
		uchar* t = temp.ptr<uchar>(i);
		uchar* s = src.ptr<uchar>(i);
		for (int j = 0; j < col; ++j)
		{
			uchar b = s[3 * j];
			uchar g = s[3 * j + 1];
			uchar r = s[3 * j + 2];
			float max = max3(r, g, b);
			float min = min3(r, g, b);
			float delta, value;
			float L, S, alpha;
			delta = (max - min) / 255;
			if (delta == 0)
				continue;
			value = (max + min) / 255;
			L = value / 2;
			if (L < 0.5)
				S = delta / value;
			else
				S = delta / (2 - value);
			if (Increment >= 0)
			{
				if ((Increment + S) >= 1)
					alpha = S;
				else
					alpha = 1 - Increment;
				alpha = 1 / alpha - 1;
				t[3 * j + 2] = static_cast<uchar>(r + (r - L * 255) * alpha);
				t[3 * j + 1] = static_cast<uchar>(g + (g - L * 255) * alpha);
				t[3 * j] = static_cast<uchar>(b + (b - L * 255) * alpha);
			}
			else
			{
				alpha = Increment;
				t[3 * j + 2] = static_cast<uchar>(L * 255 + (r - L * 255) * (1 + alpha));
				t[3 * j + 1] = static_cast<uchar>(L * 255 + (g - L * 255) * (1 + alpha));
				t[3 * j] = static_cast<uchar>(L * 255 + (b - L * 255) * (1 + alpha));
			}
			counter++;
			if (counter % num_step == 0)
				progress.StepIt();
		}
	}
	progress.SetPos(100);
	return temp;
}



//--------------------------------------------------------------------------------
// ůɫ��
cv::Mat ColorTemperature(cv::Mat src, int warm, CProgressCtrl& progress)
{
	cv::Mat result = src.clone();
	int row = src.rows;
	int col = src.cols;
	int level = warm / 2;

	int num_pixel = row * col;
	int num_step = num_pixel / 100;
	int counter = 0;

	for (int i = 0; i < row; ++i)
	{
		uchar* a = src.ptr<uchar>(i);
		uchar* r = result.ptr<uchar>(i);
		for (int j = 0; j < col; ++j)
		{
			int R, G, B;
			// Rͨ��
			R = a[j * 3 + 2];
			R = R + level;
			if (R > 255)
			{
				r[j * 3 + 2] = 255;
			}
			else if (R < 0)
			{
				r[j * 3 + 2] = 0;
			}
			else
			{
				r[j * 3 + 2] = R;
			}
			// Gͨ��
			G = a[j * 3 + 1];
			G = G + level;
			if (G > 255)
			{
				r[j * 3 + 1] = 255;
			}
			else if (G < 0)
			{
				r[j * 3 + 1] = 0;
			}
			else
			{
				r[j * 3 + 1] = G;
			}
			// Bͨ��
			B = a[j * 3];
			B = B - level;
			if (B > 255)
			{
				r[j * 3] = 255;
			}
			else if (B < 0)
			{
				r[j * 3] = 0;
			}
			else {
				r[j * 3] = B;
			}
			counter++;
			if (counter % num_step == 0)
				progress.StepIt();
		}
	}
	progress.SetPos(100);
	return result;
}


//--------------------------------------------------------------------------------
// ��
cv::Mat Sharpen(cv::Mat input, int percent, int type)
{
	cv::Mat result;
	cv::Mat s = input.clone();
	cv::Mat kernel;
	switch (type)
	{
	case 0:
		kernel = (cv::Mat_<int>(3, 3) <<
			0, -1, 0,
			-1, 4, -1,
			0, -1, 0
			);
	case 1:
		kernel = (cv::Mat_<int>(3, 3) <<
			-1, -1, -1,
			-1, 8, -1,
			-1, -1, -1
			);
	default:
		kernel = (cv::Mat_<int>(3, 3) <<
			0, -1, 0,
			-1, 4, -1,
			0, -1, 0
			);
	}
	cv::filter2D(s, s, s.depth(), kernel);
	result = input + s * 0.01 * percent;
	return result;
}
