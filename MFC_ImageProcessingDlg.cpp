
// MFC_ImageProcessingDlg.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "CScaleDlg.h"
#include "CTextInputDialog.h"
#include "framework.h"
#include "MFC_ImageProcessing.h"
#include "MFC_ImageProcessingDlg.h"
#include"toning_func.h"
#include <gdiplus.h>
#include <windows.h>

#pragma comment(lib, "gdiplus.lib")

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define _MinDefTmp_
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define _MaxDefTmp_
#endif


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
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCImageProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_HEIGHT_EDIT, m_Scale_Height);
	//DDX_Text(pDX, IDC_WIDTH_EDIT, m_Scale_Width);
	DDX_Control(pDX, IDC_LIST_FUNC, m_func_select);
}

BEGIN_MESSAGE_MAP(CMFCImageProcessingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_BUTTON, &CMFCImageProcessingDlg::OnClickedOpenButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &CMFCImageProcessingDlg::OnClickedSaveButton)
	ON_BN_CLICKED(IDC_Rotation, &CMFCImageProcessingDlg::OnClickedRotation)
	ON_BN_CLICKED(IDC_BLUR_BUTTON, &CMFCImageProcessingDlg::OnClickedBlurButton)
	ON_BN_CLICKED(IDC_SHARP_BUTTON, &CMFCImageProcessingDlg::OnClickedSharpButton)
	ON_BN_CLICKED(IDC_SCALE_BUTTON, &CMFCImageProcessingDlg::OnClickedScaleButton)
	ON_BN_CLICKED(IDC_BUTTON_TEXT, &CMFCImageProcessingDlg::OnBnClickedTextButton)
	ON_WM_LBUTTONDOWN()
	ON_LBN_SELCHANGE(IDC_LIST_FUNC, &CMFCImageProcessingDlg::OnSelchangeListFunc)
	ON_WM_HSCROLL()
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
	
	m_func_select.AddString(_T("顺时针旋转"));
	m_func_select.AddString(_T("缩放"));
	m_func_select.AddString(_T("添加文本框"));
	m_func_select.AddString(_T("模糊"));
	m_func_select.AddString(_T("锐化"));
	m_func_select.AddString(_T("调节饱和度"));
	m_func_select.AddString(_T("调节曝光"));
	m_func_select.AddString(_T("调节色温"));
	m_func_select.AddString(_T("图像分割"));


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

// 获取图像编码器的 CLSID
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


// 将 bmp 文件转换成 jpg, jpeg, png 文件
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
			CString tempBmpPath = _T("transfer_temp.bmp");
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
	cv::GaussianBlur(img, blurredImg, cv::Size(19, 19), 0,0);

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
	cv::GaussianBlur(img, SharpenedImg, cv::Size(3, 3), 0,0);
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

	CScaleDlg Dlg;
	if (Dlg.DoModal() == IDOK2) {

		// 2. 用 opencv resize 函数放缩图片
		if (m_Scale_Height == 0 || m_Scale_Width == 0)
		{
			AfxMessageBox(_T("倍数不能为零！"));
			return;
		}
		cv::Mat ResizedImg;
		cv::resize(img, ResizedImg, cv::Size(0, 0), m_Scale_Width, m_Scale_Height);
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
}


void CMFCImageProcessingDlg::CreateTextImage(CString& text, CRect& textRect, const CString& textImagePath,const int font_size,const COLORREF& font_col)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	Gdiplus::Bitmap bitmap(textRect.Width(), textRect.Height(), PixelFormat32bppARGB);
	Gdiplus::Graphics graphics(&bitmap);
	graphics.Clear(Gdiplus::Color(255, 255, 255, 255));

	Gdiplus::FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, font_size, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF pointF(0.0f, 0.0f);

	BYTE red = GetRValue(font_col);
	BYTE green = GetGValue(font_col);
	BYTE blue = GetBValue(font_col);

	Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, red, green, blue));

	graphics.DrawString(text, -1, &font, pointF, &solidBrush);

	CLSID clsid;
	GetEncoderClsid(L"image/bmp", &clsid);
	bitmap.Save(textImagePath, &clsid, nullptr);

	//Gdiplus::GdiplusShutdown(gdiplusToken);
}

