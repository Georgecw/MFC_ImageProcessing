
// MFC_ImageProcessingDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFC_ImageProcessing.h"
#include "MFC_ImageProcessingDlg.h"
#include "afxdialogex.h"
#include "CTextInputDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCImageProcessingDlg 对话框



CMFCImageProcessingDlg::CMFCImageProcessingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_IMAGEPROCESSING_DIALOG, pParent)
	, m_Scale_Height(0)
	, m_Scale_Width(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCImageProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HEIGHT_EDIT, m_Scale_Height);
	DDX_Text(pDX, IDC_WIDTH_EDIT, m_Scale_Width);
}

BEGIN_MESSAGE_MAP(CMFCImageProcessingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_BUTTON, &CMFCImageProcessingDlg::OnClickedOpenButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &CMFCImageProcessingDlg::OnClickedSaveButton)
	ON_BN_CLICKED(IDC_Rotation, &CMFCImageProcessingDlg::OnClickedRotation)
	ON_BN_CLICKED(IDOK, &CMFCImageProcessingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BLUR_BUTTON, &CMFCImageProcessingDlg::OnClickedBlurButton)
	ON_BN_CLICKED(IDC_SHARP_BUTTON, &CMFCImageProcessingDlg::OnClickedSharpButton)
	ON_BN_CLICKED(IDC_SCALE_BUTTON, &CMFCImageProcessingDlg::OnClickedScaleButton)
	ON_BN_CLICKED(IDC_BUTTON_TEXT, &CMFCImageProcessingDlg::OnBnClickedTextButton)
	ON_WM_LBUTTONDOWN()
	
END_MESSAGE_MAP()


// CMFCImageProcessingDlg 消息处理程序

BOOL CMFCImageProcessingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCImageProcessingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCImageProcessingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);

		
	}
	else
	{
		
		
		
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCImageProcessingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 内部函数

// 显示位图函数
void CMFCImageProcessingDlg::Show_Bmp(double hfactor = 1,double wfactor = 1)
{
	// 清除图片控件
	GetDlgItem(IDC_STATIC_PIC)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_PIC)->ShowWindow(TRUE);

	// 获取图片控件基本变量
	CWnd* pWnd = GetDlgItem(IDC_STATIC_PIC); //获得pictrue控件窗口的句柄
	CDC* pDC = pWnd->GetDC(); //获得pictrue控件的DC
	CRect rect;
	pWnd->GetClientRect(&rect); //获得pictrue控件所在的矩形区域

	// 计算宽高比，使用浮点数避免精度问题
	double wid_hei_ratio = (double)bmpInfo.biWidth / (double)bmpInfo.biHeight;
	int image_width = bmpInfo.biWidth;
	int image_height = bmpInfo.biHeight;

	// 调整缩略图大小
	if (rect.Width() < wid_hei_ratio * rect.Height())
	{
		if (wfactor <= 1 && hfactor <= 1) 
		{
			image_width = static_cast<int>(rect.Width() * wfactor);
			image_height = static_cast<int>(image_width / wid_hei_ratio * hfactor);
		}
		else 
		{
			AfxMessageBox(_T("倍数大于一将无法在缩略图中呈现缩放效果"));
			image_width = rect.Width();
			image_height = static_cast<int>(image_width / wid_hei_ratio);
		}
		
	}
	else
	{
		if (wfactor <= 1 && hfactor <= 1) 
		{
			image_height = static_cast<int>(rect.Height() * hfactor);
			image_width = static_cast<int>(wid_hei_ratio * image_height * wfactor);
		}
		else 
		{
			AfxMessageBox(_T("倍数大于一将无法在缩略图中呈现缩放效果"));
			image_height = rect.Height();
			image_width = static_cast<int>(wid_hei_ratio * image_height);
		}
	}

	// 确保图像宽高不为负数
	if (image_width <= 0 || image_height <= 0)
	{
		AfxMessageBox(_T("宽高为负数！"));
		return;
	}

	// 计算每行的填充字节数
	int rowSize = ((bmpInfo.biWidth * bmpInfo.biBitCount + 31) / 32) * 4;
	int bmpDataSize = bmpInfo.biHeight * rowSize;

	// 显示位图
	pDC->SetStretchBltMode(COLORONCOLOR);
	StretchDIBits(pDC->GetSafeHdc(),
		(rect.Width() - image_width) / 2,
		(rect.Height() - image_height) / 2,
		image_width,
		image_height,
		0,
		0,
		bmpInfo.biWidth,
		bmpInfo.biHeight,
		pBmpData,
		pBmpInfo,
		DIB_RGB_COLORS,
		SRCCOPY);

	pWnd->ReleaseDC(pDC);
}

