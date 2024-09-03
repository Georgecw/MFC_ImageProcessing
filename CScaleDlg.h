#pragma once
#include "afxdialogex.h"


// CScaleDlg 对话框

class CScaleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScaleDlg)

	double m_Scale_Height_Temp;
	double m_Scale_Width_Temp;

public:
	CScaleDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CScaleDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCALE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nModalResult; //用于在主界面调用DoMadal
	afx_msg void OnClickedIdok2();
};
