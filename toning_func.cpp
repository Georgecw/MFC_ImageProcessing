#include "pch.h"
#include "toning_func.h"

//调整对比度与亮度
cv::Mat Brightness(cv::Mat src, float brightness, int contrast)
{
	cv::Mat dst;
	dst = cv::Mat::zeros(src.size(), src.type());		//新建空白模板：大小/类型与原图像一致，像素值全0。
	int height = src.rows;								//获取图像高度
	int width = src.cols;								//获取图像宽度
	float alpha = brightness;							//亮度（0~1为暗，1~正无穷为亮）
	float beta = contrast;								//对比度

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
		}
	}
	return dst;
}

//--------------------------------------------------------------------------------
// 饱和度
cv::Mat Saturation(cv::Mat src, int saturation)
{
	float Increment = saturation * 1.0f / 100;
	cv::Mat temp = src.clone();
	int row = src.rows;
	int col = src.cols;
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
		}
	}
	return temp;
}

//--------------------------------------------------------------------------------
// 高光
cv::Mat HighLight(cv::Mat src, int highlight)
{
	// 生成灰度图
	cv::Mat gray = cv::Mat::zeros(src.size(), CV_32FC1);
	cv::Mat f = src.clone();
	f.convertTo(f, CV_32FC3);
	std::vector<cv::Mat> pics;
	split(f, pics);
	gray = 0.299f * pics[2] + 0.587 * pics[2] + 0.114 * pics[0];
	gray = gray / 255.f;

	// 确定高光区
	cv::Mat thresh = cv::Mat::zeros(gray.size(), gray.type());
	thresh = gray.mul(gray);
	// 取平均值作为阈值
	cv::Scalar t = mean(thresh);
	cv::Mat mask = cv::Mat::zeros(gray.size(), CV_8UC1);
	mask.setTo(255, thresh >= t[0]);

	// 参数设置
	int max = 4;
	float bright = highlight / 100.0f / max;
	float mid = 1.0f + max * bright;

	// 边缘平滑过渡
	cv::Mat midrate = cv::Mat::zeros(src.size(), CV_32FC1);
	cv::Mat brightrate = cv::Mat::zeros(src.size(), CV_32FC1);
	for (int i = 0; i < src.rows; ++i)
	{
		uchar* m = mask.ptr<uchar>(i);
		float* th = thresh.ptr<float>(i);
		float* mi = midrate.ptr<float>(i);
		float* br = brightrate.ptr<float>(i);
		for (int j = 0; j < src.cols; ++j)
		{
			if (m[j] == 255)
			{
				mi[j] = mid;
				br[j] = bright;
			}
			else {
				mi[j] = (mid - 1.0f) / t[0] * th[j] + 1.0f;
				br[j] = (1.0f / t[0] * th[j]) * bright;
			}
		}
	}

	// 高光提亮，获取结果图
	cv::Mat result = cv::Mat::zeros(src.size(), src.type());
	for (int i = 0; i < src.rows; ++i)
	{
		float* mi = midrate.ptr<float>(i);
		float* br = brightrate.ptr<float>(i);
		uchar* in = src.ptr<uchar>(i);
		uchar* r = result.ptr<uchar>(i);
		for (int j = 0; j < src.cols; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				float temp = pow(float(in[3 * j + k]) / 255.f, 1.0f / mi[j]) * (1.0 / (1 - br[j]));
				if (temp > 1.0f)
					temp = 1.0f;
				if (temp < 0.0f)
					temp = 0.0f;
				uchar utemp = uchar(255 * temp);
				r[3 * j + k] = utemp;
			}

		}
	}
	return result;
}

//--------------------------------------------------------------------------------
// 暖色调
cv::Mat ColorTemperature(cv::Mat src, int warm)
{
	cv::Mat result = src.clone();
	int row = src.rows;
	int col = src.cols;
	int level = warm / 2;
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
		}
	}
	return result;
}