// 保存并打开临时位图
void CMFCImageProcessingDlg::Save_Open_Temp_Bmp()
{
	// 保存临时位图
	CString TempPath = _T(".\\Temp");
	if (!PathIsDirectory(TempPath)) CreateDirectory(TempPath, 0); //不存在临时文件夹则创建

	CString TempFilePath = _T(".\\Temp\\Temp.bmp");
	if (!bmpFile.Open(TempFilePath, CFile::modeCreate |
		CFile::modeWrite | CFile::typeBinary))
	{
		AfxMessageBox(_T("无法打开临时文件进行写入"));
		return;
	}

	bmpFile.Write(&bmpHeader, sizeof(BITMAPFILEHEADER));
	bmpFile.Write(&bmpInfo, sizeof(BITMAPINFOHEADER));
	DWORD bmpDataSize = bmpInfo.biSizeImage;
	if (bmpDataSize == 0)
	{
		bmpDataSize = ((bmpInfo.biWidth * bmpInfo.biBitCount
			+ 31) / 32) * 4 * bmpInfo.biHeight;
	}
	bmpFile.Write(pBmpData, bmpDataSize);
	bmpFile.Close();

	// 打开临时位图
	if (!bmpFile.Open(TempFilePath, CFile::modeRead | CFile::typeBinary))
		return;
	if (bmpFile.Read(&bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
		return;
	if (bmpFile.Read(&bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
		return;
	pBmpInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)];

	memcpy(pBmpInfo, &bmpInfo, sizeof(BITMAPINFOHEADER));
	DWORD Bytes = bmpInfo.biWidth * bmpInfo.biHeight * (bmpInfo.biBitCount / 8);
	pBmpData = new BYTE[Bytes];
	bmpFile.Read(pBmpData, Bytes);
	bmpFile.Close();
}

// 将 BGR 编码的位图转换成 OpenCV Mat 格式
void CMFCImageProcessingDlg::Bmp2Mat(cv::Mat img, int height, int width)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = (y * width + x) * 3; // 每个像素 3 byte
			img.at<cv::Vec3b>(y, x)[0] = pBmpData[index + 2]; // B
			img.at<cv::Vec3b>(y, x)[1] = pBmpData[index + 1]; // G
			img.at<cv::Vec3b>(y, x)[2] = pBmpData[index];     // R
		}
	}
}

// 将 OpenCV Mat 格式还原成 BGR 编码的位图文件
void CMFCImageProcessingDlg::Mat2Bmp(cv::Mat img, int height, int width)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = (y * width + x) * 3; // 每个像素 3 byte
			pBmpData[index + 2] = img.at<cv::Vec3b>(y, x)[0]; // B
			pBmpData[index + 1] = img.at<cv::Vec3b>(y, x)[1]; // G
			pBmpData[index] = img.at<cv::Vec3b>(y, x)[2];     // R
		}
	}
}


#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0;
	UINT size = 0;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0) return -1;

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == nullptr) return -1;

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}

	free(pImageCodecInfo);
	return -1;
}

void ConvertToBMP(const CString& inputPath, const CString& outputPath)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	Gdiplus::Image image(inputPath);
	CLSID bmpClsid;
	GetEncoderClsid(L"image/bmp", &bmpClsid);
	image.Save(outputPath, &bmpClsid, nullptr);

	//Gdiplus::GdiplusShutdown(gdiplusToken);
}

