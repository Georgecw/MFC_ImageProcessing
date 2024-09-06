#include "pch.h"
#include "toning_func.h"

//调整对比度与亮度
cv::Mat Brightness(cv::Mat src, float brightness, int contrast, CProgressCtrl& progress)
{
	cv::Mat dst;
	dst = cv::Mat::zeros(src.size(), src.type());		//新建空白模板：大小/类型与原图像一致，像素值全0。
	int height = src.rows;								//获取图像高度
	int width = src.cols;								//获取图像宽度
	float alpha = brightness;							//亮度（0~1为暗，1~正无穷为亮）
	float beta = contrast;								//对比度

	int num_pixel = height * width;
	int num_step = num_pixel / 100;
	int counter = 0;

	cv::Mat template1;
	src.convertTo(template1, CV_32F);					//将CV_8UC1转换为CV32F1数据格式。
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			if (src.channels() == 3)
			{
				float b = template1.at<cv::Vec3f>(row, col)[0];		//获取通道的像素值（blue）
				float g = template1.at<cv::Vec3f>(row, col)[1];		//获取通道的像素值（green）
				float r = template1.at<cv::Vec3f>(row, col)[2];		//获取通道的像素值（red）

				//cv::saturate_cast<uchar>(vaule)：需注意，value值范围必须在0~255之间。
				dst.at<cv::Vec3b>(row, col)[0] = cv::saturate_cast<uchar>(b * alpha + beta);		//修改通道的像素值（blue）
				dst.at<cv::Vec3b>(row, col)[1] = cv::saturate_cast<uchar>(g * alpha + beta);		//修改通道的像素值（green）
				dst.at<cv::Vec3b>(row, col)[2] = cv::saturate_cast<uchar>(r * alpha + beta);		//修改通道的像素值（red）
			}
			else if (src.channels() == 1)
			{
				float v = src.at<uchar>(row, col);											//获取通道的像素值（单）
				dst.at<uchar>(row, col) = cv::saturate_cast<uchar>(v * alpha + beta);		//修改通道的像素值（单）
				//saturate_cast<uchar>：主要是为了防止颜色溢出操作。如果color<0，则color等于0；如果color>255，则color等于255。
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
// 饱和度
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
// 暖色调
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
			// R通道
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
			// G通道
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
			// B通道
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
// 锐化
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