//--------------------------------------------------------------------------------
// 阴影
cv::Mat Shadow(cv::Mat src, int shadow)
{
	// 生成灰度图
	cv::Mat gray = cv::Mat::zeros(src.size(), CV_32FC1);
	cv::Mat f = src.clone();
	f.convertTo(f, CV_32FC3);
	std::vector<cv::Mat> pics;
	split(f, pics);
	gray = 0.299f * pics[2] + 0.587 * pics[2] + 0.114 * pics[0];
	gray = gray / 255.f;

	// 确定阴影区
	cv::Mat thresh = cv::Mat::zeros(gray.size(), gray.type());
	thresh = (1.0f - gray).mul(1.0f - gray);
	// 取平均值作为阈值
	cv::Scalar t = mean(thresh);
	cv::Mat mask = cv::Mat::zeros(gray.size(), CV_8UC1);
	mask.setTo(255, thresh >= t[0]);

	// 参数设置
	int max = 4;
	float bright = shadow / 100.0f / max;
	float mid = 1.0f + max * bright;

	// 边缘平滑过渡
	cv::Mat midrate = cv::Mat::zeros(src.size(), CV_32FC1);
	cv::Mat brightrate = cv::Mat::zeros(src.size(), CV_32FC1);
	for (int i = 0; i < src.rows; ++i)
	{
		uchar* m = mask.ptr<uchar>(i);
		float* th = thresh.ptr<float>(i);
		float* mi = midrate.ptr<float>(i);
		float* br = brightrate.ptr<float>(i);
		for (int j = 0; j < src.cols; ++j)
		{
			if (m[j] == 255)
			{
				mi[j] = mid;
				br[j] = bright;
			}
			else
			{
				mi[j] = (mid - 1.0f) / t[0] * th[j] + 1.0f;
				br[j] = (1.0f / t[0] * th[j]) * bright;
			}
		}
	}

	// 阴影提亮，获取结果图
	cv::Mat result = cv::Mat::zeros(src.size(), src.type());
	for (int i = 0; i < src.rows; ++i)
	{
		float* mi = midrate.ptr<float>(i);
		float* br = brightrate.ptr<float>(i);
		uchar* in = src.ptr<uchar>(i);
		uchar* r = result.ptr<uchar>(i);
		for (int j = 0; j < src.cols; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				float temp = pow(float(in[3 * j + k]) / 255.f, 1.0f / mi[j]) * (1.0 / (1 - br[j]));
				if (temp > 1.0f)
					temp = 1.0f;
				if (temp < 0.0f)
					temp = 0.0f;
				uchar utemp = uchar(255 * temp);
				r[3 * j + k] = utemp;
			}

		}
	}
	return result;
}

//--------------------------------------------------------------------------------
// 漫画效果
cv::Mat Cartoon(cv::Mat src, double clevel, int d, double sigma, int size)
{
	//（1）中值滤波
	cv::Mat m;
	cv::medianBlur(src, m, 7);
	//（2）提取轮廓
	cv::Mat c;
	clevel = cv::max(40., cv::min(80., clevel));
	cv::Canny(m, c, clevel, clevel * 3);
	//（3）轮廓膨胀
	cv::Mat k = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
	cv::dilate(c, c, k);
	//（4）图像反转
	c = c / 255;
	c = 1 - c;
	//（5）均值滤波
	cv::Mat cf;
	c.convertTo(cf, CV_32FC1);				// 类型转换		
	cv::blur(cf, cf, cv::Size(5, 5));
	//（6）双边滤波
	cv::Mat srcb;
	d = cv::max(0, cv::min(10, d));
	sigma = cv::max(10., cv::min(250., sigma));
	cv::bilateralFilter(src, srcb, d, sigma, sigma);
	size = cv::max(10, cv::min(25, size));
	cv::Mat temp = srcb / size;
	temp = temp * size;
	//（7）通道合并
	cv::Mat c3;
	cv::Mat cannyChannels[] = { cf, cf, cf };
	cv::merge(cannyChannels, 3, c3);
	//（8）图像相乘
	cv::Mat tempf;
	temp.convertTo(tempf, CV_32FC3);		// 类型转换
	cv::multiply(tempf, c3, tempf);
	tempf.convertTo(temp, CV_8UC3);			// 类型转换
	return temp;
}

//--------------------------------------------------------------------------------
// 白平衡-灰度世界
cv::Mat WhiteBalcane_Gray(cv::Mat src)
{
	//（1）3通道处理
	cv::Mat result = src.clone();
	if (src.channels() != 3)
	{
		std::cout << "The number of image channels is not 3." << std::endl;
		return result;
	}
	//（2）通道分离
	std::vector<cv::Mat> Channel;
	cv::split(src, Channel);
	//（3）计算通道灰度值均值
	double Bm = cv::mean(Channel[0])[0];
	double Gm = cv::mean(Channel[1])[0];
	double Rm = cv::mean(Channel[2])[0];
	double Km = (Bm + Gm + Rm) / 3;
	//（4）通道灰度值调整
	Channel[0] *= Km / Bm;
	Channel[1] *= Km / Gm;
	Channel[2] *= Km / Rm;
	//（5）通道合并
	cv::merge(Channel, result);
	return result;
}