// 控件函数
void CMFCImageProcessingDlg::OnClickedOpenButton()
{
	// 设置过滤器
	LPCTSTR szFilter = _T("Image Files (*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|");

	// 构造打开文件对话框
	CFileDialog fileDlg(TRUE, _T(".bmp"), NULL,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter, this);
	


	// 显示打开文件对话框
	if (IDOK == fileDlg.DoModal())
	{
		FilePath = fileDlg.GetPathName(); // 获取文件路径
		SetDlgItemText(IDC_OPEN_EDIT, FilePath); // 在编辑框显示文件路径

		CString BmpName = fileDlg.GetPathName();
		CString EntName = fileDlg.GetFileExt();
		EntName.MakeLower();

		if (EntName.Compare(_T("bmp")) == 0)
		{
			// 读取并显示 BMP 文件
			if (!bmpFile.Open(BmpName, CFile::modeRead | CFile::typeBinary))
				return;
			if (bmpFile.Read(&bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
				return;
			if (bmpFile.Read(&bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
				return;
			pBmpInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)];

			memcpy(pBmpInfo, &bmpInfo, sizeof(BITMAPINFOHEADER));
			DWORD Bytes = bmpInfo.biWidth * bmpInfo.biHeight * (bmpInfo.biBitCount / 8);
			pBmpData = new BYTE[Bytes];
			bmpFile.Read(pBmpData, Bytes);
			bmpFile.Close();

			Show_Bmp();
			m_is_open = true;
		}
		else if (EntName.Compare(_T("jpg")) == 0 || EntName.Compare(_T("jpeg")) == 0 || EntName.Compare(_T("png")) == 0)
		{
			// 将 JPG/PNG 文件转换为 BMP 并读取
			CString tempBmpPath = FilePath.Left(FilePath.ReverseFind('.')) + _T(".bmp");
			ConvertToBMP(FilePath, tempBmpPath);

			if (!bmpFile.Open(tempBmpPath, CFile::modeRead | CFile::typeBinary))
				return;
			if (bmpFile.Read(&bmpHeader, sizeof(BITMAPFILEHEADER)) != sizeof(BITMAPFILEHEADER))
				return;
			if (bmpFile.Read(&bmpInfo, sizeof(BITMAPINFOHEADER)) != sizeof(BITMAPINFOHEADER))
				return;
			pBmpInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)];

			memcpy(pBmpInfo, &bmpInfo, sizeof(BITMAPINFOHEADER));
			DWORD Bytes = bmpInfo.biWidth * bmpInfo.biHeight * (bmpInfo.biBitCount / 8);
			pBmpData = new BYTE[Bytes];
			bmpFile.Read(pBmpData, Bytes);
			bmpFile.Close();

			Show_Bmp();
			m_is_open = true;
		}
	}
}





void CMFCImageProcessingDlg::CreateTextImage(CString& text, CRect& textRect, const CString& textImagePath)
{
	// 创建内存 DC
	CDC memDC;
	memDC.CreateCompatibleDC(nullptr);

	// 创建透明背景的位图
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&memDC, bmpInfo.biWidth, bmpInfo.biHeight);
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// 创建一个透明背景的画刷
	CBrush brush(RGB(255, 255, 255)); // 白色背景
	memDC.FillRect(CRect(0, 0, bmpInfo.biWidth, bmpInfo.biHeight), &brush);

	// 创建字体
	CFont font;
	font.CreatePointFont(m_font_size, _T("Arial"));
	CFont* pOldFont = memDC.SelectObject(&font);

	// 设置文本颜色
	memDC.SetTextColor(m_font_col);
	memDC.SetBkMode(TRANSPARENT); // 透明背景

	// 绘制文本
	memDC.DrawText(text, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 恢复原来的对象
	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldBitmap);

	// 保存文本图像
	CImage image;
	image.Create(bmpInfo.biWidth, bmpInfo.biHeight, 24); // 24 位颜色深度
	HDC hdc = image.GetDC();
	memDC.BitBlt(0, 0, bmpInfo.biWidth, bmpInfo.biHeight, &memDC, 0, 0, SRCCOPY);
	image.ReleaseDC();
	image.Save(textImagePath); // 保存为 BMP 文件
}


#include <opencv2/opencv.hpp>
#include<cstring>


void OverlayTextOnImage(std::string& imagePath, const std::string& textImagePath, const std::string& outputPath)
{
	// 读取原始图像
	cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
	if (image.empty())
	{
		std::cerr << "无法读取原始图像" << std::endl;
		return;
	}

	// 读取文本图像
	cv::Mat textImage = cv::imread(textImagePath, cv::IMREAD_UNCHANGED);
	if (textImage.empty())
	{
		std::cerr << "无法读取文本图像" << std::endl;
		return;
	}

	// 确保文本图像有 alpha 通道
	if (textImage.channels() == 3)
	{
		cv::cvtColor(textImage, textImage, cv::COLOR_BGR2BGRA);
	}

	// 叠加图像
	cv::Mat result;
	cv::addWeighted(image, 1.0, textImage, 1.0, 0.0, result);

	// 保存叠加后的图像
	cv::imwrite(outputPath, result);
}




