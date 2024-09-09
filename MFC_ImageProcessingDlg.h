
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
	bool m_texted = false;  //图片上是否添加过文本



	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//afx_msg void OnDestroy(); // 清理全局指针
	DECLARE_MESSAGE_MAP();

public:
	afx_msg void OnClickedOpenButton();
	afx_msg void OnClickedSaveButton();


	//定义变量存储图片信息
	BITMAPINFO* pBmpInfo;       //记录图像细节
	BYTE* pBmpData;             //图像数据
	BITMAPFILEHEADER bmpHeader; //文件头
	BITMAPINFOHEADER bmpInfo;   //信息头
	CFile bmpFile;              //记录打开文件
	CString FilePath;		    //记录打开文件路径
	DWORD RowComplete;          //记录每行比特填充的字节数
	DWORD Bytes;                // 计算位图数据大小
	afx_msg void OnClickedRotation();

	// 显示位图
	void Show_Bmp(double hfactor, double wfactor);

	afx_msg void OnClickedBlurButton();
	afx_msg void OnClickedSharpButton();
	void Save_Open_Temp_Bmp();
	afx_msg void OnClickedScaleButton();
	void Bmp2Mat(cv::Mat img, int height, int width);
	void Mat2Bmp(cv::Mat img, int height, int width);

	afx_msg void OnBnClickedTextButton();

	//选择功能
	CListBox m_func_select;
	int cur_sel = 0;
	afx_msg void OnSelchangeListFunc();

	void CreateTextImage(CString& text, CRect& textRect, const CString& textImagePath, const int font_size, const COLORREF& font_col);
	void AddTextToImage(CString& text, CRect& textRect, const int font_size, const COLORREF& font_col);
	

	//图像分割
	void OnBnClickedPicseg();
	
	//调色
	void AdjustInit(int choice,int factor);
	CSliderCtrl m_slider_ad; // 声明滑块控件
	int saturation_factor = 50; // 暂存饱和度滑块位置
	int brightness_factor = 50; // 暂存亮度滑块位置
	int warmth_factor = 50; // 暂存暖色滑块位置
	

	//进度条
	CProgressCtrl m_progress;
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	
	afx_msg void OnStnClickedStaticPic();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	
	//马赛克
	bool ismosaic = 0;
	CSliderCtrl nummosaic;   
	int nummo;   //马赛克大小
	CButton m_mosaic_button;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};