//--------------------------------------------------------------------------------
// 白平衡-完美反射
cv::Mat WhiteBalcane_PRA(cv::Mat src)
{
	//（1）3通道处理
	cv::Mat result = src.clone();
	if (src.channels() != 3)
	{
		std::cout << "The number of image channels is not 3." << std::endl;
		return result;
	}
	//（2）通道分离
	std::vector<cv::Mat> Channel;
	cv::split(src, Channel);
	//（3）计算单通道最大值
	int row = src.rows;
	int col = src.cols;
	int RGBSum[766] = { 0 };
	uchar maxR, maxG, maxB;
	for (int i = 0; i < row; ++i)
	{
		uchar* b = Channel[0].ptr<uchar>(i);
		uchar* g = Channel[1].ptr<uchar>(i);
		uchar* r = Channel[2].ptr<uchar>(i);
		for (int j = 0; j < col; ++j)
		{
			int sum = b[j] + g[j] + r[j];
			RGBSum[sum]++;
			maxB = cv::max(maxB, b[j]);
			maxG = cv::max(maxG, g[j]);
			maxR = cv::max(maxR, r[j]);
		}
	}
	//（4）计算最亮区间下限T
	int T = 0;
	int num = 0;
	int K = static_cast<int>(row * col * 0.1);
	for (int i = 765; i >= 0; --i)
	{
		num += RGBSum[i];
		if (num > K)
		{
			T = i;
			break;
		}
	}
	//（5）计算单通道亮区平均值
	double Bm = 0.0, Gm = 0.0, Rm = 0.0;
	int count = 0;
	for (int i = 0; i < row; ++i)
	{
		uchar* b = Channel[0].ptr<uchar>(i);
		uchar* g = Channel[1].ptr<uchar>(i);
		uchar* r = Channel[2].ptr<uchar>(i);
		for (int j = 0; j < col; ++j)
		{
			int sum = b[j] + g[j] + r[j];
			if (sum > T)
			{
				Bm += b[j];
				Gm += g[j];
				Rm += r[j];
				count++;
			}
		}
	}
	Bm /= count;
	Gm /= count;
	Rm /= count;
	//（6）通道调整
	Channel[0] *= maxB / Bm;
	Channel[1] *= maxG / Gm;
	Channel[2] *= maxR / Rm;
	//（7）通道合并
	cv::merge(Channel, result);
	return result;
}

//--------------------------------------------------------------------------------
// 浮雕
cv::Mat Relief(cv::Mat src)
{
	CV_Assert(src.channels() == 3);
	int row = src.rows;
	int col = src.cols;
	cv::Mat temp = src.clone();
	for (int i = 1; i < row - 1; ++i)
	{
		uchar* s1 = src.ptr<uchar>(i - 1);
		uchar* s2 = src.ptr<uchar>(i + 1);
		uchar* t = temp.ptr<uchar>(i);
		for (int j = 1; j < col - 1; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				int RGB = s1[3 * (j - 1) + k] - s2[3 * (j + 1) + k] + 128;
				if (RGB < 0)RGB = 0;
				if (RGB > 255)RGB = 255;
				t[3 * j + k] = (uchar)RGB;
			}
		}
	}
	return temp;
}

//--------------------------------------------------------------------------------
// 羽化
cv::Mat Eclosion(cv::Mat src, cv::Point center, float level)
{
	if (level > 0.9)
		level = 0.9f;
	float diff = (1 - level) * (src.rows / 2 * src.rows / 2 + src.cols / 2 * src.cols / 2);
	cv::Mat result = src.clone();
	for (int i = 0; i < result.rows; ++i)
	{
		for (int j = 0; j < result.cols; ++j)
		{
			float dx = float(center.x - j);
			float dy = float(center.y - i);
			float ra = dx * dx + dy * dy;
			float m = ((ra - diff) / diff * 255) > 0 ? ((ra - diff) / diff * 255) : 0;
			int b = result.at<cv::Vec3b>(i, j)[0];
			int g = result.at<cv::Vec3b>(i, j)[1];
			int r = result.at<cv::Vec3b>(i, j)[2];
			b = (int)(b + m);
			g = (int)(g + m);
			r = (int)(r + m);
			result.at<cv::Vec3b>(i, j)[0] = (b > 255 ? 255 : (b < 0 ? 0 : b));
			result.at<cv::Vec3b>(i, j)[1] = (g > 255 ? 255 : (g < 0 ? 0 : g));
			result.at<cv::Vec3b>(i, j)[2] = (r > 255 ? 255 : (r < 0 ? 0 : r));
		}
	}
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

//--------------------------------------------------------------------------------
// 颗粒感
cv::Mat Grainy(cv::Mat src, int level)
{
	int row = src.rows;
	int col = src.cols;
	if (level > 100)
		level = 100;
	if (level < 0)
		level = 0;
	cv::Mat result = src.clone();
	for (int i = 0; i < row; ++i)
	{
		uchar* t = result.ptr<uchar>(i);
		for (int j = 0; j < col; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				int temp = t[3 * j + k];
				temp += ((rand() % (2 * level)) - level);
				if (temp < 0)temp = 0;
				if (temp > 255)temp = 255;
				t[3 * j + k] = temp;
			}
		}
	}
	return result;
}