void CMFCImageProcessingDlg::OnClickedSaveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	// 设置过滤器
	LPCTSTR szFilter = _T("bmp (*.bmp)|*.bmp|");

	// 构造保存文件对话框    
	CFileDialog fileDlg(FALSE, _T(".bmp"), _T("default.bmp"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	fileDlg.m_ofn.lpstrTitle = _T("保存");  // 保存对话窗口标题名
	CString FilePath;

	if (m_is_open == false) {
		AfxMessageBox(_T("没有打开文件，无法保存"));
		return;
	}

	// 显示保存文件对话框
	if (IDOK == fileDlg.DoModal())  //判断是否点击了确定
	{
		FilePath = fileDlg.GetPathName();  // 获取文件路径
		SetDlgItemText(IDC_SAVE_EDIT, FilePath);

		if (!bmpFile.Open(FilePath, CFile::modeCreate | 
			CFile::modeWrite | CFile::typeBinary)) 
		{
			AfxMessageBox(_T("无法打开文件进行写入"));
			return;
		}

		CString textImagePath = _T("textImage.bmp");
		CreateTextImage(m_text, m_textRect, textImagePath);
		std::string ori_pic_path = CT2A(FilePath.GetString());
		
		// 使用 OpenCV 叠加文本到原图上
		OverlayTextOnImage(ori_pic_path, "textImage.bmp", "finalImage.bmp");
		/*
		// 写入文件头
		bmpFile.Write(&bmpHeader, sizeof(BITMAPFILEHEADER));

		// 写入信息头
		bmpFile.Write(&bmpInfo, sizeof(BITMAPINFOHEADER));

		// 计算位图数据大小
		DWORD bmpDataSize = bmpInfo.biSizeImage;
		if (bmpDataSize == 0) 
		{
			bmpDataSize = ((bmpInfo.biWidth * bmpInfo.biBitCount 
				+ 31) / 32) * 4 * bmpInfo.biHeight;
		}

		// 写入位图数据
		bmpFile.Write(pBmpData, bmpDataSize);

		// 关闭文件
		bmpFile.Close();
		*/
	}
}

void CMFCImageProcessingDlg::OnClickedRotation()
{
	// TODO: 在此添加控件通知处理程序代码
	// 获取原始位图的宽度和高度
	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;

	// 计算新的宽度和高度
	int newWidth = height;
	int newHeight = width;

	// 分配新的位图数据数组
	BYTE* pNewBmpData = new BYTE[newWidth * newHeight * (bmpInfo.biBitCount / 8)];
	memset(pNewBmpData, 0, newWidth * newHeight * (bmpInfo.biBitCount / 8)); // 初始化为 0

	// 进行旋转变换
	for (int y = 0; y < height; y++) 
	{
		for (int x = 0; x < width; x++) 
		{
			// 计算新坐标
			int newX = y;
			int newY = width-x-1;

			// 计算原始和新的数据偏移
			int oldIndex = static_cast<int>((y * width + x) * (bmpInfo.biBitCount / 8));
			int newIndex = static_cast<int>((newY * newWidth + newX) * (bmpInfo.biBitCount / 8));
			memcpy(&pNewBmpData[newIndex], &pBmpData[oldIndex], (bmpInfo.biBitCount / 8)); // 复制像素
		}
	}

	// 更新位图信息
	bmpInfo.biWidth = newWidth;
	bmpInfo.biHeight = newHeight;
	bmpInfo.biSizeImage = newWidth * newHeight * (bmpInfo.biBitCount / 8);
	delete[] pBmpData;
	pBmpData = pNewBmpData;

	// 保存并打开临时位图
	Save_Open_Temp_Bmp();

	// 显示位图
	Show_Bmp();
}

void CMFCImageProcessingDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CMFCImageProcessingDlg::OnClickedBlurButton()
{
	// TODO: 在此添加控件通知处理程序代码
	 // 1. 将位图数据转换为 OpenCV Mat
	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	cv::Mat img(height, width, CV_8UC3);

	// 假设位图数据是 BGR 格式
	Bmp2Mat(img, height, width);

	// 2. 应用高斯模糊
	cv::Mat blurredImg;
	cv::GaussianBlur(img, blurredImg, cv::Size(19, 19), 0);

	// 3. 将处理后的图像转换回位图格式
	Mat2Bmp(blurredImg, height, width);

	// 4. 显示图像
	Show_Bmp();
}


void CMFCImageProcessingDlg::OnClickedSharpButton()
{
	// TODO: 在此添加控件通知处理程序代码
	// 1. 将位图数据转换为 OpenCV Mat
	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	cv::Mat img(height, width, CV_8UC3);

	// 假设位图数据是 BGR 格式
	Bmp2Mat(img, height, width);

	// 2. 应用高斯滤波锐化模糊图片
	cv::Mat SharpenedImg;
	cv::GaussianBlur(img, SharpenedImg, cv::Size(0, 0), 3);
	cv::addWeighted(img, 2, SharpenedImg, -1, 0, SharpenedImg);
	
	// 3. 将处理后的图像转换回位图格式
	Mat2Bmp(SharpenedImg, height, width);

	// 4. 显示图像
	Show_Bmp();
}


void CMFCImageProcessingDlg::OnClickedScaleButton()
{
	// TODO: 在此添加控件通知处理程序代码
	// 1. 将位图数据转换为 OpenCV Mat
	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	cv::Mat img(height, width, CV_8UC3);

	// 假设位图数据是 BGR 格式
	Bmp2Mat(img, height, width);

	// 2. 用 opencv resize 函数放缩图片
	UpdateData(TRUE);
	if (m_Scale_Height == 0 || m_Scale_Width==0)
	{
		AfxMessageBox(_T("倍数不能为零！"));
		return;
	}
	cv::Mat ResizedImg;
	cv::resize(img, ResizedImg, cv::Size(0,0),m_Scale_Width,m_Scale_Height);
	UpdateData(FALSE);

	// 更新位图信息
	int NewWidth = static_cast<int>(width * m_Scale_Width);
	int NewHeight = static_cast<int>(height * m_Scale_Height);
	bmpInfo.biWidth = NewWidth;
	bmpInfo.biHeight = NewHeight;
	bmpInfo.biSizeImage = NewWidth * NewHeight * (bmpInfo.biBitCount / 8);
	pBmpData = new BYTE[NewWidth * NewHeight * (bmpInfo.biBitCount / 8)];

	// 3. 将处理后的图像转换回位图格式
	Mat2Bmp(ResizedImg, NewHeight, NewWidth);

	Save_Open_Temp_Bmp();

	// 4. 显示图像
	Show_Bmp(m_Scale_Height, m_Scale_Width);
}


void CMFCImageProcessingDlg::OnBnClickedTextButton()
{
	m_is_text = true;
	
	// TODO: 在此添加控件通知处理程序代码
}




void CMFCImageProcessingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_is_text == true) {
		CTextInputDialog dlg;
		if (dlg.DoModal() == IDOK)  // 显示对话框并等待用户输入
		{
			m_text = dlg.GetInputText();  // 获取用户输入的文本
			m_font_size = dlg.GetFontSize(); //获取字体大小
			m_font_col = dlg.GetFontCol();  //获取字体颜色

			// 获取图片控件的指针
			CWnd* pWnd = GetDlgItem(IDC_STATIC_PIC);

			// 获取图片控件在屏幕上的矩形区域
			CRect picRect;
			pWnd->GetWindowRect(&picRect);

			// 将鼠标点击点从主窗口坐标转换为屏幕坐标
			ClientToScreen(&point);

			// 检查点击点是否在图片控件内部
			if (picRect.PtInRect(point)) {
				// 用户定义文本框的位置，可以改为动态捕获鼠标位置
				// 
				// 将屏幕坐标转换为图片控件的客户区坐标
				pWnd->ScreenToClient(&point);

				// 创建字体
				m_font.DeleteObject();
				m_font.CreatePointFont(m_font_size, _T("Arial"));

				CDC* pDC = GetDC();  // 获取设备上下文

				// 获取字符'A'的宽度和高度
				CSize size = pDC->GetTextExtent(m_text);

				// 输出宽度和高度
				int charWidth = size.cx * m_font_size / 100;
				int charHeight = size.cy * m_font_size / 100;


				// 绘制一个矩形
				int left = point.x - charWidth;
				int top = point.y - charHeight;
				int right = point.x + charWidth;
				int bottom = point.y + charHeight;


				m_textRect = CRect(left, top, right, bottom);
				//m_textRect = CRect(50,50,200,100);

				// 绘制文本框
				if (!m_text.IsEmpty())
				{
					CWnd* pWnd = GetDlgItem(IDC_STATIC_PIC);
					CDC* pDC = pWnd->GetDC();
					CFont* pOldFont = pDC->SelectObject(&m_font);
					pDC->SetTextColor(m_font_col);
					pDC->SetBkMode(TRANSPARENT);
					pDC->DrawText(m_text, &m_textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					pDC->SelectObject(pOldFont);
					ReleaseDC(pDC);
				}
			}
			//Invalidate(true); // 触发重绘以显示文本
		}
		m_is_text = false;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}
