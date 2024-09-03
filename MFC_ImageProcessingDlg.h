
// MFC_ImageProcessingDlg.h: 头文件
//
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#pragma once


// CMFCImageProcessingDlg 对话框
class CMFCImageProcessingDlg : public CDialogEx
{
// 构造
public:
	CMFCImageProcessingDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_IMAGEPROCESSING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	bool m_is_open = false; //判断是否打开了图片（鲁棒性）

	//文本框数据
	CRect m_textRect;  //文本框位置
	CString m_text;  //文本
	CFont m_font;   //字体
	int m_font_size; //字体大小
	COLORREF m_font_col;  //字体颜色
	bool m_is_text = false;  //判断是否为文本框模式


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	afx_msg void OnClickedOpenButton();
	afx_msg void OnClickedSaveButton();

	void CMFCImageProcessingDlg::CreateTextImage(CString& text, CRect& textRect, const CString& textImagePath);

	//定义变量存储图片信息
	BITMAPINFO* pBmpInfo;       //记录图像细节
	BYTE* pBmpData;             //图像数据
	BITMAPFILEHEADER bmpHeader; //文件头
	BITMAPINFOHEADER bmpInfo;   //信息头
	CFile bmpFile;              //记录打开文件
	CString FilePath;
	afx_msg void OnClickedRotation();

	// 显示位图
	void Show_Bmp(double hfactor,double wfactor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedBlurButton();
	afx_msg void OnClickedSharpButton();
	void Save_Open_Temp_Bmp();
	afx_msg void OnClickedScaleButton();
	void Bmp2Mat(cv::Mat img, int height, int width);
	void Mat2Bmp(cv::Mat img, int height, int width);
	double m_Scale_Height;
	double m_Scale_Width;
	afx_msg void OnBnClickedTextButton();
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