void CMFCImageProcessingDlg::AddTextToImage(CString& text, CRect& textRect, const int font_size, const COLORREF& font_col)
{
	// 创建文本图像
	CString textImagePath = _T("text_image.bmp");
	CreateTextImage(text, textRect, textImagePath,font_size,font_col);

	// 打开文本图像
	CImage textImage;
	textImage.Load(textImagePath);

	// 将文本图像叠加到原始图像上
	CImage originalImage;
	originalImage.Attach((HBITMAP)::CreateDIBitmap(::GetDC(NULL), &bmpInfo, CBM_INIT, pBmpData, pBmpInfo, DIB_RGB_COLORS));

	CDC* pDC = CDC::FromHandle(originalImage.GetDC());
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.Attach(textImage.Detach());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	pDC->TransparentBlt(textRect.left, textRect.top, textRect.Width(), textRect.Height(), &memDC, 0, 0, textRect.Width(), textRect.Height(), RGB(255, 255, 255));

	memDC.SelectObject(pOldBitmap);
	originalImage.ReleaseDC();

	// 保存修改后的图像
	originalImage.Save(_T("output.bmp"));
}


void CMFCImageProcessingDlg::AdjustInit(int choice,int factor)
{
	//读取图像
	// 获取 CString 的缓冲区
	CT2CA pszConvertedAnsiString(FilePath);
	// 将缓冲区转换为 std::string
	std::string str(pszConvertedAnsiString);
	std::string img_path = str;
	cv::Mat src = cv::imread(img_path, 1);
	if (src.empty())
	{
		std::cout << "can not load image" << std::endl;
		return;
	}

	
	cv::Mat dst = src.clone();

	int saturation_value = 0;		//[-100, 100]		饱和度。
	double brightness_value = 1;		//[0,10]			亮度。暗~亮：[0, 1] ~ [1, 10]
	int warm_value = 0;				//[-100, 100]		暖色调。

	switch(choice){
	case 1:
		saturation_value = factor;
		dst = Saturation(dst, saturation_value);
		break;
	case 2:
		if (factor < 0)
			brightness_value = - factor / 500.0;
		else
			brightness_value = factor / 50.0;   //手动缩小了亮度的调整范围
		dst = Brightness(dst, brightness_value, 0);
		break;
	case 3:
		warm_value = factor;
		dst = ColorTemperature(dst, warm_value);
		break;
	}
	
	// 保存图像
	std::string output_path = "output_ad.bmp";
	cv::imwrite(output_path, dst);

	CString BmpName = _T(".\\output_ad.bmp");

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

	Show_Bmp();
	m_is_open = true;
	bmpFile.Close();

    
}







void CMFCImageProcessingDlg::OnBnClickedTextButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_is_open)
	{
		AfxMessageBox(_T("请先打开一张图片"));
		return;
	}
	m_is_text = true;
	m_texted = true;
	AfxMessageBox(_T("请在图片上点击鼠标左键，选择文本框位置"));
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

				// 获取图片控件的矩形区域
				CRect rect;
				pWnd->GetClientRect(&rect);   //556 453

				// 计算宽高比
				double wid_hei_ratio = (double)bmpInfo.biWidth / (double)bmpInfo.biHeight;

				// 计算缩放比例
				double scaleX = (double)rect.Width() / bmpInfo.biWidth;
				double scaleY = (double)rect.Height() / bmpInfo.biHeight;
				double scale = min(scaleX, scaleY);

				// 计算缩略图在控件区的偏移量
				int offsetX = (rect.Width() - (int)(bmpInfo.biWidth * scale)) / 2;
				int offsetY = (rect.Height() - (int)(bmpInfo.biHeight * scale)) / 2;

				// 将控件区的坐标映射回原始图片的像素坐标
				int originalX = (int)((point.x - offsetX) / scale);
				int originalY = (int)((point.y - offsetY) / scale);

				// 确保坐标在原始图片的范围内
				if (originalX < 0 || originalX >= bmpInfo.biWidth || originalY < 0 || originalY >= bmpInfo.biHeight)
				{
					AfxMessageBox(_T("坐标超出图片范围！"));
					return;
				}

				// 创建字体
				m_font.DeleteObject();
				m_font.CreatePointFont(m_font_size, _T("Arial"));

				CDC* pDC = GetDC();  // 获取设备上下文
				
				// 获取要输入的字符的宽度和高度
				CSize size = pDC->GetTextExtent(m_text);

				// 输出宽度和高度
				int charWidth = (size.cx * m_font_size / 100) / scale;
				int charHeight = (size.cy * m_font_size / 100) / scale;


				// 绘制一个矩形
				int left = originalX - charWidth / 2;
				int top = originalY - charHeight / 2;
				int right = originalX + charWidth / 2;
				int bottom = originalY + charHeight / 2;


				m_textRect = CRect(left, top, right, bottom);
				//m_textRect = CRect(50,50,200,100);

				// 添加文本到图像
				AddTextToImage(m_text, m_textRect,m_font_size,m_font_col);


				CString BmpName = _T(".\\output.bmp");
				
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

				Show_Bmp();
				m_is_open = true;
				bmpFile.Close();
			}
			else {
				AfxMessageBox(_T("不能点击图片外的区域！"));
			}
		}
		m_is_text = false;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCImageProcessingDlg::OnBnClickedPicseg()
{
	// 获取图像宽度和高度
	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;

	// 将 BMP 数据转换为 OpenCV 的 cv::Mat 格式
	cv::Mat img(height, width, CV_8UC3);
	// 假设位图数据是 BGR 格式
	Bmp2Mat(img, height, width);


	// 将图像数据转换为 k-means 所需的格式
	cv::Mat samples(img.rows * img.cols, 3, CV_32F);
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
			samples.at<float>(y + x * img.rows, 0) = pixel[0];
			samples.at<float>(y + x * img.rows, 1) = pixel[1];
			samples.at<float>(y + x * img.rows, 2) = pixel[2];
		}
	}

	// 设置 k-means 算法的参数
	int clusterCount = 3; // 你可以根据需要调整聚类数量
	cv::Mat labels;
	cv::Mat centers;
	cv::kmeans(samples, clusterCount, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0), 3, cv::KMEANS_PP_CENTERS, centers);

	// 将聚类结果转换回图像格式
	cv::Mat segmented_image(img.size(), img.type());
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			int cluster_idx = labels.at<int>(y + x * img.rows, 0);
			segmented_image.at<cv::Vec3b>(y, x) = cv::Vec3b(
				static_cast<uchar>(centers.at<float>(cluster_idx, 0)),
				static_cast<uchar>(centers.at<float>(cluster_idx, 1)),
				static_cast<uchar>(centers.at<float>(cluster_idx, 2))
			);
		}
	}
	// 将 K-means 分割后的图像转换为灰度图
	cv::Mat gray;
	cv::cvtColor(segmented_image, gray, cv::COLOR_BGR2GRAY);

	// 使用高斯模糊来减少噪声
	cv::Mat blurred;
	cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 1.5);

	// 使用Canny边缘检测器来找到边界
	double lowThreshold = 30;
	double highThreshold = 200;
	int apertureSize = 3;
	cv::Mat edges;
	cv::Canny(blurred, edges, lowThreshold, highThreshold, apertureSize);

	// 形态学操作
	// 创建内核
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)); // 矩形内核，大小3x3
	cv::Mat dilated;
	cv::Mat eroded;

	// 进行膨胀操作
	cv::dilate(edges, dilated, kernel);

	// 进行腐蚀操作
	cv::erode(dilated, eroded, kernel);

	// 连通域分析
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(eroded, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	cv::Mat connected_edges = cv::Mat::zeros(edges.size(), CV_8UC1);
	for (const auto& contour : contours) {
		if (cv::contourArea(contour) > 100) {  // 仅保留较大的连通区域
			cv::drawContours(connected_edges, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), 1);
		}
	}

	// 在处理完成后，仅在 img_copy 上进行边界应用
	cv::Mat fused_image = img.clone();
	for (int y = 0; y < img.rows; y++)
	{
		for (int x = 0; x < img.cols; x++)
		{
			if (edges.at<uchar>(y, x) > 0) // 如果是边界区域
			{
				fused_image.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 0, 0); // 设置为红色
			}
		}
	}
	// 将处理后的图像转换回位图格式
	Mat2Bmp(fused_image, height, width);

	// 显示图像
	Show_Bmp();
}

void CMFCImageProcessingDlg::OnSelchangeListFunc()
{
	// TODO: 在此添加控件通知处理程序代码
	cur_sel = m_func_select.GetCurSel();

	if (cur_sel > 4 && cur_sel <8) {
		CRect rect(700, 629, 900, 669); // 设置滑块控件的位置和大小  802 649
		m_slider_ad.Create(WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS, rect, this, IDC_SLIDER_AD);
		m_slider_ad.SetRange(0, 100); // 设置滑块控件的范围
		m_slider_ad.SetTicFreq(5);   // 设置刻度频率
		m_slider_ad.SetPos(50);       // 设置初始位置
	}



	switch (cur_sel)
	{case 0:  //旋转
		OnClickedRotation();
		break;
	case 1:  //缩放
		OnClickedScaleButton();
		break;
	case 2:  //添加文本
		OnBnClickedTextButton();
		break;
	case 3: //模糊
		OnClickedBlurButton();
		break;
	case 4: //锐化
		OnClickedSharpButton();
		break;
	case 8:  //图像分割
		OnBnClickedPicseg();
		break;

	//调色部分直接滑块条控制
	default:
		break;
	}
}


void CMFCImageProcessingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) //调色滑块条
{
	 // 确保是 slider 控件发出的消息
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_AD)
	{
		// 获取滑块的当前位置
		int m_adjust_factor = m_slider_ad.GetPos();
		AdjustInit(cur_sel - 4, m_adjust_factor);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
